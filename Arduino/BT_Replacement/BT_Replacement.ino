
#define _bt Serial1

/**
 * Repeater / forwarder. This represents the Teensy at the PC side. 
 * It forwards all messages and acts as a medium, between communication technologies. 
 * TODO: this should represent the teensy by the PC side and be set up as a MASTER/Centra/ "GATT Client"
 * Hvis connection lost prøv at oprette forbindelse igen
 */
// This is: OK+ADDR:20914841C5B0

void setup() {
  Serial.begin(9600);
  _bt.begin(9600);
  delay(2000);
  Serial.println("Here is repeater...");
  connectToSlave();
  Serial.println("Tried connecting");
  pinMode(13,OUTPUT);
}

bool status;

void loop() {
  while (Serial.available()){
    _bt.write(Serial.read());
  }
  
  while (_bt.available()){

    Serial.write(_bt.read());
    //_bt.write();
    
//    char ch = _bt.read();
//    _bt.write(ch);
//    _bt.print('|');
//    
//    Serial.write(ch);
//    Serial.print('|');

    status = !status;
    digitalWrite(13, status);
    //delay(50);
  }
    // put your main code here, to run repeatedly:

}

void connectToSlave(){
  int timeInMillis = 2000;
//  delay(timeInMillis);
//    Serial.println("Writing AT commands");
//    _bt.write("AT");
//    delay(timeInMillis);
//    _bt.write("AT+RESET");
//    delay(timeInMillis);
//    _bt.write("AT+NAMEbt_passthrough");
//    delay(timeInMillis);
//    _bt.write("AT+IMME1");
//    delay(timeInMillis);
//    _bt.write("AT+NOTI1");
//    delay(timeInMillis);
//    _bt.write("AT+ROLE1");
//    delay(timeInMillis);
//    _bt.write("AT+MODE0");
//    delay(timeInMillis);
//    _bt.write("AT+SHOW1");
//    delay(timeInMillis);
//    Serial.println("Done writing AT commands!");
    char test[19];
    String cmd = "AT+CON20914841C5E7";
    cmd.toCharArray(test, sizeof(test));
    _bt.write(test); // Add mac address here
    Serial.println();
}

