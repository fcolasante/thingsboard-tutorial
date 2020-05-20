# Crowd sensing
- Check the previous guides: tutorial [1](README.md), tutorial [2](RIOT.md) 
- Watch my video on [YouTube](https://youtu.be/p3gq2VLTSiI)
- Read my [blog-post](https://medium.com/@colasante.francesco/4-how-to-create-a-crowd-sensing-application-using-generic-sensor-api-and-javascript-47e3ff1df49e) on Medium

# Architecture
![Crowd sensing](https://cdn-images-1.medium.com/max/1200/1*MmoSdsb-oU7MYuFq9tXBZA.png)

## Linear Acceleration Sensor
The LinearAccelerationSensor measures acceleration that is applied to the device hosting the sensor, excluding the contribution of a gravity force. When a device is at rest, for instance, lying flat on the table, the sensor would measure ≈ 0 m/s2 acceleration on three axes.

## Activity recognition Model
After reading several papers about Human Activity recognition (HRA) [1] [2] I decided to use to build a simple model (PoC) using the Linear acceleration sensor sampling at 1 Hz, creating a 4 sampling window and analyzing two features:
