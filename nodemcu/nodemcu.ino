#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiUdp.h>

const char *ssid = "UPM-Student";
const char *password = "STud123!?";

const String host = "192.168.0.105";

// creating custom rx/tx
SoftwareSerial customSerial(D7, D8);

bool _connectedToServer = false;
bool _listeningToUdp = false;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacketBuffer[12];

const int port = 6655;
WiFiClient client;

const int bufferLength = 1024;
byte* serverReadBuffer = new byte[bufferLength];

void setup() {

  Serial.begin(9600);
  customSerial.begin(9600);

  Serial.setTimeout(50);
  customSerial.setTimeout(50);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}

bool listenForServer(IPAddress& ipAddress)
{
  if (_connectedToServer == false)
  {
    if (_listeningToUdp == false)
    {
      Udp.begin(localUdpPort);
      Serial.print("Listening for server packet!");
      _listeningToUdp = true;
    }

    int packetSize = Udp.parsePacket();

    if (packetSize)
    {
      int len = Udp.read(incomingPacketBuffer, 12);
      if (len == 3)
      {
        if (incomingPacketBuffer[0] == 0 && incomingPacketBuffer[1] == 1 && incomingPacketBuffer[2] == 2)//our server package
        {
          ipAddress = Udp.remoteIP();
          return true;
        }
      }
    }
    return false;
  }
  else
  {
    if (_listeningToUdp == true)
    {
      Udp.begin(localUdpPort);
      Serial.print("Stopped listening for server packet!");
      Udp.stop();
      _listeningToUdp = false;
    }
    return true;
  }

}

void loop() {

  IPAddress ipAddress;
  bool foundServerIp = listenForServer(ipAddress);

  if (foundServerIp)
  {
    if (_connectedToServer == false)
    {
      Serial.printf("Found Server Ip:%s, connecting!", ipAddress.toString().c_str());
      
      if (client.connect(ipAddress, port) == false)
      {
        Serial.println("connection failed!");
      }
      else
      {
        Serial.println("connection succeded!");
        _connectedToServer = true;
      }

    }
    else
    {
      if (client.connected() == true)
      {

        if (customSerial.available()) {
          String msg = customSerial.readString();
          if (_connectedToServer) {
            client.print(msg);
          }
        }

        if (client.available() > 0) {
          int count = client.read(serverReadBuffer, bufferLength);
          customSerial.write(serverReadBuffer, count);
        }
      }
      else
      {
        _connectedToServer = false;
        Serial.println("Disconnected from server!");
      }
    }

  }

}
