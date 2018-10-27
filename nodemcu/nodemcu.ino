#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
const char *ssid     = "WTF?";
const char *password = "Depakine13579";

const String host = "192.168.0.105";

// creating custom rx/tx
SoftwareSerial customSerial(D7, D8);
 
void setup() {
  Serial.begin(9600);
  customSerial.begin(9600);
  delay(100);
 
  // Connect to WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

const int port = 6655;
bool isConnected = false;
WiFiClient client;

void connectToServer() {
  if(!isConnected) {
     Serial.println(host);
  
    // Try to connect to @host
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }

    Serial.print("Connected to ");
    Serial.println(host + ":" + port);
    isConnected = true;
  }
}


void sendMessage(String msg) {
  if(isConnected){
    client.print(msg);
  }
}

void listenToArduino() {
  // check for arduino serial input and send it to the server
  if(customSerial.available()) {
    String msg = customSerial.readString();
    Serial.println("ARDUINO: " + msg);
    sendMessage(msg);
  }
}

void listenToServer() {
  // Read response from server and send it to arduino serial
  if(client.available()){
    String msg = client.readString();
    customSerial.println(msg);
    Serial.println("SERVER: " + msg);
  }
}

//void sendMessage(byte *data) {
//  int dataLenght = sizeof(data);
//  byte *totalBytes = (byte*) malloc((dataLenght * sizeof(byte)) + 4);
//  totalBytes[0] = (byte) dataLenght;
//  totalBytes[1] = (byte) (dataLenght >> 8);
//  totalBytes[2] = (byte) (dataLenght >> 16);
//  totalBytes[3] = (byte) (dataLenght >> 24);
//
//  for(int i = 0; i < dataLenght; ++i) {
//    totalBytes[i + 4] = data[i];
//  }
//
//  Serial.println("TO_SEND: " + String(sizeof(totalBytes) + 4));
//  if(isConnected) {
//    client.write(totalBytes, sizeof(totalBytes) + 4);
//  }
//}

void verifyCommand(String command){
  
}

void loop() {
  connectToServer(); 
  listenToServer();
  listenToArduino();
//  if(Serial.available()) {
//    String msg = Serial.readString();
//    Serial.println("MSG: " + msg);
//    Serial.println("BYTES: " + String(sizeof(msg)));
//    sendMessage(msg);
//  }
}
