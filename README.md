# iot_home_analysis
## Overview
This project is created to analyse telemetry from sensors and create real-time dashboards and analytics
## Architecture overview
## How to deploy locally
1. Install docker
2. Copy the repository on your machine
3. In the code change ssid and password according yo your credentials
4. Install all the necessary libraries for project
5. Run in terminal ```ipconfig``` in order to get the IP for your MQTT broker server
6. Run ```docker-compose up```
7. Compile the code and boot your NodeMCU board
8. Check logs via docker or serial monitor
9. For better experience I recommend to use MQTTX client
## Libraries and dependencies
## Possible issues
1. Sometimes you may see rc-2 error. You can try to establish your mqtt broker using mobile hotspot.
