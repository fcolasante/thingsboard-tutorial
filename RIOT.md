#  Build IoT device using RIOT-OS, MQTT and Thingsboard
- Check the previous [guide](README.md).
- Watch my video on [YouTube](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- Read my [article](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7) on Medium

## Architecture
![IoT architecture with MQTT-SN](https://miro.medium.com/max/1000/1*ZxxghBwu89XkaN-rAqwFIQ.png)

1. Pull the repository:
```s
git pull
git submodule update --init --recursive
```
2. Setup `RIOT`. Follow the official [Getting Started](https://github.com/RIOT-OS/Tutorials/blob/master/README.md)


3. Setup `RIOTBASE ?= ...` into  the `device/riot/sensors_mqtttsn/Makefile`

## Setup virtual network
```s
cd device/riot/sensors_mqttsn
./start_network.sh
```
Then, run the MQTT-SN transparent gateway and the MQTT broker.


## Run Gateway and broker

### Mosquitto as bridge
Update `bridge_gateway_[dev|prod].conf` with your parameters and run it. You need to setup:
- `address` 
- `remote_username` 

```s
mosquitto -c brokers/conf/bridge_gateway_prod.conf 
```

## Paho MQTT-sn
It performs as MQTT-SN broker (RIOT devices connect to him) and as MQTT-SN -MQTT transparent gateway.

Setup custom `gateway.conf`file:
```s
cd brokers/MQTTSN-Gateway
./custom_install.sh
mv gateway.conf gateway.conf.bkp
cp ../conf/MQTTSN-Gateway.conf gateway.conf
```

Then, run the MQTT-SN MQTT transparent gateway:
```s
cd  brokers/MQTTSN-Gateway
./MQTT-SNGateway
```


## RIOT Usage
```s
PORT=tap0 make clean all term
```
- To connect to a broker, use the `con` command:
On `riot-shell`:
```s
ifconfig 5 add fec0:affe::99
con fec0:affe::1 1885
set_device "Device Piano"
pub_telemetry
```
you can also use:
```s
pub v1/gateway/telemetry "{ 'Device Piano': [ { 'ts': 1585744760000, 'values':{'humidity': 42 }}]}" 1
```


### Other utilities

This commands can be used for testing:
```s
sudo netstat -ltup
mosquitto_pub -d -h "127.0.0.1" -t "v1/devices/me/telemetry" -u "$ACCESS_TOKEN" -f "telemetry-data-as-object.json"
```


# IoT-LAB

1. Register to IoT-Lab. (*Replace*  `colasant` *with your iot-lab username*) 
2. Configure SSH Access [link](https://www.iot-lab.info/tutorials/ssh-access/). Just create a pair key and copy your public key into your IoT-lab profile.
3. Follow this [tutorial (riot-compilation)](https://www.iot-lab.info/tutorials/riot-compilation/)
4. Complete all tutorials about RIOT on [iot-lab](https://www.iot-lab.info/tutorials/): Networking, Public IPv6, MQTT-SN.

Login with SSH in your remote machine: 
```s
ssh colasant@grenoble.iot-lab.info
```
To build: *(into your SSH remote connection, after have done (1,2,3,4))*
```sh
    source /opt/riot.source
    BOARD=iotlab-m3 make all
    iotlab-experiment submit -n riot_m3 -d 60 -l 1,archi=m3:at86rf231+site=saclay
    iotlab-experiment get -i <exp_id> -r
    iotlab-node --update <your_binary>.elf -l saclay,m3,<dev_id>
    nc <dev_id> 20000
```
In the previous example I am using `M3` devices. If you want to use `a8-m3` devices, the steps are different, check [iot-lab tutorials](https://www.iot-lab.info/tutorials/)

## IoT-LAB Networking example for M3 Nodes
1. Connect to SSH
2. Compile the firmware in remote (in your SSH connection)
3. Copy the remote `elf` into your local pc if you want to use iot-lab web interface.
```s
scp colasant@grenoble.iot-lab.info:iot-lab/parts/RIOT/examples/gnrc_networking/bin/iotlab-m3/gnrc_networking.elf gnrc_networking.elf
```


# Issues
Common errors:
- If you cannot connect to the nodes due to SSH configuration: (*make sure that you have also copied your private key*)
```s
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
```
- `lps331ap`sensors are not available on M3 subsystem of A8 device. (It works only on M3).

- When you are performing networking tutorials, pay attention to the subnets you propagate. Consult the official [documentation](https://www.iot-lab.info/tutorials/understand-ipv6-subnetting-on-the-fit-iot-lab-testbed/).

Example: `node-a8-101.saclay.iot-lab.info` will have `2001:660:3207:0465::/64` as `	/64 subnet on M3 interface`.


- `miniterm.py` is bugged on `2019.01` branch.  In the new version `2020.01` has been fixed, you can avoid using the `-e` parameter.

- Check the my Hands-on [tips](hands-on.md) about RIOT tutorials.

- if you have problems contact me on [Linkedin](https://www.linkedin.com/in/francesco-colasante/) or open a new issue. 


Tutorial of the [Internet of Things course 2020](http://ichatz.me/Site/InternetOfThings2020), part of the MSc on Computer Engineering, Department of Computer, Control and Management Engineering (DIAG), Sapienza University of Rome.


- [YouTube](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- [Medium article](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7)