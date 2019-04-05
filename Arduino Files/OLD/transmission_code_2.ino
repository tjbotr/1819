#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
#include <Adafruit_Sensor.h>
#include <SD.h>
I2CGPS myI2CGPS;
//File myFile;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  XBee.begin(9600);
  //myFile = SD.open("test.txt", FILE_WRITE); 
}
  byte pascals;
  byte altm;
  byte tempC;
  byte xVal;
  byte yVal;
  byte zVal;
  byte xAccel;
  byte yAccel;
  byte zAccel;
  byte incoming;
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

  byte dataToSend[] = {pascals, altm, tempC, xVal, yVal, zVal, xAccel, yAccel, zAccel};
  //Xbee.write(dataToSend);

  /**
  //myFile.print("Pressure: "); myFile.println(pascals);
  Xbee.write("Pressure: "); Xbee.write(pascals);
  //myFile.print("Altitude: "); myFile.println(altm);
  Xbee.write("Altitude: "); Xbee.write(altm);
  //myFile.print("Temperature: "); myFile.println(tempC);
  Xbee.write("Temperature: "); Xbee.write(tempC);
  //myFile.print("X position: ") myFile.println(xVal);
  Xbee.write("X position: "); Xbee.write(xVal);
  //myFile.print("Y position: ") myFile.println(yVal);
  Xbee.write("Y position: "); Xbee.write(yVal);
  //myFile.print("Z position: ") myFile.println(zVal);
  Xbee.write("Z position: "); Xbee.write(zVal);
  //myFile.print("X acceleration: ") myFile.println(xAccel);
  Xbee.write("X acceleration: "); Xbee.write(xAccel);
  //myFile.print("Y acceleration: ") myFile.println(YAccel);
  Xbee.write("Y acceleration: "); Xbee.write(yAccel);
  //myFile.print("Z acceleration: ") myFile.println(zAccel);
  Xbee.write("Z acceleration: "); Xbee.write(zAccel);
  //myFile.print("Coordinates ") myFile.println(incoming);
  Xbee.write("Coordinates "); Xbee.write(incoming);
  **/
   
  delay(1000);  
}
