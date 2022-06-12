// Author: Marcus Francisco
// Date: 6/8/2022
// Created in VS Code
// REU Research project for Digital Manufacturing
// This program is intended to recieve data via serial pins from an Arduino MEGA to an ESP-WROOM-32 
// The work of Rui Santos (Described below) was used to assist in connecting to the server.

/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
  
  Modified for AWS API Gateway by Stephen Borsay teaching AWS IoT on Udemy.com
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebSocketClient.h>

#define RXp0 3
#define TXp0 1
#define RXp1 27
#define TXp1 26
#define RXp2 16
#define TXp2 17

#define D2 2
#define D4 4

String inputString1 = "";
String inputString2 = "";
bool string1Complete = false;
bool string2Complete = false;

HardwareSerial SerialTFMini( 1 );
HardwareSerial SerialController( 2 );

// Integer "count" will allow us to track the order in which data is sent.
// From this we can observe the delay between transmission and reception.
int count = 0;

const char* ssid = "franhome3";
const char* password = "Ajfr@N93.";

//Your Domain name with URL path or IP address with path
String serverName = "https://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod"; //aws API Gateway url, Need htps:// pre-fix for ESP32 sketch
const char* API_KEY = "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8"; //Your AWS API-Key

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
unsigned long Uptime;

void setup() 
{


  
  // Here, the setup establishes serial connection by beginning serial comunication
  // The parameter here is the baud rate that we should be able to read data off of a serial monitor from the ESP32.
  Serial.begin(115200, SERIAL_8N1, RXp0, TXp0); // Serial bus for RX0 TX0 pins

  setupServerConnection();

  inputString1.reserve(200);
  inputString2.reserve(200);
  SerialController.begin( 9600 );
  SerialTFMini.begin(  9600, SERIAL_8N1, 27, 26 );
  //Serial1.begin(9600, SERIAL_8N1, RXp1, TXp1); // Serial bus for RX1 TX1 pins
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); // Serial bus for RX2 TX2 pins

  // Recieve data from Serial1
  //readFromRX2TX2();
  // Recieve data from Serial2
  delay(1500);

  pinMode(D2, INPUT);
  pinMode(D4, INPUT);
}
 
boolean buttonPressed = false;
boolean buttonPressed1 = false;

void loop()
{

  maintainServerConnection();
  // Request input from Arduino UNO on Serial0, in this case replicating the 3D printer.
  //requestInputS(); // Request serial input by powering D2 pin

  //delay(1000);

  //requestInputS2(); // Request serial2 input by powering D4 pin
  //inputString = Serial2.readString();
  //delay(1000);

  //readFromRX2TX2();
  //delay(1500);
  //readFromSerial1();  
  //delay(1500);
  //Serial0toSerial2();
  //helloWorldToSerial();

  // Send a request to the Arduino MEGA
  if(digitalRead(D2) == HIGH)
  {
    if(buttonPressed == false)
    {
      Serial2.println("MTemperature:"); // key is RtM for "Request To Mega"
    }
    buttonPressed = true;
  }
  else buttonPressed = false;

  // Send a request to the Arduino UNO
  if(digitalRead(D4) == HIGH)
  {
    if(buttonPressed1 == false)
    {
      SerialTFMini.println("UHumidity:"); // key is RtM for "Request To Mega"
    }
    buttonPressed1 = true;
  }
  else buttonPressed1 = false;
  

  // print the string when a newline arrives:
  if (string1Complete) {
    Serial.print("Serial1: ");
    Serial.println(inputString1);
    // clear the string:
    inputString1 = "";
    string1Complete = false;
  }  
  if (string2Complete) {
    Serial.print("Serial2: ");
    Serial.println(inputString2);
    // clear the string:
    inputString2 = "";
    string2Complete = false;
  }  
  serialEvent();
}

void setupServerConnection()
{
  //Serial.begin(115200); 

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
}

void maintainServerConnection() {

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
      //http.sendRequest("x-api-key: {zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8}");
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("Server Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        http.sendRequest("Hello World!");
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



void serialEvent() {
  while (SerialTFMini.available()) {
    // get the new byte:
    char inChar = (char)SerialTFMini.read();
    // add it to the inputString:
    inputString1 += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      string1Complete = true;
    }
  }
  while (Serial2.available()) {
    // get the new byte:
    char inChar1 = (char)Serial2.read();
    // add it to the inputString:
    inputString2 += inChar1;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar1 == '\n') {
      string2Complete = true;
    }
  }
}


void requestInputS()
{
  digitalWrite(D2, LOW);
  inputString1 = Serial.readString();

  Serial.print("StartESP");
  Serial.print(inputString1);
  Serial.println("EndESP");
  delay(1500);
  digitalWrite(D2, HIGH);
}

void requestInputS2()
{
  digitalWrite(D4, LOW);
  inputString1 = Serial2.readString();
  delay(1500);
  digitalWrite(D4, HIGH);
}

void helloWorldToSerial()
{
  // TO SEND UPON REQUEST USE SERIAL INTURRUPT

  // The Arduino will loop through sending various messages through the serial pins repeatedly. 
  // This will Allow us to confirm the connection between the two devices.
  Serial.print("ESP_Hello World!!_");
  Serial.println(count);

  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;

  // Delay between transmissions. Delay can be altered to make development easier.
  delay(1500);
}

void Serial0toSerial2()
{
  // This will output the message recieved from the Arduino, the message number, and the transmission number.
  Serial2.print("Message_");
  Serial2.print(count);
  Serial2.print(" Recieved: ");
  Serial2.println(Serial.readString());

  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;

}

void readFromRX2TX2()
{
  // This will output the message recieved from the Arduino, the message number, and the transmission number.
  Serial.print("S2_Message_");
  Serial.print(count);
  Serial.print(" Recieved: ");
  Serial.println(Serial2.readString());

  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;
}


void readFromSerial1()
{
  // This will output the message recieved from the Arduino, the message number, and the transmission number.
  Serial.print("S1_Message_");
  Serial.print(count);
  Serial.print(" Recieved: ");
  if(SerialTFMini.available())
  {
    String command = Serial.readStringUntil('\n'); // read string until newline character
    Serial.println(SerialTFMini.readString());
  }
  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;
}