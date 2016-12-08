#include "BT.h"


#define LED_PIN 13
bool ledStatus = HIGH;
BT bt;



enum State {
  Idle,
  Drive,
  Brake,
  Blink,
  Dummy,
};
State _state = Idle;
State _lastState = Dummy;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledStatus);
  Serial.begin(9600);
  delay(2000);
  Serial.println("Here is Wheelchair Controller");
 
  bt.initialize();
  
  ledStatus = !ledStatus;
  digitalWrite(LED_PIN, ledStatus);
}

void getAndHandleNextBTRequest() {
  BT::Request btReq = bt.getNextRequest();
  //Serial.print("handling msg type: "); Serial.println(btReq.type);
  switch (btReq.type) {
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
  if (bt.checkForIncomingMsg())
    getAndHandleNextBTRequest();

  handleCurrentState();

}
