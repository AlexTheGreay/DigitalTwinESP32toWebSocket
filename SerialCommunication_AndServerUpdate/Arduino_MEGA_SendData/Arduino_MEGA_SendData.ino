// Author: Marcus Francisco
// Date: 6/8/2022
// Created in VS Code
// REU Research project for Digital Manufacturing
// This program is intended to send data via serial pins from an Arduino MEGA to an ESP-WROOM-32 
// Upon request from the ESP32, the Arduino will send a packet of data to the ESP32 in the form of a string.

#include <Arduino.h>

#define RXp2 16
#define TXp2 17

// Integer "count" will allow us to track the order in which data is sent.
// From this we can observe the delay between transmission and reception.
int count = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// Setup includes all actions that occur once or at the start of initialization.
void setup() {
  // Here, the setup establishes serial connection by beginning serial comunication via pins 1 and 0 (by default).
  // The parameter here is the baud rate. This must match up with the rate the ESP32 is set to read this data.
  Serial.begin(9600); // The RX0 and TX0 pins are being used so the output can be observed by a connected computer.
  inputString.reserve(200);

  pinMode(53, INPUT); // Pins for input and output are used for developmental purposes.
  pinMode(52, OUTPUT);
  digitalWrite(52, HIGH);
}

void loop() 
{
  // When a newline character arrives, store the string as a variable.
  if (stringComplete) {
    Serial.print("From MEGA: ");
    Serial.print(millis());
    Serial.println("ms passed since start");
    Serial.print(inputString);
    Serial.print(rand());
    Serial.println("F");
    // clear the string:
    inputString = "";
    stringComplete = false;
  }  
  serialEvent();
}

void serialEventDefault() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

boolean keyFound = false;
void serialEvent() { // key is RtM for "Request To Mega"
    while(!keyFound & Serial.available())
    {
      char inChar1 = (char)Serial.read(); // get the new byte:

      if(inChar1 == 'M')// & inChar2 == 't' & inChar3 == 'M' ) // if the input chars (Bytes Read from the Serial) do not line up with key.
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

boolean isRequest(String inputString) // key is RtM for "Request To Mega"
{        
  if(strcmp("RtM_", "inputString") == 0)
  {
    return true;
  }
}

void readSerialInput() // This method will read any serial data being sent to the RX0 and TX0 pins and output to serial monitor at 9600 Baud Rate.
{
  // This will output the message recieved into the TX0 and RX0 pins, the message number, and the transmission number.
  Serial.print("Message_");
  Serial.print(count);
  Serial.print(" Recieved: ");
  Serial.println(Serial.readString());

  // The variable "count" is utilized to keep track of how many times the Arduino has transmitted data.
  // We will eventually expand on this to include the time of transmission, to ensure data is sent in the proper order.
  count++;
}

void sendSerialString(String message) // This method will send a string through the RX0 and TX0 pins with a 1.5s delay (for developmental purposes).
{
  // The Arduino will send the message through the serial pins. 
  Serial.print(message);

  // Delay between transmissions. Delay can be altered to make development easier.
  delay(1500);
}


void arduinoToESP(String message)
{
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


boolean aToE = true;
void defaultBehavior()
{
  // By default this program will transmit Arduino to ESP. Pressing the button for 2s will change direction.
  if(aToE) arduinoToESP("MEGA_Hello World!!_");

  // Use the button to switch the transfer direction 
  if(digitalRead(53) == HIGH)
  {
    // Loop will not break until button is unpressed.
    while(digitalRead(53) == HIGH)
    {
    digitalWrite(52, LOW); // Low voltage on 52 will cause ESP32 to switch to transmitting mode.
    delay(1500);
    count = 0;
    }
    // Toggle transfer direction. Arduino to Esp is now false.
    aToE = ~aToE;
    if(~aToE)
    {
        // THIS CODE IS NOT FINISHED - Setup to ESP to ARDUINO goes here.
    }
    else
    {
      Serial.begin(9600);
    }
  }
}
