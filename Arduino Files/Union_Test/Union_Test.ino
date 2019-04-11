void setup() {
  Serial.begin(9600);
  
  float acceleration=2;

  union u_tag {
     float floatData;
     uint8_t b[4];
  } u;

  union v_tag {
     float floatData;
     uint8_t b[4];
  } v;

  u.floatData=acceleration;
  Serial.println(u.floatData);

  Serial.println(u.b[0]);
  Serial.println(u.b[1]);
  Serial.println(u.b[2]);
  Serial.println(u.b[3]);
  
//  u.b[0] = snelheidArray[0];
//  u.b[1] = snelheidArray[1];
//  u.b[2] = snelheidArray[2];
//  u.b[3] = snelheidArray[3];
//  
//  snelheid = u.fval;
//
  v.b[0]=u.b[0];
  v.b[1]=u.b[1];
  v.b[2]=u.b[2];
  v.b[3]=u.b[3];

  Serial.println(v.floatData);


}

void loop() {
  // put your main code here, to run repeatedly:

}
