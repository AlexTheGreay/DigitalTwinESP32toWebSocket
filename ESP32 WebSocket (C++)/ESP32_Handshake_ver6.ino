
// Author: Alexander Francisco
// 6/8-10/2022
// REU Research Project for Digital Manufacturing
// Purpose: Have an ESP32 connect to a WebSocket Server

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h>



// Wifi Authentication
const char* ssid     = "franhome3"; //Wifi SSID
const char* password = "Ajfr@N93."; //Wifi Password
const char* URI_ADDRESS = "wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod"; //Websocket Server's Address
const char* publicKey="zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8";



using namespace websockets;


//const char x-api-key= "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8" ;




void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());

Serial.print("Called : onMessageCallback\n");

}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }

Serial.print("Called : onEventsCallback\n");
}

WebsocketsClient client;
void setup() {
  
Serial.print("Made it to step 2: Enter Void Setup\n");
    Serial.begin(9600);
    // Connect to wifi
    WiFi.begin(ssid, password);

 

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
        
    }

Serial.print("Made it to step 3: Connect to Wifi\n");

    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);

    // Before connecting, set the ssl fingerprint of the server
    client.setCACert(publicKey);

    //PublicKey *publicKey = new PublicKey(public_key);
    //client.setKnownKey(publicKey);

    // Connect to server
    client.connect(URI_ADDRESS);



 StaticJsonDocument<400> doc; //Allocate JSON Doc
  doc["Content-Type"]= "application/json";
  doc["x-api-key"] = "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8";  //added random values 123456 at front to test
  
Serial.print("Made it to step 4 Allocated JSON & Assigned Key\n\n\n");

  JsonArray data = doc.createNestedArray("data");
  data.add(1234);
  data.add(2.333);

  char output[]=" ";
  serializeJson(doc,output);
  Serial.println(output);
  Serial.println();

Serial.print("Serialized\n");
 
  delay(10000);

    // Send a message
    client.send(output);

Serial.print("Sent JSON to server!\n");

    // Send a ping
    client.ping();
    
Serial.print("Sent Ping\n");
}

void loop() {
    client.poll();
    
Serial.print("Made it to Void Loop (Client Poll)\n");
delay (5000) ; 
Serial.print("\n");
client.send("Hi Server!");
client.ping();
}






 
