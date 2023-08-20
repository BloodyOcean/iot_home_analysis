// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 8     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

int sensorPin = 0; //define analog pin 0
int sensorPinTemp = 1; //define analog pin 1 for thermoresistor
int value = 0;
int value_temp = 0; 


void setup() {
	Serial.begin(9600); 
}


void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  // Low values - high brightness, high values - low brightness
	value = analogRead(sensorPin);
  Serial.print("Brightness = ");
	Serial.println(value); // light intensity

  
  value_temp = analogRead(sensorPinTemp);
  
  Serial.print("Temperature from analogue = ");
	Serial.println(t);


  Serial.print("Temperature = ");
	Serial.print(t);
	Serial.print("°C | ");
	Serial.print(f);	// Convert celsius to fahrenheit
	Serial.println("°F ");
	Serial.print("Humidity = ");
	Serial.print(h);
	Serial.println("% ");
	Serial.println("");

	delay(1500); 
}