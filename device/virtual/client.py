import paho.mqtt.client as mqtt
import time
from random import randint
ACCESS_TOKEN = "A1_TEST_TOKEN"

def on_connect(client, userdata, flags, rc):
	if rc == 4:
		print("Connection Refused, bad user name or password - Username is empty.")
	elif rc == 5:
		print("Connection Refused, not authorized - Username contains invalid $ACCESS_TOKEN.")
	else:
		print("Connected with success. Result code:", str(rc))
	#client.subscribe("$SYS/#")

def on_message(client, userdata, msg):
	print(msg.topic, " ", str(msg.payload))

def on_publish(client,userdata,result):             #create function for callback
    print("data published \n"+ result)
    pass

client = mqtt.Client()
client.username_pw_set(username=ACCESS_TOKEN)
client.on_connect = on_connect
client.on_message = on_message

client.connect("127.0.0.1", 1883, 60)


data = {"temperature":"20", "humidity":"70"}

client.loop_start()

while True:
	time.sleep(5)
	temperature = randint(5,30)
	humidity = randint(20,100)
	data = {"temperature": temperature, "humidity":humidity}
	res = client.publish("v1/devices/me/telemetry", payload=str(data))
	print("Published telemetry:" + str(data))