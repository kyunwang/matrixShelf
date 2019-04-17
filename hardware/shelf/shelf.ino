#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SPI.h>

// Shelf text and state management
String shelfText[5] = {
    "175",
    "Available",
    "Unavailable",
    "Try shoes on?",
    "Arriving shortly"};

bool isIdle = true;
int currentStep = 0;  // 0: IDLE, 1: Available, 2: Unavailable, 3: Try shoes on, 4: Please wait

// Matrix images
const byte IMAGES[][8] = {{B00000010,  // Checkmark
                           B00000001, B00000010, B00000100, B00001000, B00010000, B00000000, B00000000},
                          {B00010001,  // X - Cross
                           B00001010, B00000100, B00001010, B00010001, B00000000, B00000000, B00000000},
                          // Euro sign
                          {B00011100, B00110110, B01010101, B01010101, B01000001, B00100010, B00000000, B00000000}};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

// Button
const int buttonPin1 = D2;  // D2 & PWD
int buttonState1;           // the current reading from the input pin
int lastButtonState1;       // the previous reading from the input pin
const int buttonPin2 = D3;  // D3
int buttonState2;           // the current reading from the input pin
int lastButtonState2;       // the previous reading from the input pin

// LDR
const int ldrPin = A0;

// Debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// LED Matrix Max7219 - NodeMCU Amica
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

    for (int i = 0; i < 5; i++) Serial.println(shelfText[i]);
}

void loop() {
    int testState1 = handleButtonPress1();
    // int testState2 = handleButtonPress2();
    // Serial.println(testState1);
    // Serial.print(" - ");
    // Serial.print(digitalRead(buttonPin1));
    // Serial.println();
    // int testState = handleLdr();
    // Serial.println(testState);
    // drawMatrix();
    handleMatrix();
}

//
// Setup functions
//
void setupPinModes() {
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(ldrPin, INPUT);
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

    // matrix.setTextSize(2);
}

//
// Loop functions
//
int handleLdr() {
    int ldrValue = analogRead(ldrPin);
    return ldrValue;
}

int handleButtonPress1() {
    int currentButtonState1 = digitalRead(buttonPin1);

    if (currentButtonState1 != lastButtonState1) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (currentButtonState1 != buttonState1) {
            buttonState1 = currentButtonState1;

            Serial.print(currentButtonState1);
            Serial.print(" - ");
            Serial.print(buttonState1);
            Serial.println();

            if (buttonState1 == LOW) {
                if (currentStep == 4) {
                    currentStep = 0;
                } else {
                    currentStep++;
                }
            } else {
            }

            // lastButtonState1 = currentButtonState1;
            // return buttonState1;
        }
    }

    lastButtonState1 = currentButtonState1;
    return buttonState1;
}
int handleButtonPress2() {
    int currentButtonState2 = digitalRead(buttonPin2);

    if (currentButtonState2 != lastButtonState2) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Serial.print(currentButtonState2);
        // Serial.print(" - ");
        // Serial.print(buttonState2);
        // Serial.println();

        if (currentButtonState2 != buttonState2) {
            buttonState2 = currentButtonState2;

            if (buttonState2 == LOW) {
            } else {
            }

            return buttonState2;
        }
    }

    lastButtonState2 = currentButtonState2;
}

void drawMatrix(bool shouldRefresh = true) {
    if (shouldRefresh) {
        matrix.fillScreen(LOW);
    }

    // displayImage(IMAGES[0], 15, 1);  // Cross
    // displayImage(IMAGES[1], 7, 1);   // Check mark
    // if (isPickedUpLdr > 750) {
    // displayImage(IMAGES[2], matrix.width(), 0);  // Euro sign

    // } else {
    // for (int i = 0; i < shelfText[step].length(); i++) {
    //     matrix.drawChar(i * (width) + 1, 0, shelfText[step][i], HIGH, LOW, 1);
    // }
    String string = shelfText[currentStep];

    for (int i = 0; i < string.length(); i++) {
        matrix.drawChar((i + 1) * (width) + 1, 1, string[i], HIGH, LOW, 1);
    }

    // for (int i = 0; i < printWord.length(); i++) {
    //     matrix.drawChar((i + 1) * (width) + 1, 1, printWord[i], HIGH, LOW, 1);
    // }
    // }

    matrix.write();
}

void handleMatrix() {
    switch (currentStep) {
        case 0:  // IDLE
            matrix.fillScreen(LOW);
            displayImage(IMAGES[2], matrix.width(), 0);  // Euro sign
            drawMatrix(false);
            break;
        case 1:  // Available
            drawMatrix();
            break;
        case 2:  // Unavailable shoes on?
            drawMatrix();
            break;
        case 3:  // Try shoes on?
            drawMatrix();
            break;
        case 4:  // Please wait
            drawMatrix();
            break;
        default:
            break;
    }
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
