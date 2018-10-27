#include <SoftwareSerial.h>

SoftwareSerial newSerial(8, 7);
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
  Serial.println("ARDUINO: " + msg);
}

void listenForMessage() {
  if(newSerial.available()) {
    String msg = newSerial.readString();
    Serial.println("SERVER: " + msg);
  }
}

void loop() {
   sendMessage("Test Message " + String(i));
   listenForMessage();
   ++i;
   delay(10000);
}
