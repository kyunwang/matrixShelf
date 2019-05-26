#include <ESP8266WiFi.h>
#include <SocketIOClient.h>

SocketIOClient client;

const char* ssid = "ssid";
const char* password = "pw";

char host[] = "192.168.1.12";
int port = 4000;

// Event - Key - Value from socket server
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long lastReply = 0;
unsigned long lastSend = 0;

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
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

    // Connect with sockets
    if (!client.connect(host, port)) {
        Serial.println("Connection failed");
        return;
    }
    if (client.connected()) {
        client.send("connection", "message", "Connected !!!!");
    }
}

void loop() {
    // client.send("event", "key", "value");
		delay(2000);

    if (client.monitor()) {
        Serial.print("RID: ");
        Serial.print(RID);
        Serial.print(", Rname: ");
        Serial.print(Rname);
        Serial.print(", Rcontent: ");
        Serial.println(Rcontent + " .");

        lastReply = millis();

        if (RID == "setSize" && Rname == "message") {
            Serial.print("Got size: ");
            Serial.println(Rcontent);

						client.send("message", "message", "message");
        }
    } else if (!client.connected()) {
        client.connect(host, port);
    }
}