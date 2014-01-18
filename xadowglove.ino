
#include <Wire.h>
#include <LEDMatrix.h>

#define ADDRESS 0x04
#define VOLTS_CONVERTER 310.0

LEDMatrix fiveBySeven = LEDMatrix(ADDRESS);
char buffer[5];


void setup () {
   fiveBySeven.setDir(DIR_NORMAL);
}

void loop () {
  
  double volts = analogRead(A5) / VOLTS_CONVERTER;
  char* v = dtostrf(volts, 5, 2, buffer);
  Serial.println(v);
  
  fiveBySeven.sendString(STR_ONCE, 100, v);
  delay (1500);
}
