#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
I2CGPS myI2CGPS;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}
  float pascals;
  float altitude;
  float altm;
  float tempC;
  float xVal;
  float yVal;
  float zVal;
  float xAccel;
  float yAccel;
  float zAccel;
  byte gps;
void loop() {
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
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
  delay(1000);
  
}
