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

int heatTime = 80;
int headInterval = 255;
char printDensity = 20;

int incomingByte = 0;   // for incoming serial data

String inputString = "";         // a String to hold incoming data
// char inputString[];         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete



// const byte numChars = 90;
// char receivedChars[numChars];
//
// boolean newData = false;


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
  // if (Serial.available() > 0) {
  //   String l = Serial.readString();
  //   Serial.flush();
  //   printer.feed(3);
  // }
  // printer.feed(1);

  printer.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  printer.wake();       // MUST wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults

  // reserve 200 bytes for the inputString:
  inputString.reserve(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  resetBtnState = digitalRead(resetBtnPin);
  printBtnState = digitalRead(printBtnPin);
  upSwitchState = digitalRead(upSwitchPin);
  downSwitchState = digitalRead(downSwitchPin);

  if (printBtnState == LOW) {
      Serial.println("X");
      delay(1000);
  }

  if (resetBtnState == LOW) {
      Serial.print("inputString =====AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      Serial.println(inputString);
      delay(1000);
  }

  // while (Serial.available() > 0) {
  //   // get the new byte:
  //   char inChar = (char)Serial.read();
  //   // add it to the inputString:
  //   inputString += inChar;
  //   // if the incoming character is a newline, set a flag so the main loop can
  //   // do something about it:
  //   delay(10);
  //   if (inChar == '\n') {
  //     stringComplete = true;
  //   }
  // }

  // print the string when a newline arrives:
  if (stringComplete) {
    // printer.boldOn();
    // printer.println(inputString);
    // printer.boldOff();
    // printer.feed(2);
    printMessage();
    //Serial.println(inputString);

    // clear the string:
    inputString = "";
    stringComplete = false;
  }


  // recvWithStartEndMarkers();
	// showNewData();

  // if (Serial.available()) {
  //   printMessage();
  // }
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

// Called when data is available. Currently not compatible with the Leonardo
// void serialEvent() {
//   while (Serial.available()) {
//     // get the new byte:
//     // String inSerial = (String)Serial.readStringUntil('\n');
//     String inSerial = Serial.readString();
//     // add it to the inputString:
//     inputString += inSerial;
//     // if the incoming character is a newline, set a flag so the main loop can
//     // do something about it:
//     if (inputString.indexOf("8") >= 0 ) {
//       stringComplete = true;
//     }
//   }
// }

void printMessage() {
  // String input = Serial.readString();
  printer.boldOn();
  // printer.setSize('L');
  printer.println(inputString);
  printer.boldOff();
  printer.feed(2);
  //Serial.println(input);
}


// void showNewData() {
// 	if (newData == true) {
// 		//Serial.print("This just in ... ");
// 		//Serial.println(receivedChars);
// 		newData = false;
// 	}
// }
//
// void recvWithStartEndMarkers() {
// 	static boolean recvInProgress = false;
// 	static byte ndx = 0;
// 	char startMarker = '<';
// 	char endMarker = '>';
// 	char rc;
//
// 	// if (Serial.available() > 0) {
// 	while (Serial.available() > 0 && newData == false) { // <<== NEW - get all bytes from buffer
// 		rc = Serial.read();
//
// 		if (recvInProgress == true) {
// 			if (rc != endMarker) {
// 				receivedChars[ndx] = rc;
// 				ndx++;
// 				if (ndx >= numChars) {
// 					ndx = numChars - 1;
// 				}
// 			}
// 			else {
// 				receivedChars[ndx] = '\0'; // terminate the string
// 				recvInProgress = false;
// 				ndx = 0;
// 				newData = true;
// 			}
// 		}
//
// 		else if (rc == startMarker) {
// 			recvInProgress = true;
// 		}
// 	}
// }
