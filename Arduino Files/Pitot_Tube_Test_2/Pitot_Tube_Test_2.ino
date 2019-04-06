//Most of this code is from turnale's comment on page 5 of this thread: https://www.rcgroups.com/forums/showthread.php?1118096-Info-for-accessing-G-Force-Airspeed-Altitude-sensors-with-your-microcontroller/page5
//A part of this code is from nakki5's comment on page 6 of this thread: https://www.rcgroups.com/forums/showthread.php?1118096-Info-for-accessing-G-Force-Airspeed-Altitude-sensors-with-your-microcontroller/page6

#include <Wire.h>

#define ALT_ADDRESS (0x75)
boolean DEBUG = true;
#define I2C_WRITE_BIT (0x00)
#define I2C_READ_BIT (0x01)

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  int altitude = read_sensor();
  if(DEBUG){
  Serial.println(altitude); delay(200);
}
}

int read_sensor(){
  int data = 0;
  Wire.beginTransmission(0x75);
  Wire.write(0x00);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x75);
  Wire.write(0x07);
  Wire.endTransmission();

  Wire.requestFrom(0x75,2); //
  if (Wire.available()) {
  data = Wire.read();
  }
  if (Wire.available()) {
  data |=Wire.read() << 8;
  }
  
  return data;
}
