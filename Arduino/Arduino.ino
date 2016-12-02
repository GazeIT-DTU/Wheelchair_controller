void setup() {
  Serial.begin(9600);
  delay(2000);

  pinMode(13, OUTPUT);
  bool init = true;
  while (init) {
    Serial.print("controller");
    delay(50);
  }


}

bool status;
void loop() {
  Serial.println("Halllooo");
  status = !status;
  digitalWrite(13, status);
  delay(500);
}
