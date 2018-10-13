#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
const char *ssid     = "wifi_name";
const char *password = "wifi_password";
 
const String host = "your_host";

// creating custom rx/tx
SoftwareSerial customSerial(D2, D3);
 
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

const int port = 80;
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
    sendMessage(customSerial.readString());
  }
}

void listenToServer() {
  // Read response from server and send it to arduino serial
  if(client.available()){
    String line = client.readString();
    customSerial.println(line);
  }
}

void verifyCommand(String command){
  
}

void loop() {
  connectToServer(); 
  listenToServer();
  listenToArduino();

    // check for serial input and send to server
  if(Serial.available()){
    String msg = Serial.readString();

    if(msg == "quit"){
      client.stop();
    }
    else if(msg == "connect"){
      isConnected = false;
    }
    else {
      sendMessage(msg); 
    }
  }
}
