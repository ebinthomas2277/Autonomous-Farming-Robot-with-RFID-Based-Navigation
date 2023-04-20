  
#include "DHT.h"
#include <SPI.h>
#include <WiFi.h>
#include <Servo.h> 
#include <MFRC522.h>

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define weedCuttingMotor 2      // Weed cutting motor
#define weedCuttingServoA 16   // Weed cutting servo right
#define weedCuttingServoB 17 // Weed cutting servo left

#define submersibbleMotorA 4  // Submersible motor main tank
#define submersibbleMotorB 5 // Submersible motor sub tank

#define rightMotorA 13    // Gear motor R1
#define rightMotorB 12   // Gear motor R2 
#define leftMotorA  14  // Gear motor L1
#define leftMotorB  15 // Gear motor L2  

#define SDA 21       // Rfid sck 18, miso 19, mosi 23
#define RST 22      // Rfid

#define moistureServo 25 // Soil moisture sensor servo
#define ploughServo 26  // Ploughing arm servo
#define lights 27       // Led lights
#define dhtSensor 32  // Dht sensor
#define buzzer 33    // buzzer gpio

// Analog
#define moistureSensor 34     // soil moisture sensor
#define waterLevelSensor 35  // water level sensor
#define mq2Sensor 36        // Mq2 smoke sensor
#define batteryLevel 39    // Battery level indicator
#define baudRate 9600

// Blynk Config
const char ssid[] = "Galaxy S10e";
const char pass[] = "rxap8239";
const char auth[] = "bhrY-5HW5jkuR_dBkNDCQ57vzrPK6z2l";

//************RFID**********
int arrayCounter = 0;
const int arraySize = 10;

byte addTag;
byte driveMode;
byte deleteTag;
byte rfidState;

String tagId;
String tagDirection;
String previousContent;
String tagAlreadyPresent;

String leftArray  [arraySize];
String flipArray  [arraySize];
String stopArray  [arraySize];
String rightArray [arraySize];
//**********RFID***********

int mq2Value;
int batteryValue;
int waterLevelValue;
static int moistureValue = 2;

float humidity;
float heatIndex;
float temperature;
String manualMotion;

BlynkTimer timer;
MFRC522 mfrc522(SDA, RST);
DHT dht(dhtSensor, DHT11);

Servo myServo1; // weed cutting servo right
Servo myServo2; // weed cutting servo left
Servo myServo3; // soil moisture sensor servo
Servo myServo4; // plought servo
//******************************************************
BLYNK_WRITE(V0)
{
  byte weedCuttingStatus = param.asInt();
  if (weedCuttingStatus == 1)
  {
    digitalWrite(buzzer, HIGH);
    Blynk.virtualWrite(V8, 0);
    myServo3.write(35);
    delay(2000);
    digitalWrite(buzzer, LOW);
    myServo1.write(123);
    delay(2000);
    myServo2.write(15);
    delay(2000);
    digitalWrite(weedCuttingMotor, HIGH);
  }
  else
  {
    digitalWrite(buzzer, HIGH);
    Blynk.virtualWrite(V8, 0);
    myServo3.write(35);
    delay(2000);
    digitalWrite(buzzer, LOW);
    digitalWrite(weedCuttingMotor, LOW);
    delay(5000);
    myServo2.write(180);
    delay(2000);
    myServo1.write(0);
  }
}

BLYNK_WRITE(V1)
{
  byte ledStatus = param.asInt();
  if (ledStatus == 1)
  {
    digitalWrite(lights, HIGH); // turn on lights
  }
  else
  {
    digitalWrite(lights, LOW); // turn off lights
  }
}

BLYNK_WRITE(V2)
{
  byte ploughStatus = param.asInt();
  if (ploughStatus == 1)
  {
    for (int i = 175; i >= 90; i--)
    {
      myServo4.write(i);
      delay(5);
    }
  }
  else
  {
    for (int i = 90; i <= 175; i++)
    {
      myServo4.write(i);
      delay(5);
    }
  }
}

BLYNK_WRITE(V4)
{
  int stickX = param[0].asInt();
  int stickY = param[1].asInt();

  if (stickX >= 200 && stickX <= 900 && (stickY >= 900))
  {
    manualMotion = "Forward";
  }

  else if (stickX >= 200 && stickX <= 900 && (stickY <= 200))
  {
    manualMotion = "Backward";
  }
  else if (stickY >= 200 && stickY <= 900 && (stickX >= 900))
  {
    manualMotion = "Right";
  }
  else if (stickY >= 200 && stickY <= 900 && (stickX <= 200))
  {
    manualMotion = "Left";
  }
  else
  {
    manualMotion = "Stop";
  }
}

BLYNK_WRITE(V6)
{
  byte sprayingStatus = param.asInt(); // sub tank mixer
  if (sprayingStatus == 1)
  {
    digitalWrite(submersibbleMotorA, HIGH);
  }
  else
  {
    digitalWrite(submersibbleMotorA, LOW);
  }
}

BLYNK_WRITE(V7)
{
  byte mixerStatus = param.asInt(); // sub tank mixer
  static int mixedCount = 0;
  if (mixedCount != 100)
  {
    switch (mixerStatus)
    {
      case 25: digitalWrite(submersibbleMotorB, HIGH); delay(1500); digitalWrite(submersibbleMotorB, LOW); mixedCount += 25; break;
      case 50: digitalWrite(submersibbleMotorB, HIGH); delay(1500); digitalWrite(submersibbleMotorB, LOW); mixedCount += 25; break;
      case 75: digitalWrite(submersibbleMotorB, HIGH); delay(1500); digitalWrite(submersibbleMotorB, LOW); mixedCount += 25; break;
      case 100: digitalWrite(submersibbleMotorB, HIGH); delay(1500); digitalWrite(submersibbleMotorB, LOW); mixedCount += 25; break;
    }
  }
  else
  {
    Blynk.notify("Mixer Tank Empty");
  }
}

BLYNK_WRITE(V8) // Moisture Servo
{
  byte moistureStatus = param.asInt(); // sub tank mixer
  if (moistureStatus == 1)
  {
    myServo1.write(123);
    delay(2000);
    myServo2.write(15);
    delay(2000);
    myServo3.write(110);
    getMoistureValue();
  }
  else
  {
    myServo3.write(35);
    delay(2000);
    myServo2.write(180);
    delay(2000);
    myServo1.write(0);
  }
}

BLYNK_WRITE(V9)
{
  driveMode = param.asInt();
}
BLYNK_WRITE(V12)
{
  addTag = param.asInt();
}
BLYNK_WRITE(V11)
{
  switch (param.asInt())
  {
    case 2:  tagDirection = "Right";  break;
    case 3:  tagDirection = "Left" ;  break;
    case 4:  tagDirection = "Flip" ;  break;
    case 5:  tagDirection = "Stop" ;  break;
    default: tagDirection = "None" ; break;
  }
}
BLYNK_WRITE(V13)
{
  deleteTag = param.asInt();
}

BLYNK_WRITE(V10)
{
  rfidState = param.asInt();
}
