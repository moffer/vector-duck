#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#include <Wire.h> // This library allows you to communicate with I2C devices.

// WLAN SSID Define
#ifndef APSSID
#define APSSID "EnteEnteEnte"
#define APPSK  "thereisnospoon"
#endif


#define LED_BUILTIN 16 
#define D1 5 
#define D2 4 
#define D3 0 
#define D4 2  //SDA
#define D5 14 //SCL
#define D6 12 
#define D7 13 
#define D8 15
#define RX
#define TX
#define A0
 

// Motor A rechts
#define motorA1 D1  //Achtung das ist kein PWM pin  PWM nur an D6,D8,D7  TODO PWM mit Oszi testen ?!
#define motorA2 D2  //Achtung das ist kein PWM pin 
// Motor B links
#define motorB1 D6
#define motorB2 D7



  #define SDA D4
  #define SCL D5
  

//*************************i2c variablen und gyro zeug
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z, init_gyro_z;; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


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



void check_gyro ();



void setup() {
  // Set pins as outputs:
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  digitalWrite(motorA1, LOW);  digitalWrite(motorA2, LOW); 
  digitalWrite(motorB1, LOW);  digitalWrite(motorB2, LOW);
  


//I2C init 
    Wire.begin(SDA,SCL);
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

// gyro startwert Hier sollte vielleicht noch gemittelt werden
  check_gyro();
  init_gyro_z =gyro_z;
  
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

    /***********************Backward****************************/
  //If state is equal with letter 'B', boat will go backward!
    if (payload[0] == 'B') {
      digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
        digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW); 
    }
//  /**********************Forward Left************************/
//  //If state is equal with letter 'G', boat will go forward left
//    else if (payload[0] == 'G') {
//      analogWrite(motorA1, 100);    analogWrite(motorA2, 0);  
//        analogWrite(motorB1, vSpeed); analogWrite(motorB2, 0); 
//    }
//  /**********************Forward Right************************/
//  //If state is equal with letter 'I', boat will go forward right
//    else if (payload[0] == 'I') {
//        analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0); 
//        analogWrite(motorB1, 100);    analogWrite(motorB2, 200); 
//    }
  /***********************Forward****************************/
  //If state is equal with letter 'F', boat will go forward
    else if (payload[0] == 'F') {
      digitalWrite(motorA1, LOW);   digitalWrite(motorA2, HIGH); 
        digitalWrite(motorB1, LOW);   digitalWrite(motorB2, HIGH); 
    }
//  /**********************Backward Left************************/
//  //If state is equal with letter 'H', boat will go backward left
//    else if (payload[0] == 'H') {
//      analogWrite(motorA1, 0);   analogWrite(motorA2, 100); 
//        analogWrite(motorB1, 0); analogWrite(motorB2, vSpeed); 
//    }
//  /**********************Backward Right************************/
//  //If state is equal with letter 'J', boat will go backward right
//    else if (payload[0] == 'J') {
//      analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
//        analogWrite(motorB1, 0);   analogWrite(motorB2, 100); 
//    }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
    else if (payload[0] == 'L') {
      digitalWrite(motorA1, LOW);     digitalWrite(motorA2, HIGH); 
        digitalWrite(motorB1, HIGH);digitalWrite(motorB2, LOW); 
    }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
    else if (payload[0] == 'R') {
      digitalWrite(motorA1, HIGH);digitalWrite(motorA2, LOW); 
        digitalWrite(motorB1, LOW);     digitalWrite(motorB2, HIGH);      
    }

  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the boat
    else if (payload[0] == 'S'){
        digitalWrite(motorA1, LOW);  digitalWrite(motorA2, LOW); 
        digitalWrite(motorB1, LOW);  digitalWrite(motorB2, LOW);
    }
    /************************Gyro Regelung*****************************/
  //If state is equal with letter 'X', starts the gyro Regelung
    else if (payload[0] == 'X'){
        digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
        digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW); 
//  Serial.print("gogogogog");
 // Serial.println();
  delay(1000);
check_gyro();

    while(gyro_z > init_gyro_z) //rechtskorrektur
    {
          digitalWrite(motorA1, HIGH);digitalWrite(motorA2, LOW); //rechts
        digitalWrite(motorB1, LOW);     digitalWrite(motorB2, HIGH);   //rechts
    check_gyro();
    //Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));Serial.print("rechts");Serial.println();
    delay(100);
    }
    
    digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
        digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW); 
  //Serial.print("gogogogog");
  //Serial.println();
    delay(1000);
    check_gyro();

    
    while(gyro_z < init_gyro_z) //linkskorrektur
    {
            digitalWrite(motorA1, LOW);     digitalWrite(motorA2, HIGH); //links
        digitalWrite(motorB1, HIGH);digitalWrite(motorB2, LOW); //links
      check_gyro();
   //   Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));Serial.print("links");Serial.println();
      delay(100);
    }
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



void check_gyro ()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
}
