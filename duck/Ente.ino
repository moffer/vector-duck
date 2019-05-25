#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// WLAN SSID Define
#ifndef APSSID
#define APSSID "EnteEnteEnte"
#define APPSK  "thereisnospoon"
#endif


#define LED_BUILTIN 16 
#define D1 5 //Moter Rechts 0
#define D2 4 //Motor Rechts 1
#define D3 0 
#define D4 2  //SDA
#define D5 14 //SCL
#define D6 12 //Motor Links 0
#define D7 13 //Motor Links 1
#define D8 15
#define RX
#define TX
#define A0
 

#define motorA1 D1  //Achtung das ist kein PWM pin  PWM nur an D6,D8,D7  TODO PWM mit Oszi testen ?!
#define motorA2 D2  //Achtung das ist kein PWM pin 
#define motorB1 D6
#define motorB2 D7


/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

// Use it to make a delay... without delay() function!
  long previousMillis = -1000*10;// -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.  
  long interval = 1000*10;       // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
  unsigned long currentMillis;   //unsigned long currentMillis;
//Useful Variables
  int state;
  int vSpeed=200;     // Default speed, from 0 to 255


//Webserver cnfig
ESP8266WebServer server(80);
//Websocket config
WebSocketsServer webSocket(81); // create a websocket server on port 81



/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>Ich bin eine Ente</h1>");
}



void setup() {
  // Set pins as outputs:
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);  
  
  delay(1000);
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  startWebSocket();
}

void loop() {
  webSocket.loop();      // constantly check for websocket events
  server.handleClient();
  
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)
{ 

{ // When a WebSocket message is received
  switch (type)
  {
  case WStype_DISCONNECTED: // if the websocket is disconnected
    
    Serial.printf("[%u] Disconnected!\n", num);
    
    break;
  case WStype_CONNECTED:
  { // if a new websocket connection is established
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    // rainbow = false; // Turn rainbow off when a new connection is established
  }
  break;
  case WStype_TEXT: // if new text data is received
    Serial.printf("[%u] get Text: %s\n", num, payload);

  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
    if (payload[0] == '0'){
      vSpeed=0;}
    else if (payload[0] == '1'){
      vSpeed=100;}
    else if (payload[0] == '2'){
      vSpeed=180;}
    else if (payload[0] == '3'){
      vSpeed=200;}
    else if (payload[0] == '4'){
      vSpeed=255;}

    /***********************Forward****************************/
  //If state is equal with letter 'F', boat will go forward!
    if (payload[0] == 'F') {
      analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0);
        analogWrite(motorB1, vSpeed); analogWrite(motorB2, 0); 
    }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', boat will go forward left
    else if (payload[0] == 'G') {
      analogWrite(motorA1, 100);    analogWrite(motorA2, 0);  
        analogWrite(motorB1, vSpeed); analogWrite(motorB2, 0); 
    }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', boat will go forward right
    else if (payload[0] == 'I') {
        analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0); 
        analogWrite(motorB1, 100);    analogWrite(motorB2, 200); 
    }
  /***********************Backward****************************/
  //If state is equal with letter 'B', boat will go backward
    else if (payload[0] == 'B') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, vSpeed); 
    }
  /**********************Backward Left************************/
  //If state is equal with letter 'H', boat will go backward left
    else if (payload[0] == 'H') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, 100); 
        analogWrite(motorB1, 0); analogWrite(motorB2, vSpeed); 
    }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', boat will go backward right
    else if (payload[0] == 'J') {
      analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, 100); 
    }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
    else if (payload[0] == 'L') {
      analogWrite(motorA1, 0);     analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, vSpeed);analogWrite(motorB2, 0); 
    }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
    else if (payload[0] == 'R') {
      analogWrite(motorA1, vSpeed);analogWrite(motorA2, 0); 
        analogWrite(motorB1, 0);     analogWrite(motorB2, vSpeed);    
    }

  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the boat
    else if (payload[0] == 'S'){
        analogWrite(motorA1, 0);  analogWrite(motorA2, 0); 
        analogWrite(motorB1, 0);  analogWrite(motorB2, 0);
    }
    break;
  }
}
  
  }

void startWebSocket()
{                                       // Start a WebSocket server
    webSocket.begin();                   // start the websocket server
    webSocket.onEvent(webSocketEvent); // if there's an incomming websocket message, go to function 'webSocketEvent'
//    webSocket.enableHeartbeat(15000, 3000, 2);
    Serial.println("WebSocket server started.");
}
