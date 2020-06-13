#include <Wire.h>
#include "MIDIUSB.h"

#define nbrSlavePots 1
#define nbrLocalPots 0

int potPins[10] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
// list of undefined MIDI CC 
// https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2
int undefinedCCList[39] = {3, 9, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 85, 86, 87, 89, 90, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119}; 
int prevPotValues[20] = {0};
int currentPotValues[20] = {0};

void setup() {
  Wire.begin();
}

void sendMidiCC (byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
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
      sendMidiCC((uint8_t) 1, (uint8_t)undefinedCCList[i], (uint8_t)currentPotValues[i]);
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
 
