#include <Arduino.h>
#include <ESP8266WiFi.h>

// ssid dan password
const char* ssid     = "Alfyan_yu";
const char* password = "what are you say";

// Server port 80
WiFiServer server(80);


String header;


String output4State = "off";
String output1      = "off";
String output2      = "off";
String output3      = "off";



const int output4 = D4;
const int led1    = 5;
const int led2    = 14;
const int led3    = 12;


unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(output4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  digitalWrite(output4, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                   
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            //  on dan off GPIO 4
              if (header.indexOf("GET /4/off") >= 0) {
               Serial.println("LAMPU KAMAR off");
                output4State = "off";
                digitalWrite(output4, HIGH);
              } else if (header.indexOf("GET /4/on") >= 0) {
                Serial.println("LAMPU KAMAR on");
                output4State = "on";
                digitalWrite(output4, LOW);
                // on dan off GPIO 5
              } else if (header.indexOf("GET /1/off") >= 0) { 
               Serial.println("LAMPU R TAMU off");
               output1 = "off";
               digitalWrite(led1, LOW);
              } else if (header.indexOf("GET /1/on") >= 0) {
               Serial.println("LAMPU R TAMU on");
               output1 = "on";
               digitalWrite(led1, HIGH);
               // on dan off GPIO 14
              } else if (header.indexOf("GET /2/off") >= 0) { 
               Serial.println("LAMPU TERAS off");
               output2 = "off";
               digitalWrite(led2, HIGH);
              } else if (header.indexOf("GET /2/on") >= 0) {
               Serial.println("LAMPU TERAS on");
               output2 = "on";
               digitalWrite(led2, LOW);
               // oon dan off GPIO 12
              } else if (header.indexOf("GET /3/off") >= 0) { 
               Serial.println("LAMPU TIDUR off");
               output3 = "off";
               digitalWrite(led3, HIGH);
              } else if (header.indexOf("GET /3/on") >= 0) {
               Serial.println("LAMPU TIDUR on");
               output3 = "on";
               digitalWrite(led3, LOW);
              }

            // Tampilan di web
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Button CSS
            const char *style = "<style>"
                                "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
                                ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"
                                "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"
                                ".button-off {background-color: #77878A;}"
                                "</style></head>";
            client.println(style);

            client.println("<body><h1>Selamat Darang di Server ESP8266</h1>");
            
               
             
            client.println("<p> LAMPU KAMAR " + output4State + "</p>");     
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button button-off\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button\">ON</button></a></p>");
            }
            //set button io 5
            client.println("<p> LAMPU R TAMU " + output1 + "</p>"); 
             if (output1 =="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button button-off\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button\">ON</button></a></p>");
            }
            //set button io 14
            client.println("<p> LAMPU TERAS " + output2 + "</p>"); 
             if (output2 =="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button button-off\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button\">ON</button></a></p>");
            }
            //set button io 12
            client.println("<p> LAMPU TIDUR " + output3 + "</p>"); 
             if (output3 =="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button button-off\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button\">ON</button></a></p>");
            }


            client.println("</body></html>");
            
            client.println();
          
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;     
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}