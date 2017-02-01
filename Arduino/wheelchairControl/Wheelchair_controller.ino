#include "BT.h"

// This represents the teensy at the wheelchair side.

#define LED_PIN 13
bool ledStatus = HIGH;
BT bt;

// This is: OK+ADDR:20914841C5E7


enum State {
  Idle,
  Drive,
  Brake,
  Blink,
  Dummy,
};
State _state = Idle;
State _lastState = Idle;
BT::Request _currentRequrest;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledStatus);
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);
  Serial.println("Here is Wheelchair Controller");
 
  bt.initialize();
  
  ledStatus = !ledStatus;
  digitalWrite(LED_PIN, ledStatus);
}

// Set state
void getAndHandleNextBTRequest() {
  BT::Request btReq = bt.getNextRequest();
  //Serial.print("handling msg type: "); Serial.println(btReq.type);
  switch (btReq.type) {
    case BT::BT_REQUEST_DRIVE: {
        _state = Drive;
        _currentRequrest = btReq;
        break;
      }
      
    case BT::BT_REQUEST_BLINK: {
        _state = Blink;
        break;
      }

    case BT::BT_REQUEST_DUMMY: {
        _state = Dummy;
        break;
      }

    case BT::BT_REQUEST_HANDSHAKE: {
        Serial.println("Responding to handshake!");
        BT::Response res;
        res.type = BT::BT_RESPONSE_HANDSHAKE;
        bt.sendResponse(res);
        ledStatus = !ledStatus;
  digitalWrite(LED_PIN, ledStatus);
        break;
      }
  }
}

// What functions to do in this state
void handleCurrentState() {
  /**************************
    If first round with this new state
   **************************/
  if (_lastState != _state) {
    switch (_state) {
      case Idle: {
          //Empty
          break;
        }

      case Drive: {
          Serial.println("Drive!");
          Serial.print("X: "); Serial.println(_currentRequrest.value1);
          Serial.print("Y: "); Serial.println(_currentRequrest.value2);
          
          // Set output to either motordrives or joystick emulated
          
          break;
        }  

      case Blink: {
          Serial.println("Blink!");
          ledStatus = !ledStatus;
          digitalWrite(LED_PIN, ledStatus);
          _state = Idle;
          break;
        }

      case Dummy: {
          Serial.println("Dummy!");
          _state = Idle;
          break;
        }

      default: {
          Serial.print("New state defaulted.... ");
          Serial.println(_state);
          break;
        }
    }

    _lastState = _state;
    return;

  }



  /***********************************
     This state isn't new to us...
   ***********************************/
  switch (_state) {
    case Idle: {
        //Do nothing
        break;
      }
    default: {
        //Empty...
        break;
      }
  }
}



void loop() {
// Was removing the input from the buffer before it was intepreted
//  while(Serial1.available()){
//    Serial.write(Serial1.read());
//  }
//  while(Serial.available()){
//    Serial1.write(Serial.read());
//  }
//  
  if (bt.checkForIncomingMsg())
    getAndHandleNextBTRequest();

  handleCurrentState();
}
