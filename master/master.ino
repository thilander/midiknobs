#include <Wire.h>
#define nbrSlavePots 2

void setup() {
  Wire.begin();
  Serial.begin(9600);
}
 
void loop() {
  for (int i = 0; i < nbrSlavePots; i++) {
    Wire.beginTransmission(1);
    Wire.write(i);
    delay(10);
    Wire.endTransmission();
    int available = Wire.requestFrom(1, 1);
    if(available == 1) {
      int value = Wire.read();
      Serial.print("Slave pot: ");
      Serial.print(i);
      Serial.print(", value: ");
      Serial.println(value);
    }
    delay (10);
  }
  delay(1);
}
 
