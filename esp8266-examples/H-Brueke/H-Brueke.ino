
#include <Arduino.h>
//10 links 
//01 rechts
//00 11 break

  #define LED_BUILTIN 16 
  #define D1 5 //Moter Rechts 0
  #define D2 4 //Motor Rechts 1
  #define D3 0 //Motor Links 0
  #define D4 2 //Motor Links 1
  #define D5 14
  #define D6 12
  #define D7 13
  #define D8 15
  #define RX
  #define TX
  #define A0

  #define Motor_R0 D1
  #define Motor_R1 D2
  #define Motor_L0 D3
  #define Motor_L1 D4
  
void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
      Serial.begin(9600);

      //H-bridge
        pinMode(Motor_R0, OUTPUT);
        pinMode(Motor_R1, OUTPUT);
        pinMode(Motor_L0, OUTPUT);
        pinMode(Motor_L1, OUTPUT);
}

void links_up()//10
{
  digitalWrite(Motor_L0, HIGH);
  digitalWrite(Motor_L1, LOW);
}


void links_down()//01
{
  digitalWrite(Motor_L0, LOW);
  digitalWrite(Motor_L1, HIGH);
}

void rechts_up()//10
{
  digitalWrite(Motor_R0, HIGH);
  digitalWrite(Motor_R1, LOW);
}

void rechts_down()
{
  digitalWrite(Motor_R0, LOW);
  digitalWrite(Motor_R1, HIGH);
}

void links_stop()//00
{  
  digitalWrite(Motor_L0, LOW);
  digitalWrite(Motor_L1, LOW);
}
void rechts_stop()//00
{
  digitalWrite(Motor_R0, LOW);
  digitalWrite(Motor_R1, LOW);
}


void rechts()
{
  links_up();
  rechts_stop();
}

void links()
{
  rechts_up();
  links_stop();
}

void forward()
{
	rechts_up();
	links_up();
}

void backward()
{
	rechts_down();
	links_down();
}

void loop()
{
  Serial.print("Hello");
  while(1)
  {
 /* digitalWrite(LED_BUILTIN, HIGH);
  rechts_up();
  links_up();
  
  delay(8000);
  digitalWrite(LED_BUILTIN, LOW);
  rechts_down();
  links_down();
  
  delay(8000);

  links();

  delay(2000);

  rechts();*/
    rechts_up();
  links_down();
  
  }
}
