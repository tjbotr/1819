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
  

XBee xbee = XBee();




int pin5 = 0;

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
  sensors_event_t event; 
  lis.getEvent(&event);

  xAccel = event.acceleration.x;
  yAccel = event.acceleration.y;
  zAccel = event.acceleration.z;

  incoming = myI2CGPS.read();
  Tx16Request tx = Tx16Request(0x0, payload, sizeof(payload));


TxStatusResponse txStatus = TxStatusResponse();
  uint8_t payload[] = { pascals, altm, tempC, xVal, yVal, zVal, xAccel, yAccel, zAccel };
  
   Serial.println("1");
      // break down 10-bit reading into two bytes and place in payload
      pin5 = analogRead(5);
      payload[0] = pin5 >> 8 & 0xff;
      payload[1] = pin5 & 0xff;
      
      xbee.send(tx);
Serial.println("send?");
  
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000)) {
        // got a response!
Serial.println("3");
        // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getTxStatusResponse(txStatus);
 Serial.println("4");
    	   // get the delivery status, the fifth byte
           if (txStatus.getStatus() == SUCCESS) {
            	// success.  time to celebrate
  Serial.println("5");
           } else {
   Serial.println("6");
           }
        }      
    } else if (xbee.getResponse().isError()) {
      nss.print("Error reading packet.  Error code: ");  
      nss.println(xbee.getResponse().getErrorCode());
      Serial.println("7");
    } else {
      // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
      Serial.println("8");
    }
    
    delay(1000);
}
