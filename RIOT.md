# Setting up RIOT and MQTTSN

1. Clone `RIOT` repository
2. Setup `RIOTBASE ?= ...` into `Makefile`

```sh
cd /media/frank/Dati/university/iot/hands-on/Tutorials/RIOT/dist/tools/tapsetup
./tapsetup -c 2
sudo ip a a fec0:affe::1/64 dev tapbr0
ifconfig tapbr0 #check whether this ip is present
```

Build Iot-Device:
```sh
cd riot/emcute_mqttsn
make all term
ifconfig 5 add fec0:affe::99
con
```

```S
sudo netstat -ltup
mosquitto_pub -d -h "127.0.0.1" -t "v1/devices/me/telemetry" -u "$ACCESS_TOKEN" -f "telemetry-data-as-object.json"
```


# IoT-LAB

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

## IoT-LAB Networking example for M3 Nodes
1. Connect to SSH
2. Compible the firmware in remote (in your SSH connection)
3. Copy the remote elf into your local pc. 
```s
scp colasant@grenoble.iot-lab.info:iot-lab/parts/RIOT/examples/gnrc_networking/bin/iotlab-m3/gnrc_networking.elf gnrc_networking.elf
```

# Issues
Errors:
SSH config:
```s
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
```