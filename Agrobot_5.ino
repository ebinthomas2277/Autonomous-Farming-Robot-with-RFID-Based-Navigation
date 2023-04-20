void rfidMode()
{
  getTagId();
  alreadyPresent();
  if (rfidState == 1)
  {
    movement();
  }
  else
  {
    addNewTag();
    deleteOldTag();
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, LOW);
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, LOW);
  }
}

void getTagId()  // Function to get tag id
{
  String content = "";
  mfrc522.PICC_IsNewCardPresent(); mfrc522.PICC_ReadCardSerial();
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase(); content.replace(" ", "-"); content = content.substring(1);
  if (content == previousContent)
  {
    tagId = "0"; return;
  }
  else
  {
    previousContent = content; tagId = content; return;
  }
}

void alreadyPresent() // Function to find if the tag is already present
{
  for (int i = 0; i < arraySize; i++)
  {
    if ((rightArray[i] == tagId) || (leftArray[i] == tagId) || (flipArray[i] == tagId) || (stopArray[i] == tagId))
    {
      tagAlreadyPresent = "yes"; return;
    }
  }
  tagAlreadyPresent = "no"; return;
}

void addNewTag()
{
  if (tagId != "0" && tagAlreadyPresent == "no" && addTag == 1)
  {
    if ( arrayCounter < arraySize)
    {
      if (tagDirection == "Right") // Right
      {
        rightArray[arrayCounter] = tagId; arrayCounter++; 
        Blynk.notify("Tag Assigned To Turn Right"); Blynk.virtualWrite(V3, 1); return;
      }

      if (tagDirection == "Left") // Left
      {
        leftArray[arrayCounter] = tagId; arrayCounter++;
        Blynk.notify("Tag Assigned To Turn Left"); Blynk.virtualWrite(V3, 1); return;
      }

      if (tagDirection == "Flip") // Flip
      {
        flipArray[arrayCounter] = tagId; arrayCounter++;
        Blynk.notify("Tag Assigned To Flip"); Blynk.virtualWrite(V3, 1); return;
      }

      if (tagDirection == "Stop") // Stop
      {
        stopArray[arrayCounter] = tagId; arrayCounter++;
        Blynk.notify("Tag Assigned To Stop"); Blynk.virtualWrite(V3, 1); return;
      }

      if (tagDirection == "None") // None
      {
        Blynk.notify("Select Direction");
        Blynk.virtualWrite(V3, 1); return;
      }
    }
    Blynk.notify("Check Direction / Limit Exceeded"); return;
  }
  if (tagId != "0" && tagAlreadyPresent == "yes" && addTag == 1)
  {
    Blynk.notify("Tag Already Assigned"); return;
  }
}

void deleteOldTag()
{
  if (tagId != "0" && tagAlreadyPresent == "yes" && deleteTag == 1)
  {
    for (int i = 0; i < arraySize; i++)
    {
      if (rightArray[i] == tagId)
      {
        rightArray[i] = ""; arrayCounter--;
        Blynk.notify("Tag Assigned To Turn Right Deleted"); return;
      }

      if (leftArray[i] == tagId)
      {
        leftArray[i] = ""; arrayCounter--; 
        Blynk.notify("Tag Assigned To Turn Left Deleted"); return;
      }

      if (flipArray[i] == tagId)
      {
        flipArray[i] = ""; arrayCounter--; 
        Blynk.notify("Tag Assigned To Flip Deleted"); return;
      }

      if (stopArray[i] == tagId)
      {
        stopArray[i] = ""; arrayCounter--; 
        Blynk.notify("Tag Assigned To Stop Deleted"); return;
      }
    }
  }
  if (tagId != "0" && tagAlreadyPresent == "no" && deleteTag == 1)
  {
    Blynk.notify("Unassigned Tag can't be deleted !"); return;
  }
}

void movement()
{
  if ((tagAlreadyPresent == "no" || tagId == 0) && deleteTag == 0 && addTag == 0)
  {
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, HIGH);
    digitalWrite(leftMotorA, HIGH);
    digitalWrite(leftMotorB, LOW);
    return;
  }

  if (tagAlreadyPresent == "yes" && deleteTag == 0 && addTag == 0)
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    for (int i = 0; i < arraySize; i++)
    {
      if (tagId == rightArray[i])
      {
        digitalWrite(rightMotorA, HIGH);
        digitalWrite(rightMotorB, LOW);
        digitalWrite(leftMotorA, HIGH);
        digitalWrite(leftMotorB, LOW);
        delay(4000);
        return;
      }
      if (tagId == leftArray[i])
      {
        digitalWrite(rightMotorA, LOW);
        digitalWrite(rightMotorB, HIGH);
        digitalWrite(leftMotorA, LOW);
        digitalWrite(leftMotorB, HIGH);
        delay(4000);
        return;
      }
      if (tagId == flipArray[i]) // right flip
      {
        digitalWrite(rightMotorA, HIGH);
        digitalWrite(rightMotorB, LOW);
        digitalWrite(leftMotorA, HIGH);
        digitalWrite(leftMotorB, LOW);
        delay(8000);
        return;
      }
      if (tagId == stopArray[i])
      {
        digitalWrite(rightMotorA, LOW);
        digitalWrite(rightMotorB, LOW);
        digitalWrite(leftMotorA, LOW);
        digitalWrite(leftMotorB, LOW);
        Blynk.virtualWrite(V10, 0);
        return;
      }
    }
  }
}
