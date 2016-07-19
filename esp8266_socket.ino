#include <FS.h>

#include <ESP8266WiFi.h>           

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       

WiFiServer server(4998);
WiFiClient client;

char data[1500];
int ind = 0;
int type;
short int data_size ; //data size

#define serialWaitChars(n) while(Serial.available() < n){delay(10);}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
  //  wifiManager.setAPConfig(IPAddress(192,168,0,55), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  //고정 아이피 지정

  IPAddress _ip = IPAddress(192,168,0,55);
  IPAddress _gw = IPAddress(192,168,0,1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
  
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    //wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
   // wifiManager.autoConnect();

  if (!wifiManager.autoConnect("kevin", "")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  
    
    //if you get here you have connected to the WiFi
    Serial.println("Connection established");

    server.begin();
    Serial.println("Server started");
    Serial.setDebugOutput(true);
}

void loop() {
    // put your main code here, to run repeatedly:
  if(!client.connected())
  {
    //try to connect to a new client
    client = server.available();
  }
  else
  {
    if(client.available() > 0)
    { 
      while(client.available())
      {
         data[ind] = client.read();
        ind++;
      } 
      client.flush();
       
       data_size =  ((data[1] << 8) & 0xff00 ) + (data[0] & 0x00ff);

     
       Serial.println("result:");
       Serial.println(data_size);
       Serial.print("\n");
       
       Serial.println("type:");
       Serial.println(data[2]);
       Serial.print("\n");

        Serial.println("cmd:");
       Serial.println(data[3]);
       Serial.print("\n");
       
      for(int j=0;j < ind; j++)
     {
       Serial.print(data[j]);
     }
      ind = 0;
      client.print("OK!");    //send ascii code  
    }

  }
} 
