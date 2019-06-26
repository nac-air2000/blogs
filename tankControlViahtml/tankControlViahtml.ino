#include <WiFi.h>

const char ssid[]="SSID";
const char passwd[] ="PASSWORD";
const char html[]=
"<!DOCTYPE html>\
<html>\
<head>\
<meta http-equiv=”refresh” content=0.1>\
<title>\
Tank control test\
</title>\
</head>\
<body>\
<h1>\
Tank control test\
</h1>\
<p>\
</p>\
<form method='get'>\
<input type='submit' name='stop' value='Stop' />\
<input type='submit' name='go_ahead' value='Go Ahead' />\
<input type='submit' name='turn_left' value='Turn Left' />\
<input type='submit' name='turn_right' value='Turn Right' />\
<input type='submit' name='back' value='Back' />\
</form>\
</body>\
</html>";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  connectWiFi();

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( WiFi.status() == WL_DISCONNECTED ) {
        connectWiFi();
  }
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        if(c == '\n'){
          if (currentLine.length() == 0){
            //getSoundVolume();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            client.println(html);
            client.println();
            break;
          }else{
            currentLine = "";
          }
        }else if(c != '\r'){
          currentLine += c;
        }
        if(currentLine.endsWith("GET /?stop")){
          digitalWrite(16, HIGH);
          digitalWrite(17, HIGH);
          digitalWrite(18, HIGH);
          digitalWrite(19, HIGH);
        }
        if(currentLine.endsWith("GET /?go_ahead")){
          digitalWrite(16, LOW);
          digitalWrite(17, HIGH);
          digitalWrite(18, HIGH);
          digitalWrite(19, LOW);
        }
        if(currentLine.endsWith("GET /?turn_left")){
          digitalWrite(16, HIGH);
          digitalWrite(17, LOW);
          digitalWrite(18, HIGH);
          digitalWrite(19, LOW);
        }
        if(currentLine.endsWith("GET /?turn_right")){
          digitalWrite(16, LOW);
          digitalWrite(17, HIGH);
          digitalWrite(18, LOW);
          digitalWrite(19, HIGH);
        }
        if(currentLine.endsWith("GET /?back")){
          digitalWrite(16, HIGH);
          digitalWrite(17, LOW);
          digitalWrite(18, LOW);
          digitalWrite(19, HIGH);
        }

      }
    }
    client.stop();
    Serial.println("client disonnected");
  }  
}

void connectWiFi() {
  WiFi.begin(ssid, passwd);
  Serial.print("WiFi connecting...");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print(" connected. ");
  Serial.println(WiFi.localIP());
}
