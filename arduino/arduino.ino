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

void loop()
{
  if(Serial.available()) 
  {
    newSerial.println(Serial.readString());
  }

  if(newSerial.available()) {
    String msg = newSerial.readString();
    Serial.println("SERVER: " + msg);
  }
  
}
