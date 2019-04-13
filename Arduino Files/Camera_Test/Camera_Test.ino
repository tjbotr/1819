const int cameraPin = 6;

void setup() {
  Serial.begin(9600);
  pinMode(cameraPin, OUTPUT);
  digitalWrite(cameraPin, HIGH);
  delay(10000);
}

void loop() {
  digitalWrite(cameraPin, LOW);
  delay(700);
  
  Serial.println("Started recording");
  digitalWrite(cameraPin, HIGH);
  delay(5000);
  
  digitalWrite(cameraPin, LOW);
  delay(700);
  digitalWrite(cameraPin, HIGH);
  Serial.println("Stopped recording");
  while(1);
}
