#include <Wire.h>

byte lastCommand = 0;
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
  }
  lastCommand = 0;
}
 
