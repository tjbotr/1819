#include <Wire.h>

#define ALT_ADDRESS (0x75)//(0x74)
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
  Wire.beginTransmission(ALT_ADDRESS);
  Wire.write(I2C_WRITE_BIT);
  Wire.endTransmission();
  
  Wire.beginTransmission(ALT_ADDRESS);
  Wire.write(0x07);
  Wire.endTransmission();
  
  
//  Wire.requestFrom(ALT_ADDRESS,2);
//  int bitavailable = Wire.available();
//  data = Wire.read();
  Wire.requestFrom(ALT_ADDRESS,2);
  if (Wire.available()) {
  data = Wire.read();
  }
  if (Wire.available()) {
  data |=Wire.read() << 8;
  }
  
  return data;
}
