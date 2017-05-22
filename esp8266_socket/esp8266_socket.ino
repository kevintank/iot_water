#include <ESP8266WiFi.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       

WiFiServer server(4998);  //서버의 포트
WiFiClient client;

char data[1500];  //버퍼
int ind = 0;
int type;
short int data_size ;   //data size
const int led_pin = 2; //출력핀 선언
const int water = 0    //수분 센서 
 
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

  //아이피 셋팅
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

 //공유기에 접속
  if (!wifiManager.autoConnect("kevin", "")) {
    Serial.println("연결실패, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

 //ip확인용
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
 
 //if you get here you have connected to the WiFi
 Serial.println("Connection established");

//서버 시작
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

  //밀리초 구하기
  unsigned long timeout = millis();
  //while (client.available() == 0) {
  ///  if (millis() - timeout > 5000) {
    //  Serial.println(">>> Client Timeout !");
  //    client.stop();
    //  return;
  //  }
 // }
  
    if(client.available() > 0)
    { 
      //입력 데이타 취득
      while(client.available())
      {
         data[ind] = client.read();
        ind++;  // 입력받은 바이트수
      } 
      client.flush();
      
     // 클라이언트에서 보낸 정수값 체크 하기
     data_size =  ((data[1] << 8) & 0xff00 ) + (data[0] & 0x00ff);

       //물관련 명령
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
      
      ind = 0;

      //클라이언트에 문자 전송
      client.print("OK!");    //send ascii code  
    }

  }  
}  //end loop
