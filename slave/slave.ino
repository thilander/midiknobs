#include <Wire.h>

#define nbrButtons 5

byte lastCommand = 0;
int potPins[10] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};

int buttonPins[5] = {4, 5, 6, 7, 8};
int prevButtonPinStates[5] = {HIGH};
int buttonPinStates[5] = {HIGH};
 
void setup() {
  Wire.begin(1);
  for (int i = 0; i < nbrButtons; i++) {
     pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);
}

void getLocalButtonPinValues() {
  for (int i = 0; i < 5; i++) {
    buttonPinStates[i] = digitalRead(buttonPins[i]);
    if (buttonPinStates[i] != prevButtonPinStates[i]) {
      prevButtonPinStates[i] = buttonPinStates[i];      
      delay(20);
    }
  }
}

void loop(){
  getLocalButtonPinValues();
  delay(100);
}
 
void recieveEvent(int howMany) {
  lastCommand = Wire.read();
}
 
void requestEvent() {
  if (lastCommand == 1) {
    byte potValues[10] = {0};
    for (int i = 0; i < 10; i++) {
      int analogVal = analogRead(potPins[i]);
      analogVal = map(analogVal, 6, 1023, 0, 127);
      potValues[i] = (byte)analogVal;
    }
    Wire.write(potValues, 10);
  } else if (lastCommand == 2) {
    byte sendButtonValues[nbrButtons] = {LOW};
    for (int i = 0; i < nbrButtons; i++) {
      sendButtonValues[i] = (byte)prevButtonPinStates[i];
    }
    Wire.write(sendButtonValues, nbrButtons);
  }
  lastCommand = 0;
}
 
