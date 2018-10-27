#include <SoftwareSerial.h>

SoftwareSerial newSerial(8, 7);

void setup() {
  Serial.begin(9600);
  Serial.println("Waiting for pc");
  while (!Serial) {
    delay(10);
  }
  Serial.println("Pc connected");


  Serial.println("Waiting for wifi");
  newSerial.begin(9600);
  while (!newSerial) {
    delay(10);
  }


  Serial.println("Wifi connected");
}

const int bufferLength = 1024;
byte* packetBuffer = new byte[bufferLength];

void interpretGoToMessage()
{
  byte targetPlant = packetBuffer[1];
  Serial.println("Going");
}

void interpretIncomingPackage()
{
  if (newSerial.available()) {
    int packetSize = newSerial.readBytes(packetBuffer, bufferLength);

    byte command = packetBuffer[0];

    Serial.print("Command: ");
    Serial.println(command);
    
    if (command == 1)
    {
      byte targetPlant = packetBuffer[1];
      Serial.println("Going");
    }
    else if (command == 2)
    {
      for (int i = 0; i < 6; i++)
      {
        word angle = word(packetBuffer[i + 1]);
        Serial.print("Going");
        Serial.println(angle);
      }
    }
  }
}

void loop()
{
  if (Serial.available())
  {
    newSerial.println(Serial.readString());
  }

  interpretIncomingPackage();
}
