/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example demonstrating the use of LoRaWAN with RIOT
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"
#include "hts221.h"
#include "hts221_params.h"

#include "periph/rtc.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#define TOPIC_MAXLEN        (64U)
#define TOPIC_STD           ("v1/gateway/telemetry")
#define MSG_LEN             (512) //CHECK
#define MSG_PROTO           "{ '%s': [ { 'ts': %llu000, 'values':{'%s': %d,'%s': %d,'%s': %d,'%s': %d,'%s': %d } }]}"
#define MSG_PROTO_V2        "{ '%s': [ { 'ts': <timestamp>, 'values':{'%s': %d,'%s': %d, '%s': %d, '%s': %d,'%s': %d} }]}"

#define MIN_TEMPERATURE -50
#define MAX_TEMPERATURE 50
#define MIN_HUMIDITY 0
#define MAX_HUMIDITY 100
#define MIN_WIND_DIRECTION 0
#define MAX_WIND_DIRECTION 360
#define MIN_WIND_INTENSITY 0
#define MAX_WIND_INTENSITY 100
#define MIN_RAIN_HEIGHT 0
#define MAX_RAIN_HEIGHT 50

//CUSTOM PARAMS
#define DEVICE_NAME         "device_teramo"
#define PERIOD              (5U)
//END CUSTOM PARAMS

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

semtech_loramac_t loramac;

static hts221_t hts221;

static uint8_t deveui[LORAMAC_DEVEUI_LEN]; //LORAMAC_DEVEUI_LEN
static uint8_t appeui[LORAMAC_APPEUI_LEN]; //LORAMAC_APPEUI_LEN
static uint8_t appkey[LORAMAC_APPKEY_LEN]; //LORAMAC_APPKEY_LEN

static char device_name[TOPIC_MAXLEN];

int get_rnd(int min, int max) { 
    int randValue = (rand() % (max - min + 1)) + min;
    return randValue;
}

void get_payload(char* message){
    int16_t temp = 0;
    uint16_t hum = 0;
    if (hts221_read_humidity(&hts221, &hum) != HTS221_OK) {
        puts(" -- failed to read humidity!");
    }
    if (hts221_read_temperature(&hts221, &temp) != HTS221_OK) {
        puts(" -- failed to read temperature!");
    }
    printf("Device name %s\n",device_name);
    snprintf(message, MSG_LEN, MSG_PROTO_V2, device_name,
                "temperature", temp/10,
                "humidity", hum/10,
                "wind_direction", get_rnd(MIN_WIND_DIRECTION, MAX_WIND_DIRECTION),
                "wind_intensity", get_rnd(MIN_WIND_INTENSITY, MAX_WIND_INTENSITY),
                "rain_height", get_rnd(MIN_RAIN_HEIGHT, MAX_RAIN_HEIGHT)
            );
}


static void _send_message(char *message)
{
    printf("Sending: %s\n", message);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,
                                       (uint8_t *)message, strlen(message));
    if (ret != SEMTECH_LORAMAC_TX_OK) {
        printf("Cannot send message '%s', ret code: %d\n", message, ret);
        return;
    }
    /* The send was successfully scheduled, now wait until the send cycle has
       completed and a reply is received from the MAC */
    semtech_loramac_recv(&loramac);
}

static void *sender(void *arg)
{
    (void)arg;

    while (1) {
        char msg[MSG_LEN];
        get_payload(msg);
        /* Trigger the message send */
        _send_message(msg);
        xtimer_sleep(PERIOD);
        
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

    if (hts221_init(&hts221, &hts221_params[0]) != HTS221_OK) {
        puts("Sensor initialization failed");
        LED3_TOGGLE;
        return 1;
    }
    if (hts221_power_on(&hts221) != HTS221_OK) {
        puts("Sensor initialization power on failed");
        LED3_TOGGLE;
        return 1;
    }
    if (hts221_set_rate(&hts221, hts221.p.rate) != HTS221_OK) {
        puts("Sensor continuous mode setup failed");
        LED3_TOGGLE;
        return 1;
    }
    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, DEVEUI);
    fmt_hex_bytes(appeui, APPEUI);
    fmt_hex_bytes(appkey, APPKEY);

    strncpy(device_name, DEVICE_NAME, TOPIC_MAXLEN);

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    /* trigger the first send */
    //msg_t msg;
    //msg_send(&msg, sender_pid);
    return 0;
}