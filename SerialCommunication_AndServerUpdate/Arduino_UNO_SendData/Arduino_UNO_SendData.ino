// Author: Marcus Francisco
// Date: 6/8/2022
// Created in VS Code
// REU Research project for Digital Manufacturing
// This program is intended to send data via serial pins from an Arduino MEGA to an ESP-WROOM-32 
// Hold the button on the Breadboard for 2 seconds to switch the direction of transmission.

#include <Arduino.h>

#define RXp2 16
#define TXp2 17
String inputMessage;

// Integer "count" will allow us to track the order in which data is sent.
// From this we can observe the delay between transmission and reception.
int count = 0;

// Setup includes all actions that occur once or at the start of initialization.
void Oldsetup() {
  // Here, the setup establishes serial connection by beginning serial comunication via pins 1 and 0 (by default).
  // The parameter here is the baud rate. This must match up with the rate the ESP32 is set to read this data.
  Serial.begin(9600);
  pinMode(2, INPUT);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

}

boolean hasSent = false;

void Oldloop() // The arduino will loop through this code indefinitely.
{
  // By default this program will transmit Arduino to ESP. Pressing the button for 2s will change direction.
  arduinoToESP("UNO_Hello World!!_");

}

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// Setup includes all actions that occur once or at the start of initialization.
void setup() {
  // Here, the setup establishes serial connection by beginning serial comunication via pins 1 and 0 (by default).
  // The parameter here is the baud rate. This must match up with the rate the ESP32 is set to read this data.
  Serial.begin(9600); // The RX0 and TX0 pins are being used so the output can be observed by a connected computer.
  inputString.reserve(200);

}

void loop() 
{
  // When a newline character arrives, store the string as a variable.
  if (stringComplete) {
    Serial.print("From UNO: ");
    Serial.print(millis());
    Serial.println("ms passed since start");
    Serial.print(inputString);
    Serial.print(rand());
    Serial.println("C");
    // clear the string:
    inputString = "";
    stringComplete = false;
  }  
  serialEvent();
}

boolean keyFound = false;
void serialEvent() { // key is RtM for "Request To Mega"
    while(!keyFound & Serial.available())
    {
      char inChar1 = (char)Serial.read(); // get the new byte:

      if(inChar1 == 'U')// & inChar2 == 't' & inChar3 == 'M' ) // if the input chars (Bytes Read from the Serial) do not line up with key.
      {
        keyFound = true;
      }
      else 
      {
        // Shift the chars and read a new byte
        inChar1 = (char)Serial.read(); // get the new byte:
      }
        
    }
    while(keyFound & Serial.available())
    {
      // Begin reading chars.
      char inChar = (char)Serial.read(); // get the new byte:
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if (inChar == '\n') {
        stringComplete = true;
        keyFound = false;
      }
    }
}



void StandBy()
{
  //Standby mode waiting for request from ESP-32 through digital pins.
  if(digitalRead(2) == HIGH && hasSent == false)
  {
    delay(100);
    Serial.println("HelloFromUNO");
    hasSent = true;
    digitalWrite(3, HIGH);
  }
  if(digitalRead(2) != HIGH)
  {
    hasSent = false;
  }
}


void readSerialInput() // This method will read any serial data being sent to the RX0 and TX0 pins and output to serial monitor at 9600 Baud Rate.
{
  // This will output the message recieved into the TX0 and RX0 pins.
  inputMessage = Serial.readString(); // NOTE: THIS IS FLAWED, CANNOT READ SAME DATA TWICE

  //Output into serial monitor for developmental purposes. COMMENT OUT 
  Serial.print("Message_");
  Serial.print(" Recieved: ");
  Serial.println(Serial.readString());
}

void arduinoToESP(String message)
{
  // TO SEND UPON REQUEST USE SERIAL INTURRUPT

  // The Arduino will loop through sending various messages through the serial pins repeatedly. 
  // This will Allow us to confirm the connection between the two devices.

  Serial.print(message);
  Serial.println(count);

  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;

  // Delay between transmissions. Delay can be altered to make development easier.
  delay(1500);
}

void espToArduino(String message)
{
  // THIS CODE IS NOT FINISHED - Method to transmit a message from ESP to Arduino Goes here.
}