#ifndef BT_h
#define BT_h

#include "Arduino.h"

#define BT_BAUD     9600
#define _bt         Serial1
#define BT_END_CHAR ';'
#define BT_SEP_CHAR ','
#define BT_BUFFER   32

#define BT_REQUEST_DRIVE_VALUE      'd'
#define BT_REQUEST_BLINK_VALUE      'b'
#define BT_REQUEST_DUMMY_VALUE      'g'
#define BT_REQUEST_HANDSHAKE_VALUE  '?'

#define BT_RESPONSE_HANDSHAKE_VALUE '!'



class BT {
  public:
     enum RequestType {
      BT_REQUEST_DRIVE,
      BT_REQUEST_DUMMY,
      BT_REQUEST_BLINK, 
      BT_REQUEST_HANDSHAKE,    
    };
    
    class Request {
      public:
        RequestType type;
        int value1 = 0;
        int value2 = 0;
        int value3 = 0;
        int valueDouble = 0.0;
        String valueStr = "";
    };

    enum ResponseType{
      BT_RESPONSE_HANDSHAKE,     
    };
    class Response{
      public:
        ResponseType type;
        int valueInt;
        double valueDouble;
        String valueStr;
    };
    
    void initialize();

    bool checkForIncomingMsg();
    bool hasResponse();
    bool hasRequest();
    Request getNextRequest();
    Response getNextResponse();
  
    void setAsSlave();
    //void setAsMaster(String macID);
    void sendRequest(Request req);
    void sendResponse(Response res);
    bool isConnected();
    //bool isMaster();

    String getThisMacID();



  private:
    bool _initialized = false;

    bool _isConnecting = false;
    bool _isConnected;
    bool tryReconnect = true;
    //bool _isMaster;
    String _slaveMacID;
    String _thisMacID;

    char _msgBuffer[BT_BUFFER];
    uint8_t _msgBufferCounter = 0;
    void setLastMsg(String msg);
    bool _hasResponse = false;
    bool _hasRequest = false;
    Request _lastRequest;
    Response _lastResponse;
    
    void onConnected();
    void onDisconnected();
    /*String cmdsMaster[8] = {
        F("AT"),
        F("AT+RESET"),
        F("AT+NAMEWheelchair"),
        F("AT+IMME1"),
        F("AT+NOTI1"),
        F("AT+ROLE1"),
        F("AT+SHOW1"),
        F("AT+CON"), //Just add mac address here
        };*/
    String cmdsSlave[8] = {
        F("AT"),
        F("AT+RESET"),
        F("AT+NAMEWheelchair"),
        F("AT+IMME1"),
        F("AT+NOTI1"),
        F("AT+ROLE0"),
        F("AT+FILT"),
        F("AT+SHOW1"),
        };  
  };

#endif
