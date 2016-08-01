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
const int led_pin = 2;

#define serialWaitChars(n) while(Serial.available() < n){delay(10);}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH);   //on 
    
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();

    wifiManager.setTimeout(180);
    
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

  if (!wifiManager.autoConnect("kevin", "arami2005")) {
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
    if(!client){
      return;
    }
  }
  else
  {

   unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
    if(client.available() > 0)
    { 
      while(client.available())
      {
         data[ind] = client.read();
        ind++;
      } 
      client.flush();
      
     // if(data[1] != NULL && data[0] !=NULL)
     //    { 
         data_size =  ((data[1] << 8) & 0xff00 ) + (data[0] & 0x00ff);
       
        //여기서 on off를 감지한다.
         //Serial.println("result:");
        // Serial.println(data_size);
         //Serial.print("\n");
       
         //Serial.println("type:");
        // Serial.println(data[2]);
       
       if(data[2] == 'w'){
             if(data[3] == 'o'){
                   digitalWrite(led_pin, HIGH);   //on 
                    Serial.print("on");
                    Serial.print("\n");
             }else if(data[3] == 'f'){
                    digitalWrite(led_pin, LOW);  //off
                    Serial.print("off");
                    Serial.print("\n");
             }else if(data[3]== 's'){
                                                 //get status
             }else if(data[3] == 't'){

                                                //get 습도
             }
       }
       
     // }

   //   for(int j=0;j < ind; j++)
    // {
     //  Serial.print(data[j]);
    // }
      ind = 0;
      client.print("OK!");    //send ascii code  
    }//end if

  } //if 
}  //end loop
