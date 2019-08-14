#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <InfluxDb.h>

#define INFLUXDB_HOST "192.168.X.X"
#define WIFI_SSID ""
#define WIFI_PASS ""
#define INFLUXDB_USER ""
#define INFLUXDB_PASS ""
#define INFLUXDB_DATABASE ""

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10


ESP8266WiFiMulti WiFiMulti;
Influxdb influx(INFLUXDB_HOST);
Adafruit_BMP280 bme; // I2C

void setup() {
 Serial.begin(9600);
 Serial.println(F("BMP280 test"));
 ESP8266WiFiMulti WiFiMulti;
 influx.setDbAuth(INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASS); // with authentication


 if (!bme.begin()) {
   Serial.println("Could not find a valid BMP280 sensor, check wiring!");
   while (1);
 }
 
 WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
 Serial.print("Connecting to WIFI");
 while (WiFiMulti.run() != WL_CONNECTED) {
   Serial.print(".");
   delay(100);
 }
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());

 influx.setDb("esp32_tst");

 Serial.println("Setup done");
}


void loop() {
   Serial.print("T=");
   Serial.print(bme.readTemperature());
   Serial.print(" *C");

   Serial.print(" P=");
   Serial.print(bme.readPressure());
   Serial.print(" Pa");

   Serial.print(" A= ");
   Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
   Serial.println(" m");
   InfluxData row("desk_temperature");
   row.addTag("device", "alpha");
   row.addTag("sensor", "temp");
   row.addValue("value", bme.readTemperature());
   influx.write(row);
   InfluxData row2("desk_pressure");
   row2.addTag("device", "alpha");
   row2.addTag("sensor", "press");
   row2.addValue("value", (bme.readPressure()));
   influx.write(row2);

   delay(5000);
}

