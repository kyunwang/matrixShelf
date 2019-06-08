/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>

SocketIOClient client;
const char* ssid     = "....";
const char* password = "....";
char host[] = "192.168.1.65";
int port = 1234;

extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.connect(host, port);
  delay(500);
}

void loop() {
  if (client.connected()) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      //client.heartbeat(0);
      client.send("atime", "message", "Time please?");
      lastsend = millis();
    }
    if (client.monitor()) {
      lastreply = millis();
      if (RID == "atime" && Rname == "time") {
        Serial.print("Time is ");
        Serial.println(Rcontent);
      }
      if (RID == "arduino" && Rname == "message") {
        Serial.print("Message is ");
        Serial.println(Rcontent);
      }
    }
  } else {
    client.connect(host, port);
    if (client.connected()) {
      client.send("connection", "message", "Connected !!!!");
    } else delay(500);
  }
}

