// Based on: https://github.com/markruys/arduino-Max72xxPanel/blob/master/examples/Ticker/Ticker.pde

#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SPI.h>

String exampleText[2] = {
	"text one",
	"text two"
};

int pinCS = D4;  // SPI
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String printWord = exampleText[0];

int spacer = 1;
int width = 5 + spacer;  // The font width is 5 pixels
int currentStep = 0;

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
	const currentText = exampleText[currentStep];

	for (int i = 0; i < width * currentText.length() + matrix.width() - 1 - spacer; i++) {
		matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < currentText[currentStep].length() ) {
        matrix.drawChar(x, y, currentText[currentStep][letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(20);
	}
}

void textOne() {}
void textTwo() {}

void switchText() {
	if (currentStep) { // if 1
		printWord = exampleText[1];
		return;
	}
	
	printWord = exampleText[0];
}
