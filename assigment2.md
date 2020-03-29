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

```

```
sudo netstat -ltup
mosquitto_pub -d -h "127.0.0.1" -t "v1/devices/me/telemetry" -u "$ACCESS_TOKEN" -f "telemetry-data-as-object.json"
```
