# Thingsboard Tutorial
In this repository I will create a cloud-based IoT system that collects information from a set of virtual environmental sensors using the MQTT protocol. I will also create a simple web site to display the data collected from the sensors.

- Tutorial 1 [(link)](https://medium.com/@colasante.francesco/how-to-setup-an-iot-system-using-thingsboard-b705c9189e37)
- Tutorial 2 [(link)](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7)
- Tutorial 3 [(link)](https://medium.com/@colasante.francesco/3-how-to-connect-an-iot-device-to-thingsboard-using-lorawan-the-thingsnetwork-and-riot-os-46ce20dc7d5b?sk=a74c2984c610f701e5035f5acff2643b)
- Tutorial 4 [(link)](https://medium.com/@colasante.francesco/4-how-to-create-a-crowd-sensing-application-using-generic-sensor-api-and-javascript-47e3ff1df49e)

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
```s
sudo service mosquitto stop
sudo kill -9 <PID>
```
otherwise I can just swap `mqtt` binding port on docker command.


## Settings Thingsboard
In order to have a clear vision of How Thingsboard works you can see this complete video: [link](https://thingsboard.io/docs/getting-started-guides/helloworld/#video-tutorial)

it would have been impossible to briefly explain all the steps via *UI*, to complete the setup I **highly recommend you to see see my short video** on [YouTube](https://youtu.be/jm8vQPPjoS0).

In the video, I show you how to:
1. Create **device**
2. Create **asset** and setup *Relations*
3. Setup **Rule chains** : `Input -> Message Type Switch -> Post Telemetry -> Save Timeseries`
4. Setup **Entity aliases** used to Datasource into the widgets. 
5. Create **dashboards** which are linked together
6. Add **Widgets** into Dashboard
7. Build custom **data-processing code** to manipulate data

Moreover, it is mandatory to **change DEFAULT PASSWORD** if your solutions is online.

## Fake IoT device
Clone this repositort and install all dependencies, *I suggest you to use `venv`*:
```
git clone https://github.com/fcolasante/thingsboard-tutorial.git
cd device
pip install -r requirements.txt
mv secrets.example.py secrets.py
```
Update `secrets.py` setting `OPEN_WHEATER_API` and `ACCESS_TOKEN`. Each device should have a different `ACCESS_TOKEN`.
Go to Thingsboard panel and **copy it**. 

**N.B.** In order to simplify teacher's testing, I have  also versioned (for  a limited period) `mysecrets.py`. To rapidly deploy my configuration, launch:
```shell
mv mysecrets.py secrets.py
```
In the `fake-iot-device.py` you can setup many parameters:
- `city`: to choose the OpenWheater city
- `Production`: if `True` data are pushed on `PUBLIC_IP` , otherwise it sends data to `localhost`.
- `generate_data(openWeather=False)`:if `True` data are fetched by `OpenWeather API`, otherwise it produces 
random data.
- `sleep_time`: time between 2 different telemetry datas.

```shell
cd virtual
python3 fake-iot-device.py
```
You can check if it works accessing as customer (Email: *customer@thingsboard.org* Password: *customer*) visualizing **Dashboard demo**.
To run different devices you need to edit `getDeviceToken`.
*I choose to not enter the token by the keyboard because it is not user-friendly and you should copy it every time.*

### Fake Iot Asset
In order to run a **set of devices** on the same script, run `fake-asset.py`. You have to configure the array `CITIES` into the file `secrets.py`. *The example below does not work,it is for illustration purposes only, tokens are invented*.

```python
CITIES = [['V12mMG95NmrJ8Wu4asd', 'Chieti'],
            ['TldN65VTzMukZnAWasdf', 'Francavilla al Mare'],
            ['ssWgp3cschyZJf3asdf', "L\'Aquila"],
            ['qZdNlOVhuMKf7fRasdf', 'Teramo']
        ]
```
The first element represents the `ACCESS TOKEN` of the corresponding thingsboard device, the second element is used by the **OpenWheater API** to fetch real environmental data of the `CITY`.
```shell
python3 fake-asset.py
```
You can check if it works accessing as customer (Email: *customer@thingsboard.org* Password: *customer*) visualizing **Dashboard > Dati comuni abruzzesi**.

## Deploy on Digital Ocean
Get your 50$ from Education Github ad [link](https://education.github.com/pack/offers#digitalocean)
In order to **setup Thingsborad on Digital Ocean** follow this steps:

1. https://thingsboard.io/docs/user-guide/install/digital-ocean/
2. https://www.digitalocean.com/docs/droplets/how-to/connect-with-ssh/openssh/
3. https://thingsboard.io/docs/user-guide/install/ubuntu/

### Public IP
You can access on my Thingsboard solution hosted on Digital Ocean Droplet by this [Public IP](http://64.227.26.128:8080). The public credentials are: 
- Email: `customer@thingsboard.org`
- Password: `customer`

## Video and Article
Read my article and see the video in order to have a better view of this project.
- [YouTube video](https://youtu.be/jm8vQPPjoS0)
- [Medium article](https://medium.com/@colasante.francesco/how-to-setup-an-iot-system-using-thingsboard-b705c9189e37)


## Tutorial 2 

Check the Tutorial #2 available at [link](RIOT.md).

- [YouTube video](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- [Medium blogpost](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7)

It provides:
- riot firmware
- MQTT-SN configuration
- iot-lab hands-on


## Tutorial 3
Check the Tutorial #3 available at [link](LoRaWAN.md).

- [YouTube](https://youtu.be/mMJN9AqVCzI)
- [Medium article](https://medium.com/@colasante.francesco/3-how-to-connect-an-iot-device-to-thingsboard-using-lorawan-the-thingsnetwork-and-riot-os-46ce20dc7d5b?sk=a74c2984c610f701e5035f5acff2643b)



It provides:
- LoRaWAN connection
- TheThingsNetwork getting started
- MQTT gateway 
- iot-lab testbed

## Tutorial 4
Check the Tutorial #3 available at [link](crowd-sensing/README.md).

- [YouTube](https://youtu.be/p3gq2VLTSiI)
- [Medium article](https://medium.com/@colasante.francesco/4-how-to-create-a-crowd-sensing-application-using-generic-sensor-api-and-javascript-47e3ff1df49e)


It provides:
- Crowd sensing
- Human recognition activity
- Generic Sensor API
- Thingsboard Dashboard
- HTML5 + Javascript web application
- cloud / edge architecture

Tutorials of the [Internet of Things course 2020](http://ichatz.me/Site/InternetOfThings2020), part of the MSc on Computer Engineering, Department of Computer, Control and Management Engineering (DIAG), Sapienza University of Rome.

