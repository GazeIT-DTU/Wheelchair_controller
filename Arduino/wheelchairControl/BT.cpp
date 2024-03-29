#include "BT.h"


void BT::initialize() {
  if (_initialized)
    return;
  _initialized = true;

  _bt.begin(BT_BAUD);
  delay(1000); //Let it settle
  
//Empty previous buffer
  _bt.clear();
  while (_bt.available())
    _bt.read();

  //Get MacID
  _bt.write("AT");
  delay(200);
  _bt.write("AT+ADDR?");
  //_bt.write(F("AT+ADDR?"));
  delay(500);

  //while (_bt.read() != ':');
  char buff[32];
  int counter = 0;
  while (_bt.available()) {
    char c = _bt.read();
    //   Serial.print(c);
    buff[counter++] = c;
  }
  if (counter > 0) {
    buff[counter] = '\0';
    String buffStr = String(buff);
    uint8_t index = buffStr.indexOf(':');
    if (-1 < index)
      _thisMacID = buffStr.substring(index + 1);
    Serial.print("Has macID: ");
    Serial.println(_thisMacID);
  }
  setAsSlave();
}



String BT::getThisMacID() {
  return _thisMacID;
}

bool BT::checkForIncomingMsg() {
  while (_bt.available()) {
    char c = _bt.read();
    //Serial.print(c);
    //Serial.print("|");
    if (c == BT_END_CHAR) {
      _msgBuffer[_msgBufferCounter] = '\0';
      setLastMsg(String(_msgBuffer));
      _msgBufferCounter = 0;
      return true;
    }

    //Add the char to the buffer
    _msgBuffer[_msgBufferCounter++] = c;

    //Create temp String with the current buffer
    _msgBuffer[_msgBufferCounter] = '\0'; //Tell String where it should stop
    String tempStr = String(_msgBuffer); //Create the string
    if (-1 < tempStr.indexOf("OK+CONN")) {
      //Device is connected
      onConnected();
      _msgBufferCounter = 0;
    }
    if (-1 < tempStr.indexOf("OK+LOST")) {
      //Lost connection
      onDisconnected();
      _msgBufferCounter = 0;
//      Serial.print("TempStr: "); Serial.println(tempStr);
//      _bt.print("Delay for 2000");
//      Serial.print("Delay for 2000");
      delay(2000);
      
    }

    if (-1 < tempStr.indexOf("OK+CONNF")) {
      //Failed to connect. Accept this and set as slave
      setAsSlave();
      _msgBufferCounter = 0;
    }
  }
  return false;
}

// Check wether msg is a valid "command" / state of the form ""
void BT::setLastMsg(String msg) {
  //Serial.print("BT::SetLastMsg: ");
  //Serial.println(msg);
  bool validRequest = false;
  bool validResponse = false;
  Request request;
  Response response;
  switch (msg[0]) {
    case BT_REQUEST_DRIVE_VALUE: {
        request.type = BT_REQUEST_DRIVE;
        
        int indexOfComma = msg.indexOf(',');
        int indexOfSemicolon = strlen(msg.c_str())-1;
        if (indexOfComma == -1 || indexOfSemicolon == -1 ) {
          Serial.println("Drive command of wrong format");
          Serial.print("Command was: "); Serial.println(msg);
          break;
        }
        // TODO Fix her // Problem here 30/01/2017
        const char* xString = msg.substring(1, indexOfComma-1).c_str();                  // X is from 1 to indexOfComma-1
        const char* yString = msg.substring(indexOfComma+1, indexOfSemicolon-1).c_str(); // Y is from indexOfComma+1 to indexOfSemicolon-1
        Serial.println(xString);
        Serial.println(yString); 
        int x = atoi(xString);
        int y = atoi(yString);
        
        request.value1 = x;
        request.value2 = y;
        validRequest = true;
        break;
      }
    case BT_REQUEST_DUMMY_VALUE: {
        request.type = BT_REQUEST_DUMMY;
        validRequest = true;
        break;
      }
    case BT_REQUEST_BLINK_VALUE: {
        request.type = BT_REQUEST_BLINK;
        validRequest = true;
        break;
      }
    case BT_REQUEST_HANDSHAKE_VALUE: {
        request.type = BT_REQUEST_HANDSHAKE;
        validRequest = true;
        break;
      }

    /*********************
       Responses
     *********************/
    case BT_RESPONSE_HANDSHAKE_VALUE: {
        response.type = BT_RESPONSE_HANDSHAKE;
        // response.valueDouble = msg.substring(1).toFloat();

        validResponse = true;
        break;
      }
  }

  if (validRequest) {
    _lastRequest = request;
    _hasRequest = true;
  } else if (validResponse) {
    _lastResponse = response;
    _hasResponse = true;
  }
}


void BT::sendRequest(Request req) {
  switch (req.type) {
    case BT_REQUEST_DRIVE: {
        //_bt.write(BT_REQUEST_IR_ON_VALUE);
        break;
      }
    case BT_REQUEST_DUMMY: {
        //_bt.write(BT_REQUEST_IR_ON_VALUE);
        break;
      }

    case BT_REQUEST_BLINK:
    case BT_REQUEST_HANDSHAKE:
      {
        Serial.print(F("BT is not allowed to request that..."));
        Serial.println(req.type);
        break;
      }

  }
  _bt.write(BT_END_CHAR);
  _bt.flush();
}


void BT::sendResponse(Response res) {
  switch (res.type) {
    case BT_RESPONSE_HANDSHAKE: {
        _bt.write(BT_RESPONSE_HANDSHAKE_VALUE);
        //_bt.print(res.valueDouble, 2);
        break;
      }
  }
  _bt.write(BT_END_CHAR);
  _bt.flush();
}

void BT::onConnected() {
  _isConnected = true;
  // Serial.println("We are connected to someone!!");



}

void BT::onDisconnected() {
  _isConnected = false;
  Serial.print("We lost the connection...");

}

bool BT::hasRequest() {
  return _hasRequest;
}


bool BT::hasResponse() {
  return _hasResponse;
}

BT::Request BT::getNextRequest() {
  _hasRequest = false;
  return _lastRequest;
}

BT::Response BT::getNextResponse() {
  _hasResponse = false;
  return _lastResponse;
}



void BT::setAsSlave() {
  _isConnecting = true;
  initialize();
  Serial.println("Setting device as BT slave... wait a couple of seconds");

  char buf[64];
  uint8_t length = 8;//sizeof(cmdsMaster);
  //Serial.print("Length: "); Serial.println(length);
  for (uint8_t i = 0; i < length; i++) {
    cmdsSlave[i].toCharArray(buf, sizeof(buf));



    _bt.write(buf);
    //   Serial.print(buf); Serial.print(": ");
    delay(1000);
    while (_bt.available())
      _bt.read();
    // Check that response is OK / OK+STUFF
    // checkForIncomingMsg();
    //   Serial.println();
  }
  _slaveMacID = "";
  _isConnecting = false;
  Serial.println("Done!");
}


bool BT::isConnected() {
  return _isConnected;
}

