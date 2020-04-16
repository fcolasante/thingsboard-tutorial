# How to connect an IoT device to Thingsboard using LoRaWAN, The ThingsNewtwork and RIOT-OS
- Check the previous guides: tutorial [1](README.md), tutorial [2](RIOT.md)

- Watch my video on [YouTube](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- Read my [blogpost](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7) on Medium


## Architecture
![IoT architecture with MQTT-SN](https://cdn-images-1.medium.com/max/800/1*Wpt6eyg_YSJCJVPZgwxG5A.png)

## Requirements 
- RIOT
- IoT-LAB
- TheThingsNetwork (TTN)
- Thingsboard
- Paho MQTT library
- LoRaWAN

### Hardware
This is the device that we will use on the iot-lab, you do not need to buy it
- b-l072z-lrwan1 [documentation](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)


First of all, you need to `git pull` or `clone` this repository.
*If you have not done it yet, I suggest you do it.*

In this tutorial, you will use this 2 folders:
- `device/riot/sensors_lora`
- `brokers/TTN_BT_Gateway`

The other folders are useful for previous tutorials
## Setup TTN

1. Create an account on [TheThingsNetwork](https://account.thethingsnetwork.org/register)
2. Create an application on [TTN](https://www.thethingsnetwork.org/docs/applications/add.html)
3. Register [two devices](https://www.thethingsnetwork.org/docs/devices/registration.html)

Choose name which you prefer as `Application ID` and `Device ID`.

4. Setup payload decoding through `console > application > Payload Formats` and add the following 
`javascript` code  to the **decoder** section:
```javascript
function Decoder(bytes, port) {
  let result = "";
  for (let byte in bytes) {
    result += String.fromCharCode(bytes[byte]);
  }
  return {"string": result};
}
```


## IoT-LAB

1. Access to `saclay`: `ssh <username>@saclay.iot-lab.info`
2. Clone RIOT repository (already done see Tutorial 2)
3. Setup `gcc compiler`
```s
cd ~/RIOT
source /opt/riot.source
export PATH=/opt/gcc-arm-none-eabi-7-2017-q4-major/bin:$PATH
arm-none-eabi-gcc --version # to check wheter is 7.x
mkdir ~/RIOT/examples/mylorawan
```
4. Config your custom TTN params into `Makefile` and `main.c`
```Makefile
DEVEUI ?= 00EDBC97D72E55AA
APPEUI ?= 70B3D57ED002DBAA
APPKEY ?= AA352D887CC62A69DEE4B46983DEAAAA
```
Replace `device_teramo` with your Thingsboard device name.
```c
//CUSTOM PARAMS
#define DEVICE_NAME         "device_teramo"
#define PERIOD              (5U)
//END CUSTOM PARAMS
```

5. Copy RIOT firmware using `scp` (from your pc to IoT-LAB).
```s
cd device/riot
scp sensors_lora/* colasant@saclay.iot-lab.info:~/RIOT/examples/mylorawan
```
5. Compile your riot-firmware
```
make -C examples/mylorawan clean all
```
6. Launch new experiment:
```s
iotlab-experiment submit -n riot_ttn -d 120 -l 1,archi=st-lrwan1:sx1276+site=saclay
iotlab-experiment get -r
```
Keep note of your `<dev_id>`

6. Flash `.elf` file on your device
```s
iotlab-node --update lorawan.elf -l saclay,st-lrwan1,<dev_id>
```
7. Access to your device using `netcat`
```s
nc st-lrwan1-<dev_id> 20000
```

Now your device is running

## Transparent Gateway
1. Setup your secrets data in `mysecrets.example.py` and then:
Edit:
`mysecrets.py`:
```python
ACCESS_TOKEN_TB_GATEWAY = 'REPLACE_WITH_THINGSBOARD_GATEWAY_AT'
THINGSBOARD_IP = "REP_WITH_PUB_IP_OR_DEMO_THINGSBOARD.io"

TTN_APPID = 'REPLACE_WITH_YOUR_APP_ID'
TTN_KEY = 'REPLACE_WITH_YOUR_TTN_ACCESS_KEY' 
```
```s
mv mysecrets.example.py myscrets.py
```

## Check Thingsboard dashboard


## Helps
If you have some problem, I suggest you to do this tutorial on [IoT-LAB](https://www.iot-lab.info/tutorials/riot-ttn/)


# Details

Tutorial of the [Internet of Things course 2020](http://ichatz.me/Site/InternetOfThings2020), part of the MSc on Computer Engineering, Department of Computer, Control and Management Engineering (DIAG), Sapienza University of Rome.


- [YouTube](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- [Medium article](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7)






