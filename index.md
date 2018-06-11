# TP ARDUINO - CNAM:STMN 2018

- TP2 : Mise en œuvre de capteurs - Dialogues sur la liaison série 
- TP3 : Objet Connecté

## Réalisé par Dylan Raimon, Dylan Antonio et David MOLINARI


  
  
<img src="https://i.imgur.com/dFmzsmO.jpg" />


# TP2

[Code TP2](./TPFULL/TPFULL.ino)

###  1) Stocker les données reçues et détecter la fin d'une commande

```cpp

#define TMAX  300
#define CF '\n'
#define CASSE 0

....

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
   
    // add it to the inputString:
    if(inputString.length() <= Tmax){
      if (inChar == CF) {
        stringComplete = true;
      }else
         inputString += inChar;
    }

  }
}
```


###  2) Implémenter les premières commandes

```cpp
String parseCommandLine(String s){
  if(s[0] == '!'){
    switch (s[1]){
      case 'R':
        if(s[2] == '1')digitalWrite(10, HIGH);else if(s[2]=='0')digitalWrite(10, LOW);else return "?";
        aze = "R;";
        res = s[2];
        break;
      case 'G':
        if(s[2] == '1')digitalWrite(6, HIGH);else if(s[2]=='0')digitalWrite(6, LOW);else return "?";
        aze = "G;";
        res = s[2];
        break;
       case 'B':
        aze = "B;";
        res = s[2];
        if(s[2] == '1')digitalWrite(5, HIGH);else if(s[2]=='0')digitalWrite(5, LOW);else return "?";
        break;  
    }
    rty = ";ok";
    return aze+res+rty;
  }else return "?";
 }
```


###  3) Lecture température et humidité

```cpp

  humidity = (float)(dht.humidity);
  temperature = (float)(dht.temperature); 
    
....

case 'T':
        aze = "T;";
        res = temperature;
        break;
case 'H':
        aze = "H;";
        res = humidity;
        break;    


```

###  4)  Lecture de la luminosité

```cpp

case 'L':
        aze = "L;";
        light = sensor.getRawLight(); 
        res = light;
        break;

```

###  5)  Éclairage automatique

```cpp
void switchLight() {
  light = sensor.getRawLight();
  if(light > (SEUIL+20)){
    unsigned long currentMillis4 = millis();
    if (currentMillis4 - previousMillisLux >= intervalLux) {
    previousMillisLux = currentMillis4;
    digitalWrite(2, LOW);   // JOUR
    }
} else if(light <(SEUIL-20)) {
  unsigned long currentMillis4 = millis();
    if (currentMillis4 - previousMillisLux >= intervalLux) {
    previousMillisLux = currentMillis4;
  digitalWrite(2, HIGH);// NUIT
    }
}

....


}
```

###  6)   Lire la tension d'alimentation

```cpp
int16_t readPwrVoltage(){
  sensorValue = analogRead(0);
  sensorValue = sensorValue*(3300/1024);
  return sensorValue;
}

....

case 'V':
        aze = "V;";
        volt = readPwrVoltage(); 
        res = volt;
        break;

```

###  7)   Buttons "analogiques"

```cpp

int8_t readButtons(int s){
  if(s!=0 || oldInput != 0){
    if(oldInput != s && oldInput != s+1 && oldInput != s-1){
      oldInput = s;
  if (s == 138) {
    digitalWrite(10, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    }
      if (s == 204){
        digitalWrite(6, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(5, LOW);
    }
      if (s == 369){
       digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(10, LOW);
    }
    aze = "#B;";
    res = (s<375 && s>365)?1
    :(s<210 && s>200)?2
    :(s<150 && s>120)?4
    :(s<470 && s>450)?3
    :(s<300 && s>290)?6
    :(s<435 && s>420)?5
    :(s<510 && s>500)?7
    :0;
    
    rty = ";ok";
    Serial.println(aze+res+rty);
    }
  }

 }

```

###  8) Trame d'information périodique

