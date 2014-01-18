#include <Xadow.h>
#include <Wire.h>
#include <LEDMatrix.h>

#define ADDRESS 0x04;

#define VOLTS_CONVERTER 310.0;
void setup () {
     
}

void loop () {
  double volts = analogRead(A5) / VOLTS_CONVERTER;
  Serial.println(volts);
  delay (50);
}
