# Thingsboard Tutorial
In this repository I will create a cloud-based IoT system that collects information from a set of virtual environmental sensors using the MQTT protocol. I will also create a simple web site to display the data collected from the sensors.

## Thingsboard
I will create 2 different Thingsboard setup:
- on premise: using **Docker** for development
- on cloud: using **Digital Ocean** for production

You have different user roles: sysadmin, tenant, user and customer. See this [link](https://thingsboard.io/docs/samples/demo-account/) to check default email and password

## Installing Thingsboard using Docker
```s
docker run -it -p 9090:9090 -p 1883:1883 -p 5683:5683/udp -v ~/.mytb-data:/data -v ~/.mytb-logs:/var/log/thingsboard --name mytb --restart always thingsboard/tb-postgres
```

Ports:
- Admin Panel: `-p 9090:9090` - connect local port 9090 to exposed internal HTTP port 9090
- MQTT Broker: `-p 1883:1883` - connect local port 1883 to exposed internal MQTT port 1883
- CoAP Gateway: `-p 5683:5683` - connect local port 5683 to exposed internal COAP port 5683

The complete guide is available at this [link](https://thingsboard.io/docs/user-guide/install/docker/)

### MQTT API

```s
https://thingsboard.io/docs/reference/mqtt-api/
```

#### Mosquitto
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

After have tested that `mosquitto_pub` communicates with Thingsboard you can start to run your fake-iot-device. In this short tutorial is simply a small Python script.


### Restart Thingsboard container
Check if ports is free:
```
sudo netstat -tulpn | grep LISTEN
```
In my case I have been installed `mosquitto` hence I need to stop it.
```
sudo service mosquitto stop
```
otherwise I can just swap `mqtt` binding port on docker command.

## Fake IoT device
Install all dependencies, *I suggest you to use `venv`*:
```
cd device
pip install -r requirements.txt
```

In the `fake-iot-device.py` you can setup many parameters:
- `city`: to choose the OpenWheater city
- `Production`: if `True` data are pushed on public ip, otherwise it sends data to localhost.
- `generate_data(openWeather=False)`:if `True` data are fetched by `OpenWeather API`, otherwise it produces 
random data.
- `sleep_time`: time between 2 different dat

```shell
cd virtual
python3 fake-iot-device.py
```
### Fake Iot Asset
In order to run a **set of devices** on the same script, run `fake-asset.py`. You have to configure the array `CITIES` into the file `secrets.py`. 
```shell
CITIES = [['V12mMG95NmrJ8Wu4asd', 'Chieti'],
            ['TldN65VTzMukZnAWasdf', 'Francavilla al Mare'],
            ['ssWgp3cschyZJf3asdf', 'L\'Aquila'],
            ['qZdNlOVhuMKf7fRasdf', 'Teramo']
        ]
```
The first element represents the `ACCESS TOKEN` of the corresponding thingsboard device, the second element is used by the **OpenWheater API** to fetch real environmental data.
```shell
python3 fake-asset.py
```

## Settings Thingsboard
In order to have a clear vision of How Thingsboard works you can see this complete video: [link](https://thingsboard.io/docs/getting-started-guides/helloworld/#video-tutorial)

it would have been impossible to briefly explain all the steps via *UI*, to complete the setup I **highly recommend you to see see my short video** on [YouTube](https://youtu.be/jm8vQPPjoS0).

## Deploy on Digital Ocean
Get your 50$ from Education Github ad [link](https://education.github.com/pack/offers#digitalocean)
In order to **setup Thingsborad on Digital Ocean** follow this steps:

1. https://thingsboard.io/docs/user-guide/install/digital-ocean/
2. https://www.digitalocean.com/docs/droplets/how-to/connect-with-ssh/openssh/
3. https://thingsboard.io/docs/user-guide/install/ubuntu/

### Public IP
[Public access](http://64.227.26.128:8080)
- Email: `customer@thingsboard.org`
- Password: `customer`

## Video and Article

- [YouTube video](https://youtu.be/jm8vQPPjoS0)
- [Medium article](https://medium.com/@colasante.francesco/how-to-setup-an-iot-system-using-thingsboard-b705c9189e37)