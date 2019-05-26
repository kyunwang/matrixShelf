#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "WebSocketClient.h"

WebSocketClient ws(true);

void setup() {
    Serial.begin(115200);
    WiFi.begin("ssid", "pw");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

void loop() {
    if (!ws.isConnected()) {
        Serial.println('Not connected');
        Serial.println('Trying');
        // ws.connect("192.168.1.12", 4000, "/");
        ws.connect("ws://192.168.1.12", "/", 4000);
    } else {
        Serial.println('Connected');
        ws.send("hello");

        String msg;
        if (ws.getMessage(msg)) {
            Serial.println(msg);
        }
    }
    delay(500);
}