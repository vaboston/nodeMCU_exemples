#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "test"
#define APPSK  "password"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
int ledPin = 0; // to pin 3 on arduin


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<a href="/on">Continue</a> <a href="/off2">Off</a>");
  Serial.println("SEND");

}

void setup() {

  pinMode(ledPin, OUTPUT);

  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  server.on("/on", [](){
    server.send(200, "text/plain", "On !");
    digitalWrite(ledPin, HIGH);
    delay(300);
  });
  server.on("/off", [](){
    server.send(200, "text/plain", "Off !");
    digitalWrite(ledPin, LOW);
    delay(300);
  });
}

void loop() {
  server.handleClient();
}
