int LDRPin = 17;  // A0

void setup() {
    Serial.begin(115200);

    pinMode(LDRPin, INPUT);
}

void loop() {
    int LDRValue = analogRead(LDRPin);
    Serial.println(LDRValue);
}
