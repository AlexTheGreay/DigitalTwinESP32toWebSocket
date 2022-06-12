/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
  
  Modified for AWS API Gateway by Stephen Borsay teaching AWS IoT on Udemy.com
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  // This code also implements the contents of https://iotdesignpro.com/projects/real-time-data-transfer-between-two-esp32-using-websocket-client-on-arduino-ide
*/

#include <HTTPClient.h>
#include <WiFi.h> // ESP32 WiFi Library
#include <WebServer.h> // WebServer Library for ESP32
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include <ArduinoJson.h> // Arduino JSON Library
#include <Arduino.h>
//#include <Adafruit_SSD1306.h> // OLED Display Library NOT NEEDED FOR THIS PROJECT (yet)

//Your Domain name with URL path or IP address with path
String serverName = "wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod"; //aws API Gateway url, Need htps:// pre-fix for ESP32 sketch
const char* API_KEY = "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8"; //Your AWS API-Key

// Wifi Credentials
const char* ssid = "franhome3";
const char* password = "Ajfr@N93.";

WebSocketsClient webSocket; // websocket client class instance
StaticJsonDocument<100> doc; // Allocate a static JSON document




void setup() {
  // Connect to local WiFi
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP address

  delay(2000); // wait for 2s

  //address, port, and URL path 
  webSocket.begin(serverName, 81, API_KEY); 
  // webSocket event handler
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
}







void loop() {
  webSocket.loop(); // Keep the socket alive
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