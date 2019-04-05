#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_LIS3DH.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <XBee.h>
I2CGPS myI2CGPS;
#include <SoftwareSerial.h>
#include <TinyGPS++.h> //From: https://github.com/mikalhart/TinyGPSPlus

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
TinyGPSPlus gps; //Declare gps object
XBee xbee = XBee();
SoftwareSerial softserialxbee(2, 3); // RX, TX

float pascals;
float altm;
float tempC;
float xAccel;
float yAccel;
float zAccel;
float totalAccel;
//byte incoming;
//byte gps;


void setup() {
  Serial.begin(9600);
  softserialxbee.begin(9600);
  xbee.setSerial(softserialxbee);
//  xbee.setSerial(Serial);

  lis.setRange(LIS3DH_RANGE_16_G);
  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  
  if (! lis.begin(0x18)) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("Accelerometer module found!");
  
  if(myI2CGPS.begin() == false)
  {
    Serial.println("GPS module failed to respond. Please check wiring.");
    while(1);
  }
  Serial.println("GPS module found!");
}

void loop() {

//Get pressure and altitude from barometer
    if (! baro.begin()) {
    Serial.println("Couldnt find barometer");
    return;
    }
    pascals = baro.getPressure();
    Serial.print(pascals/3377); Serial.println(" Inches (Hg)");
    altm = baro.getAltitude();
    Serial.print(altm); Serial.println(" meters");
//    tempC = baro.getTemperature();
//    Serial.print(tempC); Serial.println("*C");

//Get acceleration from accelerometer
    sensors_event_t event; 
    lis.getEvent(&event);
    xAccel=(float) event.acceleration.x;
    Serial.print("\t\tX: "); Serial.print(xAccel);
    yAccel=event.acceleration.y;
    Serial.print(" \tY: "); Serial.print(yAccel); 
    zAccel=event.acceleration.z;
    Serial.print(" \tZ: "); Serial.print(zAccel); 
    Serial.println(" m/s^2 ");
    totalAccel=sqrt(pow(xAccel, 2)+pow(yAccel, 2)+pow(zAccel, 2));
    Serial.println(totalAccel);

//    while (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
//      {
//        gps.encode(myI2CGPS.read()); //Feed the GPS parser
//      }
//    
//      if (gps.time.isUpdated()) //Check to see if new GPS info is available
//      {
//        displayInfo();
//      }
    
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

//Display new GPS info
void displayInfo()
{
  //We have new GPS data to deal with!
  Serial.println();

  if (gps.time.isValid())
  {
    Serial.print(F("Date: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());

    Serial.print((" Time: "));
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());

    Serial.println(); //Done printing time
  }
  else
  {
    Serial.println(F("Time not yet valid"));
  }

  if (gps.location.isValid())
  {
    Serial.print("Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.println(F("Location not yet valid"));
  }
}
