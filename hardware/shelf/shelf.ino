#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <Max72xxPanel.h>
#include <PinButton.h>
#include <SPI.h>
#include <SocketIOClient.h>

SocketIOClient client;

////// Network stuff
// char host[] = "192.168.1.12";
// int port = 4000;
// const char* ssid = "Belcentrale2.4GHz_95791";
// const char* password = "YNQAELEF5J";

const char* ssid = "";
const char* password = "";
char host[] = "172.20.10.2";
int port = 4000;

// Event - Key - Value from socket server
extern String RID;
extern String Rname;
extern String Rcontent;

//////
// General variables
//////
int currentStep = 0;  // 0: IDLE, 1: Available, 2: Unavailable, 3: Try shoes on, 4: Please wait
int matrixLoopCount = 0;
const int ldrThreshold = 300;
String selectedSize = "43.5";

//////
// Shelf text and state management
//////
String shelfText[5] = {
    "175",
    "In store",
    "Not in store",
    "Call for shoes",  // Or: Get shoes?
                       // "Please wait... someone will come over",
    "Take a seat, someone will come over"};

//////
// Matrix images
//////
const byte IMAGES[][8] = {
    // Checkmark
    {B00000010, B00000001, B00000010, B00000100, B00001000, B00010000, B00000000, B00000000},
    // X - Cross
    {B00010001, B00001010, B00000100, B00001010, B00010001, B00000000, B00000000, B00000000},
    // Euro sign
    {B00011100, B00110110, B01010101, B01010101, B01000001, B00100010, B00000000, B00000000}};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

//////
// Button
//////
PinButton buttonPin1 = D2;  // D2 & PWD
PinButton buttonPin2 = D3;  // D3

//////
// LDR
//////
const int ldrPin = A0;

//////
// LED Matrix Max7219 - NodeMCU Amica
// CS: D4 - DIN: D7(MOSI) - CLK: D5(SCK)
//////
int pinCS = D4;  // SPI
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int spacer = 1;
int charWidth = 5 + spacer;  // The font width is 5 pixels
int wait = 20;               // 20ms

void setup() {
    // Serial.begin(115200);
    Serial.begin(230400);
    setupMatrix();
    setupPinModes();

    connectWiFi();
    connectSockets();
}

void loop() {
    buttonPin1.update();
    buttonPin2.update();

    socketMonitor();

    if (currentStep == 3) {
        handleButton();
    }

    if (currentStep == 1 || currentStep == 2 || currentStep == 3) {
        handleLdr();
    }

    assignMatrix();
}

