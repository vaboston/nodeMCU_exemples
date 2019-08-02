
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h>
#include <Arduino.h>
#include <WebSocketsClient.h>

#define INFLUXDB_HOST "192.168.1.14"
#define WIFI_SSID ""
#define WIFI_PASS ""

const String host = "hooks.slack.com";
const String slack_hook_url = "https://hooks.slack.com/services/xxxxx";
const String slack_message = "coucou";
const String slack_username = "vabo";


int ledPin = 0;
int sensorPin = 4;
int pirState = LOW;
int val = 0;

ESP8266WiFiMulti WiFiMulti;
Influxdb influx(INFLUXDB_HOST);




void setup() {
  Serial.begin(115200);
  Serial.println(" ### Hello ###");

 pinMode(ledPin, OUTPUT);
 pinMode(sensorPin, INPUT);


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
  //



  val = digitalRead(sensorPin);
  if (val == HIGH){
    BearSSL::WiFiClientSecure client;
    const int API_TIMEOUT = 10000;  //keep it long if you want to receive headers from client


client.setInsecure();
client.setTimeout(API_TIMEOUT);
    if (client.connect(host, 443))
     {

       Serial.println("Connected to Slack");

       char PostData[] = "{\"text\": \"Detection de personne\"}"; // your JSON payload

       Serial.println(PostData);

       Serial.println("Connected to Slack");
       client.println("POST /services/TLSTYB7CL/BLSU1HE92/Gewwt7JrviXnMZCKgtal7qF4 HTTP/1.1");
       client.println("Host: hooks.slack.com"); //
       client.println("User-Agent: Arduino/unowifi");
       client.println("Content-Type: application/json");
       client.println("Connection: close");
       client.print("Content-Length: ");
       client.println(strlen(PostData));// number of bytes in the payload
       client.println();// important need an empty line here
       client.println(PostData);// the payload
       client.println();// important need an empty line here


   }
    digitalWrite(ledPin, HIGH);
    Serial.println("vue");
    InfluxData row("detect");
    row.addTag("device", "alpha");
    row.addTag("sensor", "one");
    row.addTag("mode", "pwm");
    row.addValue("value", 1);
    influx.write(row);
    delay(300);
  } else {
    digitalWrite(ledPin, LOW);
    delay(300);
  }


}

