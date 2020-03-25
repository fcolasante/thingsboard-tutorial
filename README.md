# Thingsboard Tutorial
In this repository I will create a cloud-based IoT system that collects information from a set of virtual environmental sensors using the MQTT protocol. I will also create a simple web site to display the data collected from the sensors.

## Thingsboard
I will create 2 different setup:
- on premise: using **Docker**
- cloud: using **Digital Ocean**

### Docker
```s
docker run -it -p 9090:9090 -p 1883:1883 -p 5683:5683/udp -v ~/.mytb-data:/data -v ~/.mytb-logs:/var/log/thingsboard --name mytb --restart always thingsboard/tb-postgres
```

Ports:
- Admin Panel: `-p 9090:9090` - connect local port 9090 to exposed internal HTTP port 9090
- MQTT Broker: `-p 1883:1883` - connect local port 1883 to exposed internal MQTT port 1883
- CoAP Gateway: `-p 5683:5683` - connect local port 5683 to exposed internal COAP port 5683

## Account access
[link](https://thingsboard.io/docs/samples/demo-account/)

- Sysadmin (sysadmin@thingsboard.org)
- Tenants: provision customer, device, things (tenant@thingsboard.org)
- Users
- Customers: (customer@thingsboard.org)

## MQTT API

```s
https://thingsboard.io/docs/reference/mqtt-api/
```

### Mosquitto
I used Mosquitto_pub to test wheather MQTT broker works. Once I have tested it, I stopped the `mosquitto` service. Otherwise, the `1883` port remains used by Mosquitto. 

```s
sudo apt install mosquitto mosquitto-cSlients
export ACCESS_TOKEN='A1_TEST_TOKEN'
```
Download file *(or write yourself)*
```s
mosquitto_pub -d -h "127.0.0.1" -t "v1/devices/me/telemetry" -u "$ACCESS_TOKEN" -f "telemetry-data-as-object.json"
```

Parameters:
```
-u, --username
           Provide a username to be used for authenticating with the broker. This requires a
           broker that supports MQTT v3.1. See also the --pw argument.
```


## Restart Thingsboard container
Check if ports is free:
```
sudo netstat -tulpn | grep LISTEN
```
In my case I have been installed `mosquitto` hence I need to stop it.
```
sudo service mosquitto stop
```
otherwise I can just swap `mqtt` binding port on docker command.

https://thingsboard.io/docs/getting-started-guides/helloworld/#video-tutorial

## Virtual sensor
In the `fake-iot-device.py` you can setup many parameters:
- `city`: to choose the OpenWheater city
- `Production`: if `True` data are pushed on public ip, otherwise it sends data to localhost.
- `generate_data(openWeather=False)`:if `True` data are fetched by `OpenWeather API`, otherwise it produces 
random data.
- `sleep_time`: time between 2 different dat
## Digital Ocean
Get your 50$ from Education Github ad [link](https://education.github.com/pack/offers#digitalocean)

### Public IP
[Public access](http://64.227.26.128:8080)
- Email: `customer@thingsboard.org`
- Password: `customer`

### How to setup Thingsboard on Digital Ocean
1. https://thingsboard.io/docs/user-guide/install/digital-ocean/
2. https://www.digitalocean.com/docs/droplets/how-to/connect-with-ssh/openssh/
3. https://thingsboard.io/docs/user-guide/install/ubuntu/

## Video and Article

- [YouTube video](https://medium.com/@colasante.francesco/how-to-setup-an-iot-system-using-thingsboard-b705c9189e37)
- [Medium article](https://medium.com/@colasante.francesco/how-to-setup-an-iot-system-using-thingsboard-b705c9189e37)