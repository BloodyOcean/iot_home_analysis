#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>    /*  Temperature sensor DHT Library  */
#include <time.h>   /*  Time setting library  */

#define DHTPIN 5  /*  Arduino pin for DHT sensorn   */
#define DHTTYPE DHT11   /*  DHT sensor model  */

// Update these with values suitable for your network.

const char* ssid = "iPhone (Eduard)";
const char* password = "borlo123";
IPAddress mqtt_server(172, 20, 10, 3);

struct Metrics {
  float  humidity;
  float  celsius;
  float  fahrenheit;
  float  heatIndexCels;
  float  heatIndexFahr;
};

char timestamp_buff[20]; // declare a character variable that contains time-stamp

DHT dht(DHTPIN, DHTTYPE);   /*  DHT sensor client  */

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

const int timezone = 3;  /*  Timezone (Ukraine)   */

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  dht.begin();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  configureTime();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {
  String payload;
  
  time_t tnow = time(nullptr);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  struct Metrics *metric = readSensor();

  printResults(metric);
  delay(1000);
  
  if(metric == NULL) return;
  String datetime = buildDateTime(tnow);
  payload = createPayload(datetime, tnow, metric);

  unsigned long now = millis();
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", payload.c_str());
  }
}

void configureTime(){
  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("\nWaiting for time");
  
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Time configured.");
  Serial.println("");
  
}

struct Metrics * readSensor(){

  struct Metrics *metric = (Metrics*)malloc(sizeof(struct Metrics));

  metric->humidity = dht.readHumidity();
  metric->celsius = dht.readTemperature();  
  metric->fahrenheit = dht.readTemperature(true);
  
  if (isnan(metric->humidity) || isnan(metric->celsius) || isnan(metric->fahrenheit)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(1000); 
    return NULL;
  }

  metric->heatIndexFahr = dht.computeHeatIndex(metric->fahrenheit, metric->humidity);
  metric->heatIndexCels = dht.computeHeatIndex(metric->celsius, metric->humidity, false);

  return metric;
  
}

String createPayload(String datetime, time_t now, Metrics* metric){
  
  String payload = "{\"microseconds\":";
  payload += "\"" + (String)micros() + "\"";
  payload += ",\"fetched\":";
  payload += "\"" + datetime + "\"";
  payload += ",\"timestamp\":";
  payload += "\"" + (String)now + "\"";
  payload += ",\"celcius\":";
  payload += "\"" + (String)metric->celsius + "\"";
  payload += ",\"fahrenheit\":";
  payload += "\"" + (String)metric->fahrenheit + "\"";
  payload += ",\"humidity\":";
  payload += "\"" + (String)metric->humidity + "\"";
  payload += ",\"ind_cal_cel\":";
  payload += "\"" + (String)metric->heatIndexCels + "\"";
  payload += ",\"ind_cal_far\":";
  payload += "\"" + (String)metric->heatIndexFahr + "\"";
  payload += "}";

  return payload;
  
}


String buildDateTime(time_t now){
  struct tm * timeinfo;
  timeinfo = localtime(&now);
  String c_year = (String) (timeinfo->tm_year + 1900);
  String c_month = "";
  if (timeinfo->tm_mon + 1 >= 10) {
    String c_month = (String) (timeinfo->tm_mon + 1);
  } else {
    String c_month = (String) (timeinfo->tm_mon + 1);
  }
//  String c_month = (String) (timeinfo->tm_mon + 1);
  String c_day = (String) (timeinfo->tm_mday);
  String c_hour = (String) (timeinfo->tm_hour);
  String c_min = (String) (timeinfo->tm_min);
  String c_sec = (String) (timeinfo->tm_sec);
  String datetime = c_year + "-" + c_month + "-" + c_day + "T" + c_hour + ":" + c_min + ":" + c_sec;
  
  if (timezone < 0){
    datetime += "-0" + (String)(timezone) + ":00";
  }else{
    datetime += "+0" + (String)(timezone) + ":00";
  }
//  return datetime;

  sprintf (timestamp_buff, "%4d-%02d-%02d %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  String str_datetime(timestamp_buff);

  return str_datetime;
}

void printResults(Metrics* metric){
  Serial.println("Humidity: " + (String)metric->humidity);
  Serial.println("Temperature: " + (String)metric->celsius + " *C " + (String)metric->fahrenheit +" *F\t");
  Serial.println("Heat index: " + (String)metric->heatIndexCels + " *C " + (String)metric->heatIndexFahr + " *F");  
}
