
#include <Wire.h>
#include <LEDMatrix.h>

#define ADDRESS 0x04
#define VOLTS_CONVERTER 310.0

LEDMatrix fiveBySeven = LEDMatrix(ADDRESS);
char buffer[3];

void setup () {
   fiveBySeven.setDir(DIR_NORMAL);
   Serial1.begin(9600);
   backlightOn();
   clearLCD();
}

void loop () {
  
  double volts = analogRead(A5) / VOLTS_CONVERTER;
  char* v = dtostrf(volts, 3, 2, buffer);
  strcat(v,"v");
  
  fiveBySeven.sendString(STR_ONCE, 100, v);
  selectLineOne();
  Serial1.write("Voltage");
  
  selectLineTwo();
  Serial1.write(v);
  delay (500);
  Serial.println(v);
}

void selectLineOne(){  //puts the cursor at line 0 char 0.
   Serial1.write(0xFE);   //command flag
   Serial1.write(128);    //position
   delay(10);
}
void selectLineTwo(){  //puts the cursor at line 0 char 0.
   Serial1.write(0xFE);   //command flag
   Serial1.write(192);    //position
   delay(10);
}
void goTo(int position) { //position = line 1: 0-15, line 2: 16-31, 31+ defaults back to 0
if (position<16){ Serial1.write(0xFE);   //command flag
              Serial1.write((position+128));    //position
}else if (position<32){Serial1.write(0xFE);   //command flag
              Serial1.write((position+48+128));    //position 
} else { goTo(0); }
   delay(10);
}

void clearLCD(){
   Serial1.write(0xFE);   //command flag
   Serial1.write(0x01);   //clear command.
   delay(10);
}
void backlightOn(){  //turns on the backlight
    Serial1.write(0x7C);   //command flag for backlight stuff
    Serial1.write(157);    //light level.
   delay(10);
}
void backlightOff(){  //turns off the backlight
    Serial1.write(0x7C);   //command flag for backlight stuff
    Serial1.write(128);     //light level for off.
   delay(10);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  Serial1.write(0xFE);
}

