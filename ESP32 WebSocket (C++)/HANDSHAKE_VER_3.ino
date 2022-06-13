


// Author: Alexander Francisco
// 6/8-10/2022
// REU Research Project for Digital Manufacturing
// Purpose: Have an ESP32 connect to a WebSocket Server

#include <ArduinoWebsockets.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Hash.h>

webSocketsClient webSocket;
// Wifi Authentication
const char* ssid     = "franhome3"; //Wifi SSID
const char* password = "Ajfr@N93."; //Wifi Password
const char* URI_ADDRESS = "wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod"; //Websocket Server's Address
const char* key="zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8";


void setup() {
  Serial.begin(9600);
    // Connect to wifi
    WiFi.begin(ssid, password);

 

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
        
    }

Serial.print("Made it to step 3: Connect to Wifi\n");

  
  webSocket.begin("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com",8080,"/prod");
  webSocket.setExtraHeaders("x-api-key:zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8");

/*
using namespace websockets;
WebsocketsClient client;
  bool connected = client.connect("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod");
  //,"x-api-key:zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8"
  if (connected){
    Serial.println("connected");
    client.send("Hello Server!");
    }
    else {
      Serial.println("Not Connected");
      }
  }
*/
void loop() {
  // put your main code here, to run repeatedly:

}
