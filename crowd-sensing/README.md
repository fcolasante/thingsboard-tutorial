# Crowd sensing
- Check the previous guides: tutorial [1](README.md), tutorial [2](RIOT.md)
- Watch my video on [YouTube](https://youtu.be/mMJN9AqVCzI)
- Read my [blog-post](https://medium.com/@colasante.francesco/3-how-to-connect-an-iot-device-to-thingsboard-using-lorawan-the-thingsnetwork-and-riot-os-46ce20dc7d5b?sk=a74c2984c610f701e5035f5acff2643b) on Medium

# Architecture
![Crowd sensing](https://cdn-images-1.medium.com/max/1200/1*MmoSdsb-oU7MYuFq9tXBZA.png)

## Linear Acceleration Sensor
The LinearAccelerationSensor measures acceleration that is applied to the device hosting the sensor, excluding the contribution of a gravity force. When a device is at rest, for instance, lying flat on the table, the sensor would measure ≈ 0 m/s2 acceleration on three axes.

## Activity recognition Model
After reading several papers about Human Activity recognition (HRA) [1] [2] I decided to use to build a simple model (PoC) using the Linear acceleration sensor sampling at 1 Hz, creating a 4 sampling window and analyzing two features:
