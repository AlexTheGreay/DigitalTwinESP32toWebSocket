// Author: Alexander Francisco
// 6/8/2022
// REU Research Project for Digital Manufacturing
// Purpose: Have an ESP32 connect to a WebSocket Server

#include <WebServer.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>


WebSocketsClient webSocket;  //Websocket Client Class instance
StaticJsonDocument<400> doc; //Allocate JSON Doc

// Wifi Authentication
const char* ssid     = "franhome3"; //Wifi SSID
const char* password = "Ajfr@N93."; //Wifi Password

void setup() {
  
  Serial.println("\n\n\n");
  Serial.begin(9600); // Baud Rate

  //Connect to WiFi
  WiFi.begin(ssid,password);
  //Wait for WiFi Connection
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("... ");
    delay(500);
    }
  //Once WiFi Connects, show connectivity
  if(WiFi.status() == WL_CONNECTED){
      Serial.printf("\nConnection to %s successful!",ssid);
    }
    else {
      Serial.printf("\n ERROR, UNABLE TO CONNECT TO %s",ssid);
      }

  //Print Local IP Address
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //address, port, and URL path
  //FIXME
  //Attempt to connect to the WebSocket using URL
  webSocket.begin("192.168.1.217",8080,"/");
}

  // WebSocket event handler
 // webSocket.onEvent(webSocketEvent);

  // if connection fails try every 5s
 // webSocket.setReconnectInterval(5000);

  

void loop() {
  webSocket.loop(); // Keep the socket running
}


  // Local Functions:
  
  void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  // Make sure the screen is clear
  //  u8g2.clearBuffer();
  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(doc, payload); // deserialize incoming Json String
    if (error) { // Print erro msg if incomig String is not JSON formated
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    const String pin_stat = doc["PIN_Status"]; // String variable tha holds LED status
    const float t = doc["Temp"]; // Float variable that holds temperature
    const float h = doc["Hum"]; // Float variable that holds Humidity
    // Print the received data for debugging
    Serial.print(String(pin_stat));
    Serial.print(String(t));
    Serial.println(String(h));
    // Send acknowledgement
    // webSocket.sendTXT("OK");
