// const int buttonPin = 4;  // D1
//// NodeMCU
const int buttonPin = 5;  // D2 & PWD
const int ledPin = LED_BUILTIN;
//// Uno
// const int buttonPin = 4;
// const int ledPin = 12;

int buttonState;      // the current reading from the input pin
int lastButtonState;  // the previous reading from the input pin
int ledState = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
    Serial.begin(115200);

    // Initialize pins
    pinMode(buttonPin, INPUT_PULLUP);  // Inconsistent if not PULLUP
    pinMode(ledPin, OUTPUT);
}

void loop() {
    int currentButtonState = digitalRead(buttonPin);

    // If the switch changed, due to noise or pressing:
    // reset the debouncing timer
    if (currentButtonState != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        Serial.print(currentButtonState);
        Serial.print(" - ");
        Serial.print(buttonState);
        Serial.println();

        // if the button state has changed:
        if (currentButtonState != buttonState) {
            buttonState = currentButtonState;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == LOW) {
                ledState = LOW;
            } else {
                ledState = HIGH;
            }
        }
    }

    // set the LED:
    digitalWrite(ledPin, ledState);

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = currentButtonState;
}
