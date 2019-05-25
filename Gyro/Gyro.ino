

#include "Wire.h" // This library allows you to communicate with I2C devices.
#include <Arduino.h>
//10 links 
//01 rechts
//00 11 break

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

  #define Motor_R0 D1
  #define Motor_R1 D2
  #define Motor_L0 D6
  #define Motor_L1 D7

  #define SDA D4
  #define SCL D5
  


const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z, init_gyro_z;; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


void check_gyro ();


void setup() {
  Serial.begin(9600);
  Wire.begin(SDA,SCL);
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);


      //H-bridge
        pinMode(Motor_R0, OUTPUT);
        pinMode(Motor_R1, OUTPUT);
        pinMode(Motor_L0, OUTPUT);
        pinMode(Motor_L1, OUTPUT);

check_gyro();
init_gyro_z =gyro_z;
  
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




void loop() {

forward();
  Serial.print("gogogogog");
  Serial.println();
  delay(1000);
check_gyro();

    while(gyro_z > init_gyro_z)
    {
    rechts();
    check_gyro();
    Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));Serial.print("rechts");Serial.println();
    delay(100);
    }
    
    while(gyro_z < init_gyro_z)
    {
      links();
      check_gyro();
      Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));Serial.print("links");Serial.println();
      delay(100);
    }
 

  
  // print out data
 // Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
 // Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
//  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
//  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
//  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
 // Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
 // Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
 // Serial.println();
  
  // delay
  //delay(1000);
}
