#include <Wire.h>
#define nbrSlavePots 1
#define nbrLocalPots 0

int potPins[10] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int prevPotValues[20] = {0};
int currentPotValues[20] = {0};

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void getSlavePotValues() {
  Wire.beginTransmission(1);
  Wire.write(1);
  delay(10);
  Wire.endTransmission();
  int available = Wire.requestFrom(1, nbrSlavePots);
  if (available == nbrSlavePots) {
    for (int i = 0; i < nbrSlavePots; i++) {
      currentPotValues[i+nbrLocalPots] =(int) Wire.read();
    }
  }
}

void getLocalPotValues() {
  for (int i = 0; i < nbrLocalPots; i++) {
    int analogVal = analogRead(potPins[i]);
    analogVal = map(analogVal, 6, 1023, 0, 127);
    currentPotValues[i] = analogVal;
  }
}

void sendMidiForChanges() {
  for (int i = 0; i < nbrLocalPots + nbrSlavePots; i++) {
    if (currentPotValues[i] != prevPotValues[i]) {
      // Send midi
      Serial.print("pot: ");
      Serial.print(i);
      Serial.print(", value: ");
      Serial.println(currentPotValues[i]);
    }
    prevPotValues[i] = currentPotValues[i];
  }
}
 
void loop() {
  getLocalPotValues();
  getSlavePotValues();
  sendMidiForChanges();
  delay(1);  
}
 
