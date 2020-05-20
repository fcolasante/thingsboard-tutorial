/*
 * Copyright (C) 2015 Freie Universität Berlin
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
 * @brief       Example application for demonstrating RIOT's MQTT-SN library
 *              emCute
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#include "shell.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#include "thread.h"
#include "xtimer.h"

#define SENSORS 0
#define UNUSED(x) (void)(x)

#if SENSORS
#include "lps331ap.h"
#include "lps331ap_params.h"
#endif

#define EMCUTE_PORT         (1885U)
#define EMCUTE_ID           ("gertrud")
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define TOPIC_STD           ("v1/gateway/telemetry")
#define MSG_LEN             (512) //CHECK
#define MSG_PROTO           "{ '%s': [ { 'ts': %llu000, 'values':{'%s': %d,'%s': %d,'%s': %d,'%s': %d,'%s': %d } }]}"

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
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static char device_name[TOPIC_MAXLEN];

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");
}

static unsigned get_qos(const char *str)
{
    int qos = atoi(str);
    switch (qos) {
        case 1:     return EMCUTE_QOS_1;
        case 2:     return EMCUTE_QOS_2;
        default:    return EMCUTE_QOS_0;
    }
}

static int cmd_con(int argc, char **argv)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    char *topic = NULL;
    char *message = NULL;
    size_t len = 0;

    if (argc < 2) {
        printf("usage: %s <ipv6 addr> [port] [<will topic> <will message>]\n",
                argv[0]);
        return 1;
    }

    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, argv[1]) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (argc >= 3) {
        gw.port = atoi(argv[2]);
    }
    if (argc >= 5) {
        topic = argv[3];
        message = argv[4];
        len = strlen(message);
    }

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", argv[1], (int)gw.port);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n",
           argv[1], (int)gw.port);

    return 0;
}

static int cmd_discon(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("error: not connected to any broker");
        return 1;
    }
    else if (res != EMCUTE_OK) {
        puts("error: unable to disconnect");
        return 1;
    }
    puts("Disconnect successful");
    return 0;
}

static int cmd_pub(int argc, char **argv)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    if (argc < 3) {
        printf("usage: %s <topic name> <data> [QoS level]\n", argv[0]);
        return 1;
    }

    /* parse QoS level */
    if (argc >= 4) {
        flags |= get_qos(argv[3]);
    }

    printf("pub with topic: %s and name %s and flags 0x%02x\n", argv[1], argv[2], (int)flags);

    /* step 1: get topic id */
    t.name = argv[1];
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, argv[2], strlen(argv[2]), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                t.name, (int)t.id);
        return 1;
    }

    printf("Published %i bytes to topic '%s [%i]'\n",
            (int)strlen(argv[2]), t.name, t.id);

    return 0;
}

static int cmd_sub(int argc, char **argv)
{
    unsigned flags = EMCUTE_QOS_0;

    if (argc < 2) {
        printf("usage: %s <topic name> [QoS level]\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) > TOPIC_MAXLEN) {
        puts("error: topic name exceeds maximum possible size");
        return 1;
    }
    if (argc >= 3) {
        flags |= get_qos(argv[2]);
    }

    /* find empty subscription slot */
    unsigned i = 0;
    for (; (i < NUMOFSUBS) && (subscriptions[i].topic.id != 0); i++) {}
    if (i == NUMOFSUBS) {
        puts("error: no memory to store new subscriptions");
        return 1;
    }

    subscriptions[i].cb = on_pub;
    strcpy(topics[i], argv[1]);
    subscriptions[i].topic.name = topics[i];
    if (emcute_sub(&subscriptions[i], flags) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", argv[1]);
        return 1;
    }

    printf("Now subscribed to %s\n", argv[1]);
    return 0;
}

static int cmd_unsub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage %s <topic name>\n", argv[0]);
        return 1;
    }

    /* find subscriptions entry */
    for (unsigned i = 0; i < NUMOFSUBS; i++) {
        if (subscriptions[i].topic.name &&
            (strcmp(subscriptions[i].topic.name, argv[1]) == 0)) {
            if (emcute_unsub(&subscriptions[i]) == EMCUTE_OK) {
                memset(&subscriptions[i], 0, sizeof(emcute_sub_t));
                printf("Unsubscribed from '%s'\n", argv[1]);
            }
            else {
                printf("Unsubscription form '%s' failed\n", argv[1]);
            }
            return 0;
        }
    }

    printf("error: no subscription for topic '%s' found\n", argv[1]);
    return 1;
}

