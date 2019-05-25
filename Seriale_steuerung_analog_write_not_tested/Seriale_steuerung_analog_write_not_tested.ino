 
  #include <Arduino.h>
  
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




// Use it to make a delay... without delay() function!
  long previousMillis = -1000*10;// -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.  
  long interval = 1000*10;       // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
  unsigned long currentMillis;   //unsigned long currentMillis;
//Useful Variables
  int state;
  int vSpeed=200;     // Default speed, from 0 to 255

void setup() {
    // Set pins as outputs:
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);  
    // Initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
}
 
void loop() {

  //Save income data to variable 'state'
    if(Serial.available() > 0){     
      state = Serial.read();   
    }
  
  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
    if (state == '0'){
      vSpeed=0;}
    else if (state == '1'){
      vSpeed=100;}
    else if (state == '2'){
      vSpeed=180;}
    else if (state == '3'){
      vSpeed=200;}
    else if (state == '4'){
      vSpeed=255;}
 	  
  /***********************Forward****************************/
  //If state is equal with letter 'F', boat will go forward!
    if (state == 'F') {
    	analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0);
        analogWrite(motorB1, vSpeed); analogWrite(motorB2, 0); 
    }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', boat will go forward left
    else if (state == 'G') {
    	analogWrite(motorA1, 100);    analogWrite(motorA2, 0);  
        analogWrite(motorB1, vSpeed); analogWrite(motorB2, 0); 
    }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', boat will go forward right
    else if (state == 'I') {
      	analogWrite(motorA1, vSpeed); analogWrite(motorA2, 0); 
        analogWrite(motorB1, 100);    analogWrite(motorB2, 200); 
    }
  /***********************Backward****************************/
  //If state is equal with letter 'B', boat will go backward
    else if (state == 'B') {
    	analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, vSpeed); 
    }
  /**********************Backward Left************************/
  //If state is equal with letter 'H', boat will go backward left
    else if (state == 'H') {
    	analogWrite(motorA1, 0);   analogWrite(motorA2, 100); 
        analogWrite(motorB1, 0); analogWrite(motorB2, vSpeed); 
    }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', boat will go backward right
    else if (state == 'J') {
    	analogWrite(motorA1, 0);   analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, 0);   analogWrite(motorB2, 100); 
    }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
    else if (state == 'L') {
    	analogWrite(motorA1, 0);     analogWrite(motorA2, vSpeed); 
        analogWrite(motorB1, vSpeed);analogWrite(motorB2, 0); 
    }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
    else if (state == 'R') {
    	analogWrite(motorA1, vSpeed);analogWrite(motorA2, 0); 
        analogWrite(motorB1, 0);     analogWrite(motorB2, vSpeed); 		
    }

  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the boat
    else if (state == 'S'){
        analogWrite(motorA1, 0);  analogWrite(motorA2, 0); 
        analogWrite(motorB1, 0);  analogWrite(motorB2, 0);
    }

    
}