```cpp

 case 'I':
        if(s[2] == '3' && s[3] == '0'){
          activeI = true;
        }else if(s[2]=='0'){
          aze = "I;";
          activeI = false;
          n = analogRead(1);
          light = sensor.getRawLight();
          volt = readPwrVoltage();
          btn = readbuttons(n);
          res = s[2]+separateur+temperature+separateur+humidity+separateur+light+separateur+volt+separateur+btn;
        }else return "?";
        break;
....

 if (currentMillis3 - previousMillisI >= intervalI && activeI) {
      previousMillisI = currentMillis3;
          
          n = analogRead(1);
          light = sensor.getRawLight();
          volt = readPwrVoltage();
          btn = readbuttons(n);
          res = temperature+separateur+humidity+separateur+light+separateur+volt+separateur+btn;
          rty = ";ok";
          aze = "#I;30;";
          Serial.println(aze+res+rty);
    }
```

###  9) Faire varier la couleur de la led RGB

```cpp
case 'C':
        aze = "C;";
        char * p;
        long r = strtol( s.substring(3,5).c_str(), & p, 16 );
        long g = strtol( s.substring(6,8).c_str(), & p, 16 );
        long b = strtol( s.substring(9,11).c_str(), & p, 16 );
        analogWrite(10,r);
        analogWrite(6,g);
        analogWrite(5,b);
        res = s.substring(3,5)+";"+s.substring(6,8)+";"+s.substring(9,11);
        break;

```



# TP3

[Code TP3](./WebServerTP/WebServerTP.ino)

###  1.1) Communiquer avec l'ATMega

```cpp
void softSerialEvent() {
  while (swSer.available()) {
    char inChar = (char)swSer.read();
    // add it to the inputString:
    if(inputString.length() <= Tmax){
      if (inChar == CF) {
        stringComplete = true;
      }else
         inputString += inChar;
    }
  }
}

```

###  1.2) Extraire les informations d'une commande !I

```cpp

void getValueAll(String data, char separator)
{
  int found = 0;
  int cpt=0;
  String value="";
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      if(cpt==0)cmdUse = value;
      if(cpt==1)cmdOption = value;
      if(cpt==2)cmdTemperature = value;
      if(cpt==3)cmdHumidity = value;
      if(cpt==4)cmdLight = value;
      if(cpt==5)cmdVolt = value;
      if(cpt==6)cmdBtn = value;
      value="";
      cpt++;
    }
    else{
      value += data.charAt(i);
    }
  }
}

```


### 2) Un premier serveur web


```cpp
if (MDNS.begin("cnam")) {
    Serial.println("MDNS responder started");
  }
  
  ....
  
    server.on("/execute", aled);
    void aled() {
       int cmd = (server.arg(0) == "1")?1:0;
       digitalWrite(D2, cmd);
       swSer.write("!G1\n");  
}
```



### 3) Une page HTML qui affiche les informations reçues de l'ATMega


```cpp
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

           cmdUse.c_str(),cmdOption.c_str(),cmdTemperature.c_str(),cmdHumidity.c_str(),
           cmdLight.c_str(),
           cmdVolt.c_str(),
           cmdBtn.c_str()
          );
}
```


### 4)  Une courbe avec les dernières valeurs mesurées

```cpp
void drawGraphLight() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int tab[42];
  float gros = 0;
  for(int i=0;i<41;i++){
    softSerialEvent();
    float light = atoi(cmdLight.c_str());
    Serial.println(light);
    float calcul = light;
    tab[i] = calcul;
    gros = (gros < calcul)?calcul:gros;
    if (stringComplete) {
    Serial.println(inputString);
    getValueAll(inputString,';');
    rep = inputString;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  delay(1000);
  }
  float calcul = tab[0]/gros;
  calcul = 130*calcul;
  int y = calcul;
  int i = 1;
  
  for (int x = 10; x < 390; x += 10) {
    float calcul = tab[i]/gros;
  calcul = 130*calcul;
    int y2 = calcul;
    Serial.println(y2);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140-y, x + 10, 140-y2);
    out += temp;
    y = y2;
    i++;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
```


 ![GitHub Logo](https://i.imgur.com/iRi51B3.png)
