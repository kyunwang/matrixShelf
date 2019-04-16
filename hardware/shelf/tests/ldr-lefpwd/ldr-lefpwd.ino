// Source: https://github.com/JeremySCook/LDR-LEDPWM/blob/master/ArduinoCodeLDR_LED_PWM

//takes input from LDR or other analog inpu
//turns it into PWM output for LED
//by Jeremy S. Cook

int ledPin = 11;  //LED connected to digital pin 3
int PWMValue;     //Sets PWMValue as integer

void setup() {
    Serial.begin(115200);
}

void loop() {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    PWMValue = map(sensorValue, 0, 1023, 0, 255);  //maps sensor value into proportional PWM values
    Serial.println(PWMValue);                      //print PWMValue (0 to 255) to serial
    analogWrite(ledPin, PWMValue / 2);             //write PWMValue to digital pin 3
    delay(500);                                    //delay helps one see change, could be omitted
}