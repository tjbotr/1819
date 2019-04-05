#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_LIS3DH.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <XBee.h>
I2CGPS myI2CGPS;
#include <SoftwareSerial.h>

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
XBee xbee = XBee();

unsigned long start = millis();

float pascals;
float altm;
float tempC;
unsigned int xVal;
unsigned int yVal;
unsigned int zVal;
float xAccel;
float yAccel;
float zAccel;
byte incoming;
byte gps;


void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

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
    Serial.print("Not Normalized Acceleration: "); Serial.print(xVal); Serial.print(" "); Serial.print(yVal); Serial.print(" "); Serial.println(zVal);
    sensors_event_t event; 
    lis.getEvent(&event);

    xAccel = event.acceleration.x;
    yAccel = event.acceleration.y;
    zAccel = event.acceleration.z;
    Serial.print("Acceleration: "); Serial.print(xAccel); Serial.print(" "); Serial.print(yAccel); Serial.print(" "); Serial.println(zAccel);

    incoming = myI2CGPS.read();
    Serial.println(incoming);
    
//    if (millis() - start > 15000) {
//      // break down 10-bit reading into two bytes and place in payload
//      /**
//      pin5 = analogRead(5);
//      payload[0] = pin5 >> 8 & 0xff;
//      payload[1] = pin5 & 0xff;
//      **/
//      payload[0] = (byte)pascals;
//      payload[1] = (byte)altm;
//      payload[2] = (byte)tempC;
//      payload[3] = (byte)xVal;
//      payload[4] = (byte)yVal;
//      payload[5] = (byte)zVal;
//      payload[6] = (byte)xAccel;
//      payload[7] = (byte)yAccel;
//      payload[8] = (byte)zAccel;
//      payload[9] = incoming;
//      
//      xbee.send(tx);
//
//      // flash TX indicator
//      //---flashLed(statusLed, 1, 100);
//    }
    
    delay(1000);
}
