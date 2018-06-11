/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <FS.h> // Include the SPIFFS library

const char* ssid = "Le Max2";
const char* password = "123456789";
String inputString = "";         // a String to hold incoming data
String rep = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean start = false;
String cmdUse = "";
String cmdOption = "";
String cmdTemperature = "";
String cmdHumidity = "";
String cmdLight = "";
String cmdVolt = "";
String cmdBtn = "";
String valueLight = "";
int Tmax = 300;
float tabLight[5] = {0};
int indexTab = 0;
char CF = '\n';
float gros = 0;

ESP8266WebServer server(80);
SoftwareSerial swSer(D6, D5, false, 256);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  char temp[500];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 500,

           "<html>\
  <head>\
    <meta charset=\"UTF-8\">\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <ul>\
      <li>Commande utilisé : %s</li>\
      <li>Option : %s</li>\
      <li>Température : %s</li>\
      <li>Humidité : %s</li>\
      <li>Luminosité : %s</li>\
      <li>Volt : %s</li>\
      <li>bouton utilisé : %s</li>\
    </ul>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           cmdUse.c_str(), cmdOption.c_str(), cmdTemperature.c_str(), cmdHumidity.c_str(),
           cmdLight.c_str(),
           cmdVolt.c_str(),
           cmdBtn.c_str()
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleRoot2() {
  digitalWrite(led, 1);
  char temp[500];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 500,

           "<html>\
  <head>\
    <meta charset=\"UTF-8\">\
    <meta http-equiv='refresh' content='1'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <img src=\"/test2.svg\" />\
  </body>\
</html>"
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void aled() {
  int cmd = (server.arg(0) == "1") ? 1 : 0;
  digitalWrite(D2, cmd);
  if (cmd == 1) swSer.write("!G1\n");
  else if (cmd == 0) swSer.write("!G0\n");


}

void all() {
  char temp[800];
  String test2 = rep;
  snprintf(temp, 800,

           "<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <ul>\
      <li>Commande utilisé : %s</li>\
      <li>Option : %s</li>\
      <li>Température : %s</li>\
      <li>Humidité : %s</li>\
      <li>Luminosité : %s</li>\
      <li>Volt : %s</li>\
      <li>bouton utilisé : %s</li>\
    </ul>\
    <p>: %s</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           cmdUse.c_str(), cmdOption.c_str(), cmdTemperature.c_str(), cmdHumidity.c_str(),
           cmdLight.c_str(),
           cmdVolt.c_str(),
           cmdBtn.c_str()
          );
  server.send(200, "text/html", temp);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  swSer.begin(9600);
  delay(200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    swSer.write("!I30\n");
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("cnam")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/testDynamique", handleRoot2);
  server.on("/test.svg", drawGraphLight);
  server.on("/test2.svg", drawGraphLightDynamique);
  server.on("/execute", aled);
  server.on("/testall", all);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  SPIFFS.begin();                           // Start the SPI Flash Files System

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  softSerialEvent();
  if (stringComplete) {
    Serial.println(inputString);
    getValueAll(inputString, ';');
    rep = inputString;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


void drawGraphLight() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int tab[42];
  float gros = 0;
  for (int i = 0; i < 41; i++) {
    softSerialEvent();
    float light = atoi(cmdLight.c_str());
    Serial.println(light);
    float calcul = light;
    tab[i] = calcul;
    gros = (gros < calcul) ? calcul : gros;
    if (stringComplete) {
      Serial.println(inputString);
      getValueAll(inputString, ';');
      rep = inputString;
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
    delay(1000);
  }
  float calcul = tab[0] / gros;
  calcul = 130 * calcul;
  int y = calcul;
  int i = 1;

  for (int x = 10; x < 390; x += 10) {
    float calcul = tab[i] / gros;
    calcul = 130 * calcul;
    int y2 = calcul;
    Serial.println(y2);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
    i++;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

void drawGraphLightDynamique() {
  
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = 130;
  int i = 0;
  softSerialEvent();
      tabLight[indexTab] = atoi(cmdLight.c_str());
      Serial.println(atoi(cmdLight.c_str()));
      Serial.println(indexTab);
      Serial.println(tabLight[indexTab]);
     indexTab++;
     indexTab = (indexTab == 6)?0:indexTab;
  for (int x = 10; x < 390; x += 80) {
    int y2 = 0;
    gros = (gros < tabLight[i])?tabLight[i]:gros;
    if(tabLight[i]>0){
    float calcul = tabLight[i] / gros;
    calcul = 130 * calcul;
    y2 = calcul;
    }else{
      y2 = 0;
    }
    Serial.println(y2);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 80, 140 - y2);
    out += temp;
    y = y2;
    i++;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}


void softSerialEvent() {
  while (swSer.available()) {
    char inChar = (char)swSer.read();
    // add it to the inputString:
    if (inputString.length() <= Tmax) {
      if (inChar == CF) {
        stringComplete = true;
      } else
        inputString += inChar;
    }

  }
}


void getValueAll(String data, char separator)
{
  int found = 0;
  int cpt = 0;
  String value = "";
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      if (cpt == 0)cmdUse = value;
      if (cpt == 1)cmdOption = value;
      if (cpt == 2)cmdTemperature = value;
      if (cpt == 3)cmdHumidity = value;
      if (cpt == 4)cmdLight = value;
      if (cpt == 5)cmdVolt = value;
      if (cpt == 6)cmdBtn = value;
      value = "";
      cpt++;
    }
    else {
      value += data.charAt(i);
    }
  }
}

void getValueLight(String data, char separator)
{
  Serial.println(data);
  int found = 0;
  int cpt = 0;
  String value = "";
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      if (cpt == 1)valueLight = value;
      value = "";
      cpt++;
    }
    else {
      value += data.charAt(i);
    }
  }
  Serial.println(valueLight);
}



String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
