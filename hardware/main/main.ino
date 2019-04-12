
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Begin");

  pinMode(A0, INPUT);
}

void loop() {
  Serial.println(analogRead(A0));
  delay(10);
}
