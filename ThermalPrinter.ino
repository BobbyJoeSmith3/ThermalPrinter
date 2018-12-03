#include <Adafruit_Thermal.h>
#include "SoftwareSerial.h"

// Maximum width of monochrome 1-bit images = 384 pixels
// Source image does not need to be in BMP format

// Mini printer Printer Data Cable
// Black = Ground
// Yellow = Data IN to the printer
// Green = Data OUT of the printer


#define resetBtnPin 11
#define printBtnPin 12
#define upSwitchPin 10
#define downSwitchPin 9 // Available but Not Used
#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer

int resetBtnState = 0;
int printBtnState = 0;
int upSwitchState = 0;
int prevResetBtnState = 0;
int prevPrintBtnState = 0;
int prevUpSwitchState = 0;

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

int heatTime = 80;
int heatInterval = 255;
char printDensity = 15;
char printBreakTime = 15;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


void setup() {
  // This line is for compatibility with the Adafruit IotP project pack,
  // which uses pin 7 as a spare grounding point.  You only need this if
  // wired up the same way (w/3-pin header into pins 5/6/7):
  pinMode(7, OUTPUT); digitalWrite(7, LOW);

  // put your setup code here, to run once:
  pinMode(resetBtnPin, INPUT_PULLUP);
  pinMode(printBtnPin, INPUT_PULLUP);
  pinMode(upSwitchPin, INPUT_PULLUP);
  pinMode(downSwitchPin, INPUT_PULLUP);
  Serial.begin(19200); // USB Serial
  mySerial.begin(19200); // Printer Serial
  printer.begin();

  // reserve 200 bytes for the inputString:
  inputString.reserve(2000);
  initPrinter();
}

void loop() {
  // put your main code here, to run repeatedly:
  resetBtnState = digitalRead(resetBtnPin);
  printBtnState = digitalRead(printBtnPin);
  upSwitchState = digitalRead(upSwitchPin);

  if (printBtnState == LOW) {
      Serial.println("S");
      delay(3000);
  }
  

  if (resetBtnState == LOW) {
      Serial.println('A');
      delay(1000);
  }

  if (upSwitchState != prevUpSwitchState) {
    //Serial.println("The switch changed");
    if (upSwitchState == LOW) {
      Serial.println("D");
    } else if (upSwitchState == HIGH) {
      Serial.println("W");
    }
    delay(100);
  }

  prevUpSwitchState = upSwitchState;

  // print the string when a newline arrives:
  if (stringComplete) {
    printMessage();
    // clear the string:
    inputString = "";
    stringComplete = false;
  }


}

// Called when data is available. Currently not compatible with the Leonardo
void serialEvent() {
  while (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void printMessage() {
  // Test character double-height on & off
  printer.doubleHeightOn();
  printer.println(F("Double Height ON"));
  printer.println("Double Height ON");
  printer.println(inputString);
  printer.doubleHeightOff();
  
  //printer.boldOn();
  //printer.setSize('L');
  //printer.println(inputString);
  //printer.boldOff();
  //printer.feed(2);
}

void initPrinter() {
 //Modify the print speed and heat
 printer.write(27);
 printer.write(55);
 printer.write(7); //Default 64 dots = 8*('7'+1)
 printer.write(heatTime); //Default 80 or 800us
 printer.write(heatInterval); //Default 2 or 20us
 //Modify the print density and timeout
 printer.write(18);
 printer.write(35);
 int printSetting = (printDensity<<4) | printBreakTime;
 printer.write(printSetting); //Combination of printDensity and printBreakTime
 Serial.println();
 Serial.println("Printer ready"); 
}
