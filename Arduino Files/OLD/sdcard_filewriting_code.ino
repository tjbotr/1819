#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
#include <Adafruit_Sensor.h>
#include <SD.h>
I2CGPS myI2CGPS;
File myFile;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myFile = SD.open("test.txt", FILE_WRITE); 
}
  float pascals;
  float altm;
  float tempC;
  float xVal;
  float yVal;
  float zVal;
  float xAccel;
  float yAccel;
  float zAccel;
  float incoming;
  byte gps;
void loop() {
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
  }
  
  pascals = baro.getPressure();
  Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

  altm = baro.getAltitude();
  Serial.print(altm); Serial.println(" meters");

  tempC = baro.getTemperature();
  Serial.print(tempC); Serial.println("*C");

  lis.read();      // get X Y and Z data at once
  xVal = lis.x;
  yVal = lis.y; 
  zVal = lis.z; 
  sensors_event_t event; 
  lis.getEvent(&event);

  xAccel = event.acceleration.x;
  yAccel = event.acceleration.y;
  zAccel = event.acceleration.z;

  incoming = myI2CGPS.read();

  myFile.print("Pressure: "); myFile.println(pascals);
  myFile.print("Altitude: "); myFile.println(altm);
  myFile.print("Temperature: "); myFile.println(tempC);
  myFile.print("X position: ") myFile.println(xVal);
  myFile.print("Y position: ") myFile.println(yVal);
  myFile.print("Z position: ") myFile.println(zVal);
  myFile.print("X acceleration: ") myFile.println(xAccel);
  myFile.print("Y acceleration: ") myFile.println(YAccel);
  myFile.print("Z acceleration: ") myFile.println(zAccel);
  myFile.print("Coordinates ") myFile.println(incoming);
   
  delay(1000);  
}
