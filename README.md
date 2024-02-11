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
10. It is not recommended to use work(commercial) devices
## Libraries and dependencies
1. PubSubClient
## Possible issues
1. Sometimes you may see rc-2 error. You can try to establish your mqtt broker using mobile hotspot.
2. Zookeper and Kafka strongly tied by its versions. Please double-check version compatibility.
3. Double-check all the IP's you declared. Sometimes IP may be differ from original thus nothing will work.
4. Inspect actual IP addresses via Docker.
5. Be sure that your firewall allows to connect devices within private local network.
6. Ensure that you chose mqtt protocol v5.0.
## Infrastructure
1. MQTT broker (Mosquitto)
2. Apache Kafka
3. Apache Zookeper
4. Apache NiFi
5. Apache Nifi Register
6. Apache Spark
7. Apache Hadoop(HDFS)
## Monitoring links
1. [NiFi Registry](http://localhost:18080/nifi-registry/)
2. [NiFi](http://localhost:8091/nifi/)
3. [Kafka UI](http://localhost:9000/)
4. [Spark UI](http://localhost:8083/)
## NiFi dataflow overview
1. NiFi ingests data from mqtt broker 
## Useful links
1. https://medium.com/geekculture/host-a-fully-persisted-apache-nifi-service-with-docker-ffaa6a5f54a3