#include <SoftwareSerial.h>

SoftwareSerial newSerial(3, 2);
int i = 1;

void setup() {
 Serial.begin(9600);
 Serial.println("Started...");
 newSerial.begin(9600);
 delay(1000);
 Serial.println("Data is being sent...");
}

void sendMessage(String msg) {
  newSerial.println(msg);
}

void listenForMessage() {
  if(newSerial.available() > 0) {
    Serial.println(newSerial.readString());
  }
}

void loop() {

   sendMessage("Message " + String(i));
   ++i;
   delay(5000);
   listenForMessage();
}
