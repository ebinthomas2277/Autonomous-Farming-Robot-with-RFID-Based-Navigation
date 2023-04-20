void manualMode()
{
  if (manualMotion == "Forward") // forward
  {
    digitalWrite(rightMotorA, LOW); //right LH
    digitalWrite(rightMotorB, HIGH);
    digitalWrite(leftMotorA, HIGH); // left HL
    digitalWrite(leftMotorB, LOW);
  }

  if (manualMotion == "Backward") // backward
  {
    digitalWrite(rightMotorA, HIGH);
    digitalWrite(rightMotorB, LOW);
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, HIGH);
  }

  if (manualMotion == "Right") // right
  {
    digitalWrite(rightMotorA, HIGH);
    digitalWrite(rightMotorB, LOW);
    digitalWrite(leftMotorA, HIGH);
    digitalWrite(leftMotorB, LOW);
  }

  if (manualMotion == "Left") // left
  {
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, HIGH);
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, HIGH);
  }

  if (manualMotion == "Stop") // stop
  {
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, LOW);
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, LOW);
  }
}
