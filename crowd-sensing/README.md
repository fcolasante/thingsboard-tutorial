# Crowd sensing
- Check the previous guides: tutorial [1](../README.md), tutorial [2](../RIOT.md), tutorial [3](../LoRaWAN.md)
- Watch my video on [YouTube](https://youtu.be/p3gq2VLTSiI)
- Read my [blog-post](https://medium.com/@colasante.francesco/4-how-to-create-a-crowd-sensing-application-using-generic-sensor-api-and-javascript-47e3ff1df49e) on Medium



# Architecture
![Crowd sensing](https://cdn-images-1.medium.com/max/1200/1*MmoSdsb-oU7MYuFq9tXBZA.png)


## Live demo
Live demo is available [here](https://fcolasante.github.io/crowd-sensing/).


## Linear Acceleration Sensor
The LinearAccelerationSensor measures acceleration that is applied to the device hosting the sensor, excluding the contribution of a gravity force. When a device is at rest, for instance, lying flat on the table, the sensor would measure ≈ 0 m/s2 acceleration on three axes.

## Activity recognition Model
After reading several papers about Human Activity recognition (HRA) [1](https://www.researchgate.net/publication/224144663_Human_Activity_Recognition_via_an_Accelerometer-Enabled-Smartphone_Using_Kernel_Discriminant_Analysis) [2](https://www.researchgate.net/publication/279852352_Improving_classification_of_sit_stand_and_lie_in_a_smartphone_human_activity_recognition_system) I decided to use to build a simple model (PoC) using the Linear acceleration sensor sampling at 1 Hz, creating a 4 sampling window and analyzing two features:
```js
let l_sma = data.map(val => val.x + val.y + val.z).reduce( (acc, val) => acc + val);
l_sma /= win;
let tmp = data.map( val => val.x + val.y + val.z - l_sma ).map( x => Math.pow(x, 2));
let sosd = tmp.reduce( (acc, val) => acc + val);
sosd /= win;
sosd = Math.sqrt(sosd);
```

## Deployment
As already mentioned in the introduction, I created two different versions:
- cloud-deployment: the page sends the raw data to Thingsboard, processes it through the HRA model and outputs the prediction.
- edge-deployment: the page processes the model locally using Javascript, from the prediction as output and sends the prediction and raw data to Thingsboard which saves them and also providing dashboards.

## Thingsboard
Thingsboard
Now it’s time to set up Thingsboard. In this tutorial I used the Thingsboard live demo, but I remember that it is also possible to user Docker or host your instance on Digital Ocean as I showed in the previous tutorials.
First of all, create 2 new devices, copy the access token and insert it in the Javascript code respectively for edge and cloud deployment.
After this, you need to edit the default rule chain. In order to compute the data only in the case of cloud deployment we have to intercept only those packages. To do this the following command helps us.

```html
    <script>
        /*REPLACE ACCESS_TOKEN with yours. 
        This is a private info and it should be not stored in Git.
         */
        let access_token = 'AAAAAMtFyBPNCDj2Kol4';
        let lad = document.getElementById('lad');
        let stat = document.getElementById('stat');
    ...
```
This is the code to send telemetry on Thingsboard:
```js
    const Http = new XMLHttpRequest();
    const url = `https://demo.thingsboard.io/api/v1/${access_token}/telemetry`;
    Http.open("POST", url);
    Http.send(msg);
```

# Notes
 I tried to test the code locally using `http-server` but the sensor was not recognized by my smartphone. I had to test the code pushing on GitHub Pages necessarily. **(It was not very easy to debug this way)**.
I created a simple `print` on HTML that replaces the classic console.log to “easily” find errors.
I have tested the application on a samsung S8.

# Details

Tutorial of the [Internet of Things course 2020](http://ichatz.me/Site/InternetOfThings2020), part of the MSc on Computer Engineering, Department of Computer, Control and Management Engineering (DIAG), Sapienza University of Rome.


- [YouTube](https://youtu.be/p3gq2VLTSiI)
- [Medium article](https://medium.com/p/4-how-to-create-a-crowd-sensing-application-using-generic-sensor-api-and-javascript-47e3ff1df49e?source=email-b3e30a097b41--writer.postDistributed&sk=67c5c558f0f9e451caeb1b293bb06767)
