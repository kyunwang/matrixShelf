
// Can be installed from Library Manager or https://github.com/bportaluri/WiFiEsp
#define _DEBUG_
#define _DISABLE_TLS_
#include <WiFiEsp.h>
// #include <ThingerESP8266AT.h>

#define USERNAME "*****"
#define DEVICE_ID "*****"
#define DEVICE_CREDENTIAL "******"

#define SSID H369A007AEF
#define SSID_PASSWORD E7CF226AE39C
// char ssid[] = "H369A007AEF";      // your network SSID (name)
// char pass[] = "E7CF226AE39C";  // your network password

#define ARDUINO_LED 13

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

// ThingerESP8266AT thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  pinMode(ARDUINO_LED, OUTPUT);

  // serial for debug
  Serial.begin(9600);

  // initialize serial for ESP8266
  Serial1.begin(9600);

  // used to change the ESP8266 to 9600 baudrate (set with a 115200 connection)
  //Serial1.write("AT+UART_DEF=9600,8,1,0,0\r\n");

  // init ESP8266 in the additional serial port
  WiFi.init(&Serial1);
  if(WiFi.status() == WL_NO_SHIELD){
      // don't continue
      while (true);
  }

  // thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  // thing["led"] << digitalPin(ARDUINO_LED);

  // resource output example (i.e. reading a sensor value)
  // thing["millis"] >> outputValue(millis());

  // more details at http://docs.thinger.io/arduino/
}

void loop() {
  // thing.handle();
	Serial.println(WiFi.status());
}