#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiUdp.h>

//const char *ssid = "UPM-Student";
//const char *password = "STud123!?";

const char *ssid = "RomTelecom-WEP-0232";
const char *password = "277TCTVZCA5LU";

const String host = "192.168.0.105";

#define RXPin D7
#define TXPin D8

// creating custom rx/tx
SoftwareSerial customSerial(RXPin, TXPin);

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

  pinMode(RXPin, INPUT);
  pinMode(TXPin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.print("Connected to wifi!");
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
          Serial.print("arduino->server  ");
          int count = customSerial.readBytes(serverReadBuffer, bufferLength);
          Serial.print(count);
          Serial.print(" bytes \n");

          if (_connectedToServer) {
            Serial.print(" Sending \n");
            client.write(serverReadBuffer, count);
          }
          else
          {
            Serial.print(" Not connected to server! \n");
          }
        }

        if (client.available() > 0) {
          Serial.print("server->arduino ");
          int count = client.read(serverReadBuffer, bufferLength);
          Serial.print(count);
          Serial.print(" bytes \n");
          //customSerial.write(serverReadBuffer, count);
          for (int i = 0; i < count; i++)
            Serial.print(serverReadBuffer[i]);
          Serial.print("\n");

          if(serverReadBuffer[0]==3)
              {
                client.write(serverReadBuffer,1);
              }
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
