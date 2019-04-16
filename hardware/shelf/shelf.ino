#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SPI.h>

const byte IMAGES[][8] = {
    {B00000010,  // Checkmark
     B00000001,
     B00000010,
     B00000100,
     B00001000,
     B00010000,
     B00000000,
     B00000000},
    {B00010001,  // X - Cross
     B00001010,
     B00000100,
     B00001010,
     B00010001,
     B00000000,
     B00000000,
     B00000000},
    // Euro sign
    {B00011100,
     B00110110,
     B01010101,
     B01010101,
     B01000001,
     B00100010,
     B00000000,
     B00000000}};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

const int buttonPin = D2;  // D2 & PWD
int buttonState;           // the current reading from the input pin
int lastButtonState;       // the previous reading from the input pin

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// NodeMCU Amica
// CS: D4 - DIN: D7(MOSI) - CLK: D5(SCK)
int pinCS = D4;  // SPI
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String printWord = "Arduino";

int spacer = 1;
int width = 5 + spacer;  // The font width is 5 pixels

void setup() {
    Serial.begin(115200);
    setupMatrix();
    setupPinModes();
}

void loop() {
    int testState = handleButtonPress();
    drawMatrix(testState);
}

//
// Setup functions
//
void setupPinModes() {
    pinMode(buttonPin, INPUT_PULLUP);
    // pinMode(ledPin, OUTPUT);
}

void setupMatrix() {
    matrix.setIntensity(7);  // Use a value between 0 and 15 for brightness

    matrix.setRotation(0, 1);  // The first display is position upside down
    matrix.setRotation(1, 1);
    matrix.setRotation(2, 1);
    matrix.setRotation(3, 1);
    matrix.setRotation(4, 1);
    matrix.setRotation(5, 1);
    matrix.setRotation(6, 1);
    matrix.setRotation(7, 1);
    matrix.setRotation(8, 1);
    matrix.setRotation(9, 1);
    matrix.setRotation(10, 1);
    matrix.setRotation(11, 1);
}

//
// Loop functions
//
int handleButtonPress() {
    int currentButtonState = digitalRead(buttonPin);

    if (currentButtonState != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        Serial.print(currentButtonState);
        Serial.print(" - ");
        Serial.print(buttonState);
        Serial.println();

        if (currentButtonState != buttonState) {
            buttonState = currentButtonState;

            if (buttonState == LOW) {
            } else {
            }

            return buttonState;
        }
    }

    lastButtonState = currentButtonState;
}

void drawMatrix(int stateOfButton) {
    matrix.fillScreen(LOW);

    displayImage(IMAGES[0], 15, 1);  // Cross
    displayImage(IMAGES[1], 7, 1);   // Check mark
    if (stateOfButton) {
        displayImage(IMAGES[2], matrix.width(), 0);  // Euro sign

    } else {
        for (int i = 0; i < printWord.length(); i++) {
            matrix.drawChar((i + 1) * (width) + 1, 1, printWord[i], HIGH, LOW, 1);
        }
    }
    matrix.write();
}

//
// Other functions
//
void displayImage(const byte* image, int decrementX, int decrementY) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int newX = i + matrix.width() - decrementX;
            int newY = j - decrementY;
            matrix.drawPixel(newX, newY, bitRead(image[i], 7 - j));
        }
    }
}
