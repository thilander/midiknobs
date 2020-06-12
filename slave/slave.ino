#include <Wire.h>

byte lastRequestedPotPin = 0;
int potPins[10] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
 
void setup() {
  Wire.begin(1);
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);
}
 
void loop(){
  delay(100);
}
 
void recieveEvent(int howMany) {
  lastRequestedPotPin = Wire.read();
}
 
void requestEvent() {
  int analogVal = analogRead(potPins[lastRequestedPotPin]);
  analogVal = map(analogVal, 6, 1023, 0, 127);
  Wire.write(analogVal);
}
 
