#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();
float xAccel;
float yAccel;
float zAccel;
float totalAccel=0;
float totalSpeed=0;

void setup() {
  Serial.begin(9600);
  
//Initialize accelerometer
  lis.setRange(LIS3DH_RANGE_16_G);
  Serial.print("Range = "); Serial.print(2 << lis.getRange()); Serial.print("G");
  if (! lis.begin(0x18)) {
    Serial.println("Couldnt start accelerometer");
    while (1);
  }
  Serial.println("  Accelerometer module found!");

}

void loop() {
  sensors_event_t event; 
  lis.getEvent(&event);
  xAccel=(float) event.acceleration.x;
//  Serial.print("\t\tX: "); Serial.print(xAccel);
  yAccel=event.acceleration.y;
//  Serial.print(" \tY: "); Serial.print(yAccel); 
  zAccel=event.acceleration.z;
//  Serial.print(" \tZ: "); Serial.print(zAccel); 
//  Serial.println(" m/s^2 ");
  totalAccel=sqrt(pow(xAccel, 2)+pow(yAccel, 2)+pow(zAccel, 2));
//  Serial.print("Accelerometer Total Acceleration: "); Serial.print(totalAccel); Serial.println(" m/s^2"); 
  totalSpeed+=totalAccel*0.01;
  Serial.print("Accelerometer Total Speed: "); Serial.print(totalSpeed); Serial.println(" m/s"); 
  delay(10);

}
