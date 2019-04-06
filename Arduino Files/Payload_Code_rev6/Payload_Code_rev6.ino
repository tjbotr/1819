#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_LIS3DH.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h> 
#include <Adafruit_Sensor.h>
//#include <SD.h>
#include <XBee.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h> //From: https://github.com/mikalhart/TinyGPSPlus
#include <Adafruit_BNO055.h>

//Initialize sensor related variables
  int packetCount=0;
  float pascals;
  float altm;
  float xAccel;
  float yAccel;
  float zAccel;
  float totalAccel;
  float xVelocity;
  float yVelocity;
  float zVelocity;
  float totalVelocity;
  float latitude;
  float longitude;
  int pitotSpeed=0;
  boolean cameraOn=false;
  boolean cameraAllowedToBeEnabled=true;
  boolean manualCameraOn=false;
  int cameraDelay=0;
  const int cameraPin = 6;
  boolean manualArmCommandOverride=false; //Set this to true to debug the payload code without a ground station

//Initialize sensor related objects
  Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
  Adafruit_LIS3DH lis = Adafruit_LIS3DH();
  I2CGPS myI2CGPS;
  TinyGPSPlus gps; //Declare gps object

//Initialize Xbee transmit related stuff
  XBee xbee = XBee();
  uint8_t payload[] = {(byte)packetCount, (byte)altm, (byte)totalAccel, (byte)totalVelocity,(byte)pitotSpeed, (byte)latitude, (byte)longitude, (byte)cameraOn, (byte)cameraAllowedToBeEnabled, (byte)cameraDelay};
  Tx16Request tx = Tx16Request(0x0, payload, sizeof(payload)); // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  TxStatusResponse txStatus = TxStatusResponse();
  SoftwareSerial softserialxbee(2, 3); // RX, TX
  unsigned long previousMillis = 0;
  unsigned long currentMillis;

//Initialize XBee receive related stuff
  XBeeResponse response = XBeeResponse();
  Rx16Response rx16 = Rx16Response();
  Rx64Response rx64 = Rx64Response();
  uint8_t data = 0;
  
  
void setup() {
  //Initialize serial connection to computer for debugging
    Serial.begin(9600);
  
  //Initialize software serial connection to XBee
    softserialxbee.begin(9600);
    xbee.setSerial(softserialxbee);
  
  //Initialize I2C bus for pitot tube
    Wire.begin();
  
  //Initialize accelerometer
    lis.setRange(LIS3DH_RANGE_16_G);
    Serial.print("Range = "); Serial.print(2 << lis.getRange()); Serial.print("G");
    if (! lis.begin(0x18)) {
      Serial.println("Couldnt start accelerometer");
      while (1);
    }
    Serial.println("  Accelerometer module found!");

  //Make sure that the camer remains off
    pinMode(cameraPin, OUTPUT);
    digitalWrite(cameraPin, HIGH);
  
  //Initialize GPS
  //  if(myI2CGPS.begin() == false)
  //  {
  //    Serial.println("GPS module failed to respond. Please check wiring.");
  //    while(1);
  //  }
  //  Serial.println("GPS module found!");

  //Wait for arm command from ground station to continue
    while(!manualArmCommandOverride)
    {
      xbee.readPacket();
      if (xbee.getResponse().isAvailable()) {
        // got something
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) 
        {
          // got a rx packet
          if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
            xbee.getResponse().getRx16Response(rx16);
            data = rx16.getData(0);
          } else {
            xbee.getResponse().getRx64Response(rx64);
            data = rx64.getData(0);
          }
        } else {
          // not something we were expecting
          Serial.println("Did not get a 16 or 64 response");    
        }
      } else if (xbee.getResponse().isError()) {
        Serial.print("Error reading packet.  Error code: ");  
        Serial.println(xbee.getResponse().getErrorCode());
      }
      
     if(data==1){
      break;
     }
    }
    
}

