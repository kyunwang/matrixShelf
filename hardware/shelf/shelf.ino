#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <PinButton.h>
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
bool isPickedUp = false;
String selectedSize = "43.5";

// Matrix images
const byte IMAGES[][8] = {
    // Checkmark
    {B00000010, B00000001, B00000010, B00000100, B00001000, B00010000, B00000000, B00000000},
    // X - Cross
    {B00010001, B00001010, B00000100, B00001010, B00010001, B00000000, B00000000, B00000000},
    // Euro sign
    {B00011100, B00110110, B01010101, B01010101, B01000001, B00100010, B00000000, B00000000}};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

// Button
PinButton buttonPin1 = D2;  // D2 & PWD
PinButton buttonPin2 = D3;  // D3

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

int spacer = 1;
int charWidth = 5 + spacer;  // The font width is 5 pixels

void setup() {
    Serial.begin(115200);
    setupMatrix();
    setupPinModes();

    for (int i = 0; i < 5; i++) Serial.println(shelfText[i]);
}

void loop() {
    if (Serial.available()) {
        String serialRead = Serial.readStringUntil('\n');
        serialRead.trim();
        selectedSize = serialRead;
        Serial.println("You selected: " + serialRead);
        if (currentStep == 0) {
            currentStep++;
        }
    }

		buttonPin1.update();
		buttonPin2.update();

    handleButton();
    handleLdr();
    handleMatrix();
}

//
// Setup functions
//
void setupPinModes() {
    // pinMode(buttonPin1, INPUT_PULLUP);
    // pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(ldrPin, INPUT);
}

void setupMatrix() {
    matrix.setIntensity(7);  // Use a value between 0 and 15 for brightness

    matrix.setRotation(0, 1);  // Turns it 45deg clockwise
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
int handleLdr() {
    int ldrValue = analogRead(ldrPin);

    switch (currentStep) {
        case 1:  // Available
            if (ldrValue > 750) {
                // isPickedUp = true;
                currentStep = 3;
            } else {
                // Depends on availability
                currentStep = 1;
            }
            break;
        case 3:
            if (ldrValue < 750) {
                // Should check for whether it has been for 5sec and whether it had been replied on
                isPickedUp = false;
                currentStep = 1;
            }
            break;
    }

    return ldrValue;
}

void handleButton() {
	if (buttonPin1.isSingleClick()) {
		handleButtonPress(0);
	}

	if (buttonPin2.isSingleClick()) {
		handleButtonPress(1);
	}
}

void handleButtonPress(int buttonSide) {
    switch (currentStep) {
        case 0:
            currentStep++;  // Jump to availablity check
            break;
        case 3:
            if (buttonSide == 0) {  // Yes
                currentStep = 4;
            } else {  // No
                // depending on availability: 1 or 2
                // currentStep = 1;
                currentStep = 0;
            }
            break;
        // case 4:
        // break;
        default:
            break;
    }
}

void drawMatrix(bool shouldRefresh = true, int displacement = 0) {
    if (shouldRefresh) {
        matrix.fillScreen(LOW);
    }

    String string = shelfText[currentStep];

    int divider = 1;
    for (int i = 0; i < string.length(); i++) {
        if (isSpace(string[i])) {
            divider = divider - 3;
            continue;
        }

        const int posX = (i + displacement) * (charWidth) + divider;

        matrix.drawChar(posX, 1, string[i], HIGH, LOW, 1);
    }

    matrix.write();
}

void handleMatrix() {
    switch (currentStep) {
        case 0:  // IDLE
            matrix.fillScreen(LOW);
            displayImage(IMAGES[2], matrix.width() - 1, 0);  // Euro sign
            drawMatrix(false, 1);
            break;
        case 1: {  // Available
            drawMatrix();

            int j = 0;
            for (int i = selectedSize.length(); i > 0; i--) {
                const int posX = matrix.width() - ((i) * (charWidth));

                matrix.drawChar(posX, 1, selectedSize[j], HIGH, LOW, 1);
                j++;
            }
            matrix.write();

            break;
        }
        case 2:  // Unavailable shoes on?
            drawMatrix();
            break;
        case 3:  // Try shoes (on)?
            matrix.fillScreen(LOW);
            displayImage(IMAGES[0], 16, 1);  // Check mark
            displayImage(IMAGES[1], 7, 1);   // Cross
            drawMatrix(false);
            break;
        case 4:  // Please wait
            drawMatrix();
            delay(2000);
            // currentStep = 1;
            currentStep = 0;
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
