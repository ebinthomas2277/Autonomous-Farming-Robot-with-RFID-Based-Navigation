void setup()
{
  SPI.begin();
  dht.begin();
  Serial.begin(baudRate);
  mfrc522.PCD_Init();
  Blynk.begin(auth, ssid, pass);

  Blynk.virtualWrite(V0, 0); // Weed Cutting
  Blynk.virtualWrite(V1, 0); // Lights
  Blynk.virtualWrite(V2, 0); // Plough
  Blynk.virtualWrite(V3, 0); // Battery Level
  Blynk.virtualWrite(V5, 0); // Water Level
  Blynk.virtualWrite(V6, 0); // Spraying
  Blynk.virtualWrite(V7, 0); // Mixer
  Blynk.virtualWrite(V8, 0); // Moisture Servo
  Blynk.virtualWrite(V9, 0); // Drive Mode
  Blynk.virtualWrite(V10, 0); // Tag Activate
  Blynk.virtualWrite(V11, 0); // Assign Direction
  Blynk.virtualWrite(V12, 0); // Add Tag
  Blynk.virtualWrite(V13, 0); // Delete Tag
  Blynk.virtualWrite(V14, 0); // Temperature
  Blynk.virtualWrite(V15, 0); // Humidity
  Blynk.virtualWrite(V16, 0); // Moisture Level
  Blynk.virtualWrite(V17, 0); // Gas Reading

  pinMode(weedCuttingMotor, OUTPUT);

  pinMode(weedCuttingServoA, OUTPUT);
  pinMode(weedCuttingServoB, OUTPUT);

  pinMode(submersibbleMotorA, OUTPUT);
  pinMode(submersibbleMotorB, OUTPUT);

  pinMode(rightMotorA, OUTPUT);
  pinMode(rightMotorB, OUTPUT);
  pinMode(leftMotorA, OUTPUT);
  pinMode(leftMotorB, OUTPUT);

  pinMode(lights, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(dhtSensor, INPUT);
  pinMode(moistureSensor, INPUT);
  pinMode(waterLevelSensor, INPUT);
  pinMode(mq2Sensor, INPUT);
  pinMode(batteryLevel, INPUT);

  digitalWrite(weedCuttingMotor, LOW);
  digitalWrite(submersibbleMotorA, LOW);
  digitalWrite(submersibbleMotorB, LOW);

  digitalWrite(rightMotorA, LOW);
  digitalWrite(rightMotorB, LOW);
  digitalWrite(leftMotorA, LOW);
  digitalWrite(leftMotorB, LOW);
 
  myServo1.attach(weedCuttingServoA);
  myServo2.attach(weedCuttingServoB);
  myServo3.attach(moistureServo);
  myServo4.attach(ploughServo);

  myServo3.write(35);
  delay(2000);
  myServo2.write(180);
  delay(2000);
  myServo1.write(0);
  delay(2000);
  myServo4.write(175);

  timer.setInterval(250L, getDht11);
  timer.setInterval(250L, getMq2);
  timer.setInterval(250L, getwaterLevel);
  timer.setInterval(250L, batteryHealth);
}
