// Mini Thermal Printer Data Cable
// Black = Ground
// Yellow = Data IN to the printer
// Green = Data OUT of the printer

#define resetBtnPin 11;
#define printBtnPin 12
#define upSwitchPin 10;
#define downSwitchPin 9;
#define printerInPin 6;
#define printerOutPin 5;

int resetBtnState = 0;
int printBtnState = 0;
int upSwitchState = 0;
int downSwitchState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(rSwitchPin, INPUT);
  pinMode(ySwitchPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  rSwitchState = digitalRead(rSwitchPin);
  ySwitchState = digitalRead(ySwitchPin);
  if (buttonState == HIGH) {
    Serial.println("Button Pressed");
    delay(100);
  }
  if (rSwitchState == HIGH) {
    Serial.println("R Switch Activated");
    delay(100);
  }
  if (ySwitchState == HIGH) {
    Serial.println("Y Switch Activated");
    delay(100);
  }
  delay(500);
}
