void getDht11()
{
  temperature = 33;
  humidity = 52;
  
//  Serial.print(temperature);
//  if (isnan(temperature) || isnan(humidity))
//  {
//    temperature = 0;
//    humidity = 0;
//    Blynk.virtualWrite(V14, temperature);
//    Blynk.virtualWrite(V15, humidity);
//    return ;
//  }
  Blynk.virtualWrite(V14, temperature);
  Blynk.virtualWrite(V15, humidity);
}

void getMoistureValue()
{
  delay(20000);
  moistureValue = map(analogRead(moistureSensor), 3650, 1300, 0, 100);
  if (isnan(moistureValue))
  {
    moistureValue = 0;
    Blynk.virtualWrite(V16 , moistureValue);
    Blynk.notify(moistureValue);
    return ;
  }
  Blynk.notify(moistureValue);
  Blynk.virtualWrite(V16, moistureValue);
}

void getMq2()
{
  mq2Value = map(analogRead(mq2Sensor), 1200 , 3000, 0, 100);
  Serial.println(analogRead(mq2Sensor));
  if (isnan(mq2Value))
  {
    mq2Value = 0;
    Blynk.virtualWrite(V17 , mq2Value);
    return ;
  }
  if (mq2Value >= 60)
  {
    Blynk.notify("Heavy Smoke Detected");
    digitalWrite(buzzer, HIGH);
  }
  Blynk.virtualWrite(V17 , mq2Value);
  digitalWrite(buzzer, LOW);

}

void getwaterLevel()
{
  waterLevelValue = map(analogRead(waterLevelSensor), 800, 2200, 0, 100);
  if (isnan(waterLevelValue))
  {
    waterLevelValue = 0;
    Blynk.virtualWrite(V5, waterLevelValue);
  }
  Blynk.virtualWrite(V5, waterLevelValue);
  if (waterLevelValue <= 1000)
  {
    waterLevelValue = 0;
    Blynk.virtualWrite(V5, waterLevelValue);
    digitalWrite(submersibbleMotorA, LOW);
    return ;
  }
}

void batteryHealth()
{
  batteryValue = map(analogRead(batteryLevel), 0, 4096, 0, 100);
  Blynk.virtualWrite(V3, batteryValue);
}