//////
// Setup functions
//////
void setupPinModes() {
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

//////
// Loop functions
//////
int handleLdr() {
    int ldrValue = analogRead(ldrPin);
    // Serial.println(ldrValue);

    if (currentStep == 1) {
        if (ldrValue > ldrThreshold) {
            Serial.println("LDR 1");
            currentStep = 3;
        }
    }

    if (currentStep == 3) {
        if (ldrValue < ldrThreshold) {
            Serial.println("LDR 3");
            // Should check for whether it has been for 5sec and whether it had been replied on
            currentStep = 1;
            matrixLoopCount = 0;
        }
    }
    // return ldrValue;
}

void handleButton() {
    if (buttonPin1.isSingleClick()) {
        Serial.println("Click 1");
        handleButtonPress(0);
    }

    if (buttonPin2.isSingleClick()) {
        Serial.println("Click 2");
        handleButtonPress(1);
    }
}

void handleButtonPress(int buttonSide) {
    if (currentStep == 3) {
        matrixLoopCount = 0;
        if (buttonSide == 0) {  // Yes
            currentStep = 4;
        } else {  // No
            currentStep = 0;
        }
    }
}

void drawMatrix(int displacement = 0) {
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

// Source: https://github.com/markruys/arduino-Max72xxPanel/blob/master/examples/Ticker/Ticker.pde
void tickMatrix(int displacement = 0) {
    String string = shelfText[currentStep];

    int divider = 1;

    for (int i = 0; i < charWidth * string.length() + matrix.width() - 1 - spacer; i++) {
        if (isSpace(string[i])) {
            divider = divider - 3;
            continue;
        }

        matrix.fillScreen(LOW);

        int letter = i / charWidth;
        int x = (matrix.width() - 1) - i % charWidth;
        int y = (matrix.height() - 8) / 2;  // center the text vertically

        while (x + charWidth - spacer >= 0 && letter >= 0) {
            if (letter < string.length()) {
                matrix.drawChar(x, 1, string[letter], HIGH, LOW, 1);
            }

            letter--;
            x -= charWidth;
        }

        matrix.write();  // Send bitmap to display

        delay(wait);

        if (i == 0) {
            matrixLoopCount++;
            Serial.println(i);
            Serial.println(matrixLoopCount);
            if (matrixLoopCount == 1) {
                // matrix.fillScreen(LOW);

                // Quick test fix...
                if (currentStep == 3) {
                    // displayImage(IMAGES[0], 16, 1);  // Check mark
                    // displayImage(IMAGES[1], 7, 1);   // Cross
                    // drawMatrix(displacement);
                }

                return;
            }
        }
    }
}

void handleMatrix(bool shouldRefresh = true, int displacement = 0) {
    if (shouldRefresh) {
        matrix.fillScreen(LOW);
    }

    String string = shelfText[currentStep];

    if (string.length() > 12 && matrixLoopCount < 2) {
        tickMatrix(displacement);
    } else {
        drawMatrix(displacement);
    }
}

void assignMatrix() {
    switch (currentStep) {
        case 0:  // IDLE
            matrix.fillScreen(LOW);
            displayImage(IMAGES[2], matrix.width() - 1, 0);  // Euro sign
            handleMatrix(false, 1);
            break;
        case 1: {  // In store
            handleMatrix();

            int j = 0;
            for (int i = selectedSize.length(); i > 0; i--) {
                const int posX = matrix.width() - ((i) * (charWidth));

                matrix.drawChar(posX, 1, selectedSize[j], HIGH, LOW, 1);
                j++;
            }
            matrix.write();

            break;
        }
        case 2: {  // -
            handleMatrix();
            int j = 0;
            for (int i = selectedSize.length(); i > 0; i--) {
                const int posX = matrix.width() - ((i) * (charWidth));

                matrix.drawChar(posX, 1, selectedSize[j], HIGH, LOW, 1);
                j++;
            }
            matrix.write();

            break;
        }
        case 3:  // Call for shoes?
            matrix.fillScreen(LOW);
            displayImage(IMAGES[0], 16, 1);  // Check mark
            displayImage(IMAGES[1], 7, 1);   // Cross

            // if (matrixLoopCount < 3) {
            handleMatrix(false);
            // }
            break;
        case 4:  // Take a seat, someone will come over
            // matrixLoopCount = 0;

            handleMatrix();

            if (matrixLoopCount == 2) {
                delay(500);
                currentStep = 0;
            }
            // currentStep = 1;
            break;
        default:
            break;
    }
}

//////
//	Wifi & sockets
//////

//connect to Wifi
void connectWiFi() {
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);  // SETS TO STATION MODE!

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void connectSockets() {
    Serial.println("Connecting to sockets");

    // Connect with sockets
    while (!client.connect(host, port)) {
        Serial.print(",");
    }

    if (client.connected()) {
        Serial.println("");
        Serial.println("Connection succeed");
        client.send("connected", "message", "Connected !!!!");
    }
}

void socketMonitor() {
    if (client.monitor()) {
        Serial.print("RID: ");
        Serial.print(RID);
        Serial.print(", Rname: ");
        Serial.print(Rname);
        Serial.print(", Rcontent: ");
        Serial.println(Rcontent + " .");

        handleReset();

        if (currentStep == 0) {
            handleSize();
        }

    } else if (!client.connected()) {
        Serial.println("Reconnecting...");
        client.connect(host, port);
    }
}

//////
// Socket events
//////
void handleSize() {
    if (RID == "setSize") {
        if (Rname == "inStock") {
            Serial.print("Got size: ");
            Serial.println(Rcontent);

            client.send("message", "message", "in stock");

            if (currentStep == 0) {
                selectedSize = Rcontent;
                currentStep = 2;
            }
        }
    }
}

void handleReset() {
    if (RID == "reset") {
        matrixLoopCount = 0;
        currentStep = 0;

        client.send("hasReset", "key", "value");
    }
}

//////
// Other functions
//////
void displayImage(const byte* image, int decrementX, int decrementY) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int newX = i + matrix.width() - decrementX;
            int newY = j - decrementY;
            matrix.drawPixel(newX, newY, bitRead(image[i], 7 - j));
        }
    }
}
