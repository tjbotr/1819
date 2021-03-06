/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <XBee.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include "SparkFun_I2C_GPS_Arduino_Library.h" 
#include <Adafruit_Sensor.h>
#include <SD.h>
I2CGPS myI2CGPS;
#include <SoftwareSerial.h>

/*
This example is for Series 1 XBee
Sends a TX16 or TX64 request with the value of analogRead(pin5) and checks the status response for success
Note: In my testing it took about 15 seconds for the XBee to start reporting success, so I've added a startup delay
*/

XBee xbee = XBee();

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

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

// allocate two bytes for to hold a 10-bit analog reading
uint8_t payload[] = {(byte)pascals, (byte)altm, (byte)tempC, (byte)xVal, (byte)yVal, (byte)zVal, (byte)xAccel, (byte)yAccel, (byte)zAccel, incoming, gps};

// with Series 1 you can use either 16-bit or 64-bit addressing

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0x0, payload, sizeof(payload));

// 64-bit addressing: This is the SH + SL address of remote XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4008b490);
// unless you have MY on the receiving radio set to FFFF, this will be received as a RX16 packet
//Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

//int pin5 = 0;

//int statusLed = 11;
//int errorLed = 12;

/**void flashLed(int pin, int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}**/
SoftwareSerial softserialxbee(2, 3); // RX, TX

void setup() {
  //pinMode(statusLed, OUTPUT);
  //pinMode(errorLed, OUTPUT);
  Serial.begin(9600);
  //xbee.setSerial(Serial);
  softserialxbee.begin(9600);
  xbee.setSerial(softserialxbee);
}

void loop() {
   
   // start transmitting after a startup delay.  Note: this will rollover to 0 eventually so not best way to handle
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
    if (millis() - start > 15000) {
      // break down 10-bit reading into two bytes and place in payload
      /**
      pin5 = analogRead(5);
      payload[0] = pin5 >> 8 & 0xff;
      payload[1] = pin5 & 0xff;
      **/
      payload[0] = (byte)pascals;
      payload[1] = (byte)altm;
      payload[2] = (byte)tempC;
      payload[3] = (byte)xVal;
      payload[4] = (byte)yVal;
      payload[5] = (byte)zVal;
      payload[6] = (byte)xAccel;
      payload[7] = (byte)yAccel;
      payload[8] = (byte)zAccel;
      payload[9] = incoming;
      
      xbee.send(tx);
      Serial.println("data sent");

      // flash TX indicator
      //---flashLed(statusLed, 1, 100);
    }
  
    /**
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000)) {
        // got a response!
        // should be a znet tx status              
      if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
         xbee.getResponse().getTxStatusResponse(txStatus);
        
         // get the delivery status, the fifth byte
           if (txStatus.getStatus() == SUCCESS) {
              // success.  time to celebrate
              flashLed(statusLed, 5, 50);
           } else {
              // the remote XBee did not receive our packet. is it powered on?
              flashLed(errorLed, 3, 500);
           }
        }      
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
      // or flash error led
    } else {
      // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
      flashLed(errorLed, 2, 50);
    }**/
    
    delay(1000);
}
