# IoT Hands-on

## Virtual network interface on `native`
* Use `tapsetup` script in RIOT repository:

```sh
./../RIOT/dist/tools/tapsetup/tapsetup -c 2
```

## task-05

```sh
./../RIOT/dist/tools/tapsetup/tapsetup -c 2
```

## task-06
Establish UDP connection 

we use `gnrc` socket implementation using other modules.

From terminal to send messages on `udp` riot Server.

```s
echo "Hello" |nc -6u fe80::1828:9fff:fec7:18de%tapbr0 8000
```
Then we send from `riot` to external Linux application. 

Does not work the last part of the tutorial. 
Sometimes does not work the binding port.
```s
/media/frank/Dati/university/iot/hands-on/Tutorials/task-06/bin/native/Task06.elf tap0 
RIOT native interrupts/signals initialized.
LED_RED_OFF
LED_GREEN_ON
RIOT native board initialized.
RIOT native hardware initialization complete.

/media/frank/Dati/university/iot/hands-on/Tutorials/task-06/bin/native/Task06.elf: ioctl TUNSETIFF: Device or resource busy
/media/frank/Dati/university/iot/hands-on/Tutorials/task-06/bin/native/Task06.elf: probably the tap interface (tap0) does not exist or is already in use
/media/frank/Dati/university/iot/hands-on/Tutorials/task-06/../RIOT/Makefile.include:547: recipe for target 'term' failed
make: [term] Error 1
```

## MQTT - SN
https://github.com/RIOT-OS/RIOT/tree/master/examples/emcute_mqttsn

1. The `CONNECT` message is split into 3 messages. The 2 additional ones are optional and used to transfer `Will` topic

When you have to integrate `MQTT-SN` to `MQTT` you need a connection between the 2 gateway. 
You can have **transparent** or **aggregating** gateway. 



## 13 IoT-LAB
1. Register to IoT-Lab
2. Configure SSH Access [link](https://www.iot-lab.info/tutorials/ssh-access/). Just create a pair key and copy your public key into your IoT-lab profile.
3. Follow this [tutorial](https://www.iot-lab.info/tutorials/riot-compilation/)

My SSH credential is: 
```s
ssh colasant@grenoble.iot-lab.info
```
To build: *(into your SSH remote connection, after have configured as (3))*
```sh
source /opt/riot.source
BOARD=iotlab-m3 make all
```

## 14 IoT-LAB Networking example for M3 Nodes
1. Connect to SSH
2. Compible the firmware in remote (in your SSH connection)
3. Copy the remote elf into your local pc. 
```s
scp colasant@grenoble.iot-lab.info:iot-lab/parts/RIOT/examples/gnrc_networking/bin/iotlab-m3/gnrc_networking.elf gnrc_networking.elf
```

## 15
1. MQTT-SN Gateway: Install [MQTT-SN gateway](https://www.eclipse.org/paho/components/mqtt-sn-transparent-gateway/)
If you have this error:
```s
src/linux/Network.h:24:10: fatal error: openssl/ssl.h: No such file or directory
 #include <openssl/ssl.h>
          ^~~~~~~~~~~~~~~
compilation terminated.
```
install:
```s
sudo apt-get install libssl-dev
```

To run:
```s
cd /media/frank/Dati/university/iot/hands-on/MQTTSNGateway
sudo ./MQTT-SNGateway 
```
N.B: I have cloned this repo 2 times: (master and develop) but I'm using develop (because experiment does not work).

Create your RIOT project: *in my case I have just copied `emcute_mqttsn` in my folder `myTest` and changed the `RIOT` base folder on `Makefile`. 

Now, you have to setup `gateway.conf` into `MQTTSNGateway`.

```s
sudo ip a a fec0:affe::1/64 dev tapbr0
```

Check CONNECTION:
``` s
sudo netstat -ltup
```

## Iot-Lab Networking
BORDER ROUTER:
```S
iotlab-node --update examples/gnrc_border_router/bin/iotlab-m3/gnrc_border_router.elf -l saclay,m3,106
sudo ethos_uhcpd.py m3-106 tap0 2001:660:5307:316a::1/64
```

```s
cd ~/A8/riot/RIOT/examples/my_firmware/bin/iotlab-m3
iotlab-node --update emcute_mqttsn.elf -l grenoble,m3,105
```