void loop() {
  //Get altitude from barometer
      if (! baro.begin()) {
      Serial.println("Couldnt find barometer");
      return;
      }
      pascals = baro.getPressure();
      Serial.print("Pressure: "); Serial.print(pascals/3377); Serial.println(" Inches (Hg)");
      altm = baro.getAltitude();
      Serial.print("Altitude: "); Serial.print(altm); Serial.println(" meters");
  
  //Get acceleration from accelerometer
      sensors_event_t event; 
      lis.getEvent(&event);
      xAccel=(float) event.acceleration.x;
      Serial.print("Acceleration: \tX: "); Serial.print(xAccel);
      yAccel=event.acceleration.y;
      Serial.print(" \tY: "); Serial.print(yAccel); 
      zAccel=event.acceleration.z;
      Serial.print(" \tZ: "); Serial.print(zAccel); 
      Serial.println(" m/s^2 ");
      totalAccel=sqrt(pow(xAccel, 2)+pow(yAccel, 2)+pow(zAccel, 2));
      Serial.print("Total Acceleration: "); Serial.print(totalAccel); Serial.println(" m/s^2");
    //Add totalVelocity ability using the BNO055
  
  //Get latitude and longitude from GPS
  //    while (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
  //      {
  //        gps.encode(myI2CGPS.read()); //Feed the GPS parser
  //      }
  //    
  //      if (gps.time.isUpdated()) //Check to see if new GPS info is available
  //      {
  //        displayInfo();
  //      }
  
  //Get speed from pitot tube
      Wire.beginTransmission(0x75);
      Wire.write(0x00);
      Wire.endTransmission();
      
      Wire.beginTransmission(0x75);
      Wire.write(0x07);
      Wire.endTransmission();
    
      Wire.requestFrom(0x75,2); //
      if (Wire.available()) {
      pitotSpeed = Wire.read();
      }
      if (Wire.available()) {
      pitotSpeed |= Wire.read() << 8;
      }
      Serial.print("Pitot tube speed: "); Serial.print(pitotSpeed); Serial.println("MPH");

  //Camera enable and disable code
      if(cameraAllowedToBeEnabled==true && cameraOn==false && (totalVelocity>15 || manualCameraOn==true))//Turn on camera if it is off and velocity is greater than 15m/s
      {
        digitalWrite(cameraPin, LOW);
        delay(700);
        digitalWrite(cameraPin, HIGH);
        cameraOn=true;
        cameraAllowedToBeEnabled=false;
        Serial.println("****Camera is ON!****");
      }
      else if(cameraOn==true && cameraDelay<15000)//Increment 15 second delay if it has not fully elapsed
      {
        cameraDelay+=1000;
        Serial.println("Incremented camera delay by 1 second");
      }
      else if(cameraOn==true && cameraDelay>=15000)//Turn off camera if it is on and 15 second delay has elapsed
      {
        digitalWrite(cameraPin, LOW);
        delay(700);
        digitalWrite(cameraPin, HIGH);
        cameraOn=false;
        Serial.println("****Camera is OFF!****");
      }
  
  //Transmit data using XBee
      packetCount+=1;
      currentMillis = millis();
      if (previousMillis  - currentMillis >= 1000) { //Wait until 1 second has passed to transmit
        previousMillis = currentMillis;
        //payload[] = {(byte)packetCount, (byte)altm, (byte)totalAccel, (byte)totalVelocity,(byte)pitotSpeed, (byte)latitude, (byte)longitude, (byte)cameraOn, (byte)cameraAllowedToBeEnabled, (byte)cameraDelay};
        payload[0] = (byte)packetCount;
        payload[1] = (byte)altm;
        payload[2] = (byte)totalAccel;
        payload[3] = (byte)totalVelocity;
        payload[4] = (byte)pitotSpeed;
        payload[5] = (byte)latitude;
        payload[6] = (byte)longitude;
        payload[7] = (byte)cameraOn;  
        payload[8] = (byte)cameraAllowedToBeEnabled;
        payload[9] = (byte)cameraDelay;   
        xbee.send(tx);
        Serial.println("Data has been transmitted!");
      }

   Serial.println("");
}

void displayInfo() //Method to display GPS info and update the lat and long vars
{
  //We have new GPS data to deal with!
  Serial.println();

  if (gps.time.isValid())
  {
    Serial.print(F("Date: ")); Serial.print(gps.date.month());
    Serial.print(F("/")); Serial.print(gps.date.day());
    Serial.print(F("/")); Serial.print(gps.date.year());

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
    latitude=gps.location.lat(); //,6
    Serial.print(latitude);
    Serial.print(F(", "));
    longitude=gps.location.lng();
    Serial.print(longitude);
    Serial.println();
  }
  else
  {
    Serial.println(F("Location not yet valid"));
  }

  if (gps.altitude.isValid())
  {
    Serial.print(F("Altitude Meters:"));
    Serial.print(gps.altitude.meters());
    Serial.print(F(" Feet:"));
    Serial.print(gps.altitude.feet());
  }

  if (gps.satellites.isValid())
  {
    Serial.print(F(" Satellites in View:"));
    Serial.print(gps.satellites.value());
  }

  if (gps.hdop.isValid())
  {
    Serial.print(F(" HDOP:"));
    Serial.print(gps.hdop.value()/100.0, 2); //TinyGPS reports DOPs in 100ths
  }
}
