import paho.mqtt.client as paho                         # mqtt library
import os
import json
import time
import random
from datetime import datetime
import sys
import base64
import json
import datetime

#CUSTOM, YOU NEED TO MODIFY
from mysecrets import *
# Communication with Thingsboard
TB_TOPIC = "v1/gateway/telemetry"                       # Thingsboard topic
TB_PORT = 1883                                          # data listening port

# Communication with TheThingsNetwork
TTN_HOST = 'eu.thethings.network'                                       # Host for TheThingsNetwork
TTN_PORT = 1883                                                         # TTN service Port
TTN_TOPIC = '+/devices/+/up'                                            # TTN topic

def on_publish(client,userdata,result):                 # create function for callback
    print("[SUCCESS] Data published to Thingsboard")
    pass

def on_connect(client, userdata, flags, rc) :           # connect callback for datarec in TTN
    print("Connected with result code "+str(rc))
    print(client.client_id)
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("$SYS/#")

def on_subscribe(client, userdata, mid, granted_qos) :
    print ("Subscribed")
    
def on_message(client, userdata, message) :
    generic_payload = json.loads(message.payload)       # Capturing the message arrived on TTN Topic
    print(generic_payload)
    data = base64.b64decode(generic_payload['payload_raw']).decode('utf-8')
    ts = int(time.time()*1000) # timestamp with 13 digits
    data = data.replace("<timestamp>", str(ts))
    print("Sending to TS: {}".format(data))
    client_TB.publish(TB_TOPIC, data)




# Setting Up Client F
client_TB = paho.Client("Thingsboard Broker")                     # create client object
client_TB.on_publish = on_publish                        # assign function to callback
client_TB.username_pw_set(ACCESS_TOKEN_TB_GATEWAY)       # access token from thingsboard device
client_TB.connect(THINGSBOARD_IP, TB_PORT, keepalive=60)         # establish connection
client_TB.on_connect = on_connect

# Setting up Data Receiver from TTN
datarec = paho.Client("DataRec")                        # create client for data receiver from TTN
datarec.on_message = on_message                         # define what to do when a message is received
datarec.username_pw_set(TTN_APPID, password=TTN_KEY)     # access with the right credentials
datarec.on_subscribe = on_subscribe
datarec.connect(TTN_HOST, TTN_PORT, keepalive=60)       # establish connection
datarec.subscribe(TTN_TOPIC, qos=1)
datarec.on_connect = on_connect

client_TB.loop_start()
datarec.loop_start()

#client_TB.loop_forever()
#datarec.loop_forever()


# Starting up our rsmb
try:
    while True:
        time.sleep(2)
        pass
except KeyboardInterrupt:
    print("\nClosing TTN bridge")
    datarec.disconnect()
    print("Closed")
    sys.exit()


#    # Device E data transmission to ThingsBoard
#    print("EEE ENVIRONMENTAL STATION E ")
#    ret = client_E.publish(tb_topic, payload_E)
#    print ("Payload : " + payload_E)
#    
#    print ("\n")
#    print ("\n")
#    
#    # Device F data transmission to ThingsBoard
#    print("F ENVIRONMENTAL STATION F ")
#    ret = client_F.publish(tb_topic, payload_F)
#    print ("Payload : " + payload_F)
#    print ("\n")
#    print ("\n")
#    
#    payload_E = ""
#    payload_F = ""