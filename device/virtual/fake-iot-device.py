import paho.mqtt.client as mqtt
import time
from random import randint
import requests
import json
from secrets import getOpenWeatherToken, getDeviceToken

ACCESS_TOKEN = getDeviceToken(1)
PUBLIC_IP = '64.227.26.128'
OPEN_WEATHER_TOKEN = getOpenWeatherToken()
MIN_TEMPERATURE = -50
MAX_TEMPERATURE = 50
MIN_HUMIDITY = 0
MAX_HUMIDITY = 100
MIN_WIND_DIRECTION = 0
MAX_WIND_DIRECTION = 360
MIN_WIND_INTENSITY = 0
MAX_WIND_INTENSITY = 100
MIN_RAIN_HEIGHT = 0
MAX_RAIN_HEIGHT = 50

city="Guardiagrele"
Production = True
if Production:
	IP= PUBLIC_IP
	sleep_time = 5
else:
	IP= '127.0.0.1'
	sleep_time = 5

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

print("IP", IP)
client.connect(IP, 1883, 60)

def generate_data(openWeather=False):
	if openWeather:
		response = requests.get('https://api.openweathermap.org/data/2.5/weather?q={}&appid={}&units=metric'.format(city, OPEN_WEATHER_TOKEN))
		weather_data = response.json()
		data = {"temperature": weather_data['main']['temp'],
			"humidity": weather_data['main']['humidity'], 
			"wind_direction":  weather_data['wind'].get('deg', randint(MIN_WIND_DIRECTION, MAX_WIND_DIRECTION)), 
			"wind_intensity": weather_data['wind']['speed'] if weather_data['wind']  else randint(MIN_WIND_DIRECTION, MAX_WIND_DIRECTION), 
			"rain_height": randint(MIN_RAIN_HEIGHT, MAX_RAIN_HEIGHT)}
	else:
		data = {"temperature": randint(MIN_TEMPERATURE, MAX_TEMPERATURE),
				"humidity": randint(MIN_HUMIDITY, MAX_HUMIDITY), 
				"wind_direction": randint(MIN_WIND_DIRECTION, MAX_WIND_DIRECTION),
				"wind_intensity": randint(MIN_WIND_INTENSITY, MAX_WIND_INTENSITY), 
				"rain_height": randint(MIN_RAIN_HEIGHT, MAX_RAIN_HEIGHT)}
	return data
client.loop_start()

while True:
	data = generate_data(False)
	res = client.publish("v1/devices/me/telemetry", payload=str(data), qos=1)
	print("Published telemetry:" + str(data))
	print(res)
	time.sleep(sleep_time)