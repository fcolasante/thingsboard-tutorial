# How to connect an IoT device to Thingsboard using LoRaWAN, The ThingsNewtwork and RIOT-OS
- Check the previous guides: tutorial [1](README.md), tutorial [2](RIOT.md)

- Watch my video on [YouTube](https://youtu.be/mMJN9AqVCzI)
- Read my [blog-post](https://medium.com/@colasante.francesco/3-how-to-connect-an-iot-device-to-thingsboard-using-lorawan-the-thingsnetwork-and-riot-os-46ce20dc7d5b?sk=a74c2984c610f701e5035f5acff2643b) on Medium


## Architecture
![IoT architecture with MQTT-SN](https://cdn-images-1.medium.com/max/1200/1*c4_iS8ATS3ksWUJBl99i7g.png)

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
*If you have not done the previous tutorials, I suggest you do it.*

In this tutorial, you will use this 2 folders:
- `device/riot/sensors_lora`
- `brokers/TTN_BT_Gateway`

The other folders are useful for previous tutorials.
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

## Setup Thingsboard
1. Access to Thingsboard and create a new Gateway. 
2. Keep note of the `ACCESS_TOKEN`. You will use it later.

![gateway](https://miro.medium.com/max/756/1*tvjyHLgLsF_Hb6-o4nbuCQ.png)

## Test on IoT-LAB

1. Access to `saclay`: `ssh <username>@saclay.iot-lab.info`
2. Clone RIOT repository (already done see Tutorial 2)
3. Setup `gcc compiler`
```s
cd ~/RIOT
source /opt/riot.source
export PATH=/opt/gcc-arm-none-eabi-7-2017-q4-major/bin:$PATH
arm-none-eabi-gcc --version 
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

## Run Transparent Gateway
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

![Dashboard](https://miro.medium.com/max/2000/1*ExLTY1bL1dLXUDPk9GLO8A.png)
## Helps
If you have some problem, I suggest you to do this tutorial on [IoT-LAB](https://www.iot-lab.info/tutorials/riot-ttn/)


# Details

Tutorial of the [Internet of Things course 2020](http://ichatz.me/Site/InternetOfThings2020), part of the MSc on Computer Engineering, Department of Computer, Control and Management Engineering (DIAG), Sapienza University of Rome.


- [YouTube](https://youtu.be/mMJN9AqVCzI)
- [Medium article](https://medium.com/@colasante.francesco/3-how-to-connect-an-iot-device-to-thingsboard-using-lorawan-the-thingsnetwork-and-riot-os-46ce20dc7d5b?sk=a74c2984c610f701e5035f5acff2643b)






