# Thingsboard Tutorial
In this repository I will create a cloud-based IoT system that collects information from a set of virtual environmental sensors using the MQTT protocol. I will also create a simple web site to display the data collected from the sensors.

## Docker
```s
docker run -it -p 9090:9090 -p 1883:1883 -p 5683:5683/udp -v ~/.mytb-data:/data -v ~/.mytb-logs:/var/log/thingsboard --name mytb --restart always thingsboard/tb-postgres
```

Ports:
- `9090`: Admin panel
- `1883`: MQTT broker

## Account access
[link](https://thingsboard.io/docs/samples/demo-account/)

- Tenant administrator: provision customer, device, things
- Sysadmin (sysadmin)
- Tenants
- Users
- Customers

## MQTT Api

```s
https://thingsboard.io/docs/reference/mqtt-api/
```

## Mosquitto
```s
sudo apt install mosquitto mosquitto-cSlients
export ACCESS_TOKEN='A1_TEST_TOKEN'
```
Download file *(or write yourself)*
```s
mosquitto_pub -d -h "127.0.0.1" -t "v1/devices/me/telemetry" -u "$ACCESS_TOKEN" -f "telemetry-data-as-object.json"
```

Parameters:
```
-u, --username
           Provide a username to be used for authenticating with the broker. This requires a
           broker that supports MQTT v3.1. See also the --pw argument.
```


## Restart
Check if ports is free:
```
sudo netstat -tulpn | grep LISTEN
```
In my case I have been installed `mosquitto` hence I need to stop it.
```
sudo service mosquitto stop
```
otherwise I can just swap `mqtt` binding port on docker command.

https://thingsboard.io/docs/getting-started-guides/helloworld/#video-tutorial

## Digital Ocean
Get your 50$ from Education Github ad [link](https://education.github.com/pack/offers#digitalocean)

### PubIP
[Public access](http://64.227.26.128:8080)

https://thingsboard.io/docs/user-guide/install/digital-ocean/
https://www.digitalocean.com/docs/droplets/how-to/connect-with-ssh/openssh/
https://thingsboard.io/docs/user-guide/install/ubuntu/


