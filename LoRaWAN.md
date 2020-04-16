# How to connect an IoT device to Thingsboard using LoRaWAN, The ThingsNewtwork and RIOT-OS
- Check the previous [guide](README.md).

- Watch my video on [YouTube](https://www.youtube.com/watch?v=MPbuNmr0FjI)
- Read my [article](https://medium.com/@colasante.francesco/2-how-to-develop-an-iot-device-connected-to-thingsboard-using-riot-os-and-mqtt-sn-c4ccbe40dae7) on Medium


## Architecture


## LoRaWAN


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

