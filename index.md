# TP ARDUINO - CNAM:STMN 2018

> Mise en œuvre de capteurs - Dialogues sur la liaison série

## Réalisé par Dylan Raimon, Dylan Antonio et David MOLINARI

  ![GitHub Logo](https://boutique.semageek.com/2-4005-large_default/arduino-uno-dip-rev3.jpg)



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

