#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SPI.h>

const byte IMAGES[][8] = {
    {B00000010,
     B00000001,
     B00000010,
     B00000100,
     B00001000,
     B00010000,
     B00000000,
     B00000000},
    {B00010001,
     B00001010,
     B00000100,
     B00001010,
     B00010001,
     B00000000,
     B00000000,
     B00000000}};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

// NodeMCU Amica
// CS: D4 - DIN: D7(MOSI) - CLK: D5(SCK)
int pinCS = D4;  // SPI
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String printWord = "Arduino";

int spacer = 1;
int width = 5 + spacer;  // The font width is 5 pixels

// void setRotation(uint8_t rotation);
void setup() {
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

void loop() {
    displayImage(IMAGES[0], 15, 1);
    displayImage(IMAGES[1], 7, 1);
    for (int i = 0; i < printWord.length(); i++) {
        matrix.drawChar(i * (width) + 1, 1, printWord[i], HIGH, LOW, 1);
    }
    matrix.write();
}

void displayImage(const byte* image, int decrementX, int decrementY) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int newX = i + matrix.width() - decrementX;
            int newY = j - decrementY;
            matrix.drawPixel(newX, newY, bitRead(image[i], 7 - j));
        }
    }
}
