#include <Adafruit_Thermal.h>
#include "SoftwareSerial.h"

// Maximum width of monochrome 1-bit images = 384 pixels
// Source image does not need to be in BMP format

// Mini Thermal Printer Data Cable
// Black = Ground
// Yellow = Data IN to the printer
// Green = Data OUT of the printer


#define resetBtnPin 11
#define printBtnPin 12
#define upSwitchPin 10
#define downSwitchPin 9
#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer

int resetBtnState = 0;
int printBtnState = 0;
int upSwitchState = 0;
int downSwitchState = 0;

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

int incomingByte = 0;   // for incoming serial data


void setup() {
  // This line is for compatibility with the Adafruit IotP project pack,
  // which uses pin 7 as a spare grounding point.  You only need this if
  // wired up the same way (w/3-pin header into pins 5/6/7):
  pinMode(7, OUTPUT); digitalWrite(7, LOW);

  // put your setup code here, to run once:
  pinMode(resetBtnPin, INPUT_PULLUP);
  pinMode(printBtnPin, INPUT_PULLUP);
  pinMode(upSwitchPin, INPUT);
  pinMode(downSwitchPin, INPUT);
  Serial.begin(19200); // USB Serial
  mySerial.begin(19200); // Printer Serial
  printer.begin();
  if (Serial.available() > 0) {
    String l = Serial.readString();
    Serial.flush();
    printer.feed(3);
  }
  printer.feed(1);
  
  //printer.boldOn();
  //printer.println(F("Bold text"));
  //printer.boldOff();

  //printer.printBitmap(adalogo_width, adalogo_height, adalogo_data);

  printer.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  printer.wake();       // MUST wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
}

void loop() {
  // put your main code here, to run repeatedly:
  resetBtnState = digitalRead(resetBtnPin);
  printBtnState = digitalRead(printBtnPin);
  upSwitchState = digitalRead(upSwitchPin);
  downSwitchState = digitalRead(downSwitchPin);

  if (printBtnState == LOW) {
      Serial.println("Print Button Pressed");
      delay(1000);
  }

  if (Serial.available()) {
    printMessage();
  }
  // if (resetBtnState == LOW) {
  //   Serial.println("Reset Button Pressed");
  // }
  // if (upSwitchState == HIGH) {
  //   Serial.println("Recording Switch Activated");
  // }
  // if (downSwitchState == HIGH) {
  //   Serial.println("Recording Switch De-Activated");
  // }
}

void printMessage() {
  String input = Serial.readString();
  printer.boldOn();
  printer.setSize('L');
  printer.println(input);
  printer.boldOff();
  printer.feed(2);
  //Serial.println(input);
}
