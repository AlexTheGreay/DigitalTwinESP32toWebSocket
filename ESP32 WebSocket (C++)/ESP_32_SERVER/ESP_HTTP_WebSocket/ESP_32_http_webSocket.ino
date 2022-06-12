/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
  
  Modified for AWS API Gateway by Stephen Borsay teaching AWS IoT on Udemy.com
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_websocket_client.h>
#include <WebServer.h> // WebServer Library for ESP32
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include <ArduinoJson.h> // Arduino JSON Library
#include <Arduino.h>

const char* ssid = "franhome3";
const char* password = "Ajfr@N93.";

//Your Domain name with URL path or IP address with path
String serverName = "wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod"; //aws API Gateway url, Need htps:// pre-fix for ESP32 sketch
const char* API_KEY = "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8"; //Your AWS API-Key

WebSocketsClient webSocket; // websocket client class instance
StaticJsonDocument<100> doc; // Allocate a static JSON document

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
unsigned long Uptime;

void setup() {

  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  delay(2000); // wait for 2s

  //address, port, and URL path 
  //webSocket.sendHeader(webSocket.createHeader());
  webSocket.begin(serverName, 81, "/"); 
  // webSocket event handler
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);

}

void loop() {
  webSocket.loop(); // Keep the socket alive
  if(webSocket.isConnected()) Serial.println("Connected");
  else 
  {
    Serial.println("No Connection");
    webSocket.beginSocketIO(serverName, 81, "socket.io/?EIR=3", "arduino");
  } 

  delay(1000);


  int t =  random(30,110); //fake number range, adjust as you like
  int h =  random(50,100);

  Uptime = millis()/1000;
  
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

       String serverPath = serverName + "/?uptime=" + (String) Uptime
       + "&temperature=" + (String) t + "&humidity=" + (String) h;
      http.addHeader("x-api-key", API_KEY);
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }


}




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
  }
}