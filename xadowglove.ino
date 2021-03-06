
#include <Wire.h>
#include <LEDMatrix.h>
#include <Xadow.h>


#define ADDRESS 0x04
#define VOLTS_CONVERTER 310.0
#define TEMP_CONVERTER 6.0
#define MODE_VOLT_METER 0
#define MODE_HEAT_SENSOR 1
#define MODES_TOTAL 2

#define ADDRACC         0x53
#define Register_ID     0
#define Register_2D     0x2D
#define Register_X0     0x32
#define Register_X1     0x33
#define Register_Y0     0x34
#define Register_Y1     0x35
#define Register_Z0     0x36
#define Register_Z1     0x37

LEDMatrix led = LEDMatrix(ADDRESS);
char buffer[3];
int ctr, currentMode;

void setup () {
   led.setDir(DIR_NORMAL);
   Serial1.begin(9600);
   backlightOn();
   clearLCD();
   adxl_init();
   currentMode = 0;
   ctr = 0;
}

void loop () {
  double ax, ay, az, volts, temperature;
  char* volts_output, temp_output;
  
  readAcc(&ax, &ay, &az);
  //do something with the 3axis
  
  if (ax < -200) {
    currentMode = 0;
    clearLCD();
  }
  
  if (ay > 200) {
    currentMode = 1;
    clearLCD();
  }

  if (currentMode == 0) {
      led.sendChar('V');
      volts = analogRead(A5) / VOLTS_CONVERTER;
      volts_output = dtostrf(volts, 3, 2, buffer);
      strcat(volts_output,"v");
      selectLineOne();
      Serial1.write("Voltage");
      selectLineTwo();
      Serial1.write(volts_output);
      Serial.write(volts_output);
  }
  
  if (currentMode == 1) {
      led.sendChar('H');
      temperature = analogRead(A5) / TEMP_CONVERTER;
      Serial.println(volts);
      volts_output = dtostrf(temperature, 3, 2, buffer);
      strcat(volts_output,"f");
      selectLineOne();
      Serial1.write("Temperature");
      selectLineTwo();
      Serial1.write(volts_output);
      Serial.write(volts_output);
  }

  delay (500);
}

/* 3axis Accelerometer Convenience Methods */

void adxl_init()
{
    Wire.beginTransmission(ADDRACC);
    Wire.write(Register_2D);
    Wire.write(8);                          //measuring enable
    Wire.endTransmission();                 // stop transmitting
}

unsigned char readAcc(double *Xg, double *Yg, double *Zg)
{
    int X_out;
    int Y_out;
    int Z_out;
    Wire.beginTransmission(ADDRACC);        // transmit to device
    Wire.write(Register_X0);
    Wire.write(Register_X1);
    Wire.endTransmission();
    Wire.requestFrom(ADDRACC,2);
    if(Wire.available()<=2)
    {
        int X0 = Wire.read();
        int X1 = Wire.read();
        X1=X1<<8;
        X_out=X0+X1;
    }
    //------------------Y
    Wire.beginTransmission(ADDRACC);        // transmit to device
    Wire.write(Register_Y0);
    Wire.write(Register_Y1);
    Wire.endTransmission();
    Wire.requestFrom(ADDRACC,2);
    if(Wire.available()<=2)
    {
        int Y0 = Wire.read();
        int Y1 = Wire.read();
        Y1=Y1<<8;
        Y_out=Y0+Y1;
    }
    //------------------Z
    Wire.beginTransmission(ADDRACC);        // transmit to device
    Wire.write(Register_Z0);
    Wire.write(Register_Z1);
    Wire.endTransmission();
    Wire.requestFrom(ADDRACC,2);
    if(Wire.available()<=2)
    {
        int Z0 = Wire.read();
        int Z1 = Wire.read();
        Z1=Z1<<8;
        Z_out=Z0+Z1;
    }
    *Xg=X_out;
    *Yg=Y_out;
    *Zg=Z_out;
}

/* LED Convenience Methods */
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