static int cmd_will(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage %s <will topic name> <will message content>\n", argv[0]);
        return 1;
    }

    if (emcute_willupd_topic(argv[1], 0) != EMCUTE_OK) {
        puts("error: unable to update the last will topic");
        return 1;
    }
    if (emcute_willupd_msg(argv[2], strlen(argv[2])) != EMCUTE_OK) {
        puts("error: unable to update the last will message");
        return 1;
    }

    puts("Successfully updated last will topic and message");
    return 0;
}


#if SENSORS
static int cmd_sensors(int argc, char ** argv){
    UNUSED(argc);
    UNUSED(argv);
    lps331ap_t dev;
    printf("Initializing LPS331AP sensor\n");
    if (lps331ap_init(&dev, &lps331ap_params[0]) == 0) {
        puts("[OK]\n");
    }else{
        puts("Initialization failed");
        return 1;
    }

    uint16_t pres;
    int16_t temp;
    temp = lps331ap_read_temp(&dev);
    pres = lps331ap_read_pres(&dev);
    int temp_abs = temp / 100;
    temp -= temp_abs * 100;
    printf("Pressure value: %ihPa - Temperature: %2i.%02i°C\n",
               pres, temp_abs, temp);
    
    return 0;
}
#endif


static int cmd_telemetry(int argc, char ** argv){
    UNUSED(argc);
    UNUSED(argv);
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_1;
    t.name = TOPIC_STD;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }
    char buffer[MSG_LEN];
    snprintf(buffer, MSG_LEN, MSG_PROTO, device_name, (unsigned long long int)time(NULL),
                "temperature", rand() % (MAX_TEMPERATURE + 1 - MIN_TEMPERATURE) + MIN_TEMPERATURE,
                "humidity", rand() % (MAX_HUMIDITY + 1 - MIN_HUMIDITY) + MIN_HUMIDITY,
                "wind_direction", rand() % (MAX_WIND_DIRECTION + 1 - MIN_WIND_DIRECTION) + MIN_WIND_DIRECTION,
                "wind_intensity", rand() % (MAX_WIND_INTENSITY + 1 - MIN_WIND_INTENSITY) + MIN_WIND_INTENSITY,
                "rain_height", rand() % (MAX_RAIN_HEIGHT + 1 - MIN_RAIN_HEIGHT) + MIN_RAIN_HEIGHT
            );

    if (emcute_pub(&t, buffer, strlen(buffer), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                t.name, (int)t.id);
        return 1;
    }

    printf("Published %i bytes to topic '%s [%i] msg: %s'\n",
            (int)strlen(buffer), t.name, t.id, buffer);

    return 0;
}
void *thread_handler(void *arg)
{
    UNUSED(arg);
    while(1){
        printf("Time: %llu\n", xtimer_now64().ticks64);
        xtimer_sleep(5);
    }
    return NULL;
}

static int cmd_pub_data(int argc, char ** argv){
    UNUSED(argc); UNUSED(argv);
    printf("Starting new thread to publish data");
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler, NULL,
                  "thread pub data");
    return 0;
}

static int cmd_timestamp(int argc, char ** argv){
    UNUSED(argc); UNUSED(argv);
    printf ( "Current local time and date: %llu000\n",(unsigned long long int)time(NULL));
    return 0;
}
static int cmd_set_dev(int argc, char ** argv){
    if (argc < 2) {
        printf("usage: %s <device_name>\n",
                argv[0]);
        return 1;
    }
    printf("Dev name: %s\n", argv[1]);
    strcpy(device_name, argv[1]);
    printf("Setted: %s\n", device_name);
    return 0;
}
static const shell_command_t shell_commands[] = {
    { "con", "connect to MQTT broker", cmd_con },
    { "discon", "disconnect from the current broker", cmd_discon },
    { "pub", "publish something", cmd_pub },
    { "sub", "subscribe topic", cmd_sub },
    { "unsub", "unsubscribe from topic", cmd_unsub },
    { "will", "register a last will", cmd_will },
    { "multi_thread", "print time continously on a new thread", cmd_pub_data},
    #if SENSORS
    { "sensors", "retrieve data sensors", cmd_sensors},
    #endif
    { "pub_telemetry", "publish default msg on default topic", cmd_telemetry},
    { "timestamp", "get current timestamp", cmd_timestamp},
    { "set_device", "set device name", cmd_set_dev},
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("MQTT-SN example application\n");
    puts("Type 'help' to get started. Have a look at the README.md for more"
         "information.");

    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(queue, (sizeof(queue) / sizeof(msg_t)));

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
