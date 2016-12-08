
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);
  Serial.println("Here is repeater...");
  pinMode(13,OUTPUT);
}

bool status;

void loop() {
  while (Serial.available()){
    Serial1.write(Serial.read());
    status = !status;
    digitalWrite(13, status);
    delay(200);
  }
  
  while (Serial1.available())
    Serial.write(Serial1.read());
  // put your main code here, to run repeatedly:

}
