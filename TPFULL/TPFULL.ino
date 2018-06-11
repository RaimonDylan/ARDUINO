#include <dht11.h>
#include <bh1750.h>


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int Tmax = 300;
#define SEUIL 200
char CF = '\n'; 
int val = 0;
int m  = 0;
int n  = 0;
int16_t sensorValue  = 0;
dht11 dht;
BH1750 sensor;
String aze = "";
String rty = "";
String res = "";
String humidity = "";
String temperature = "";
String separateur = ";";
bool activeI = false;
uint16_t light=0;
int red=0;
int green=0;
int blue=0;
int oldInput=0;
uint16_t volt=0;
String btn="";
unsigned long previousMillisLed = 0;        // will store last time LED was updated
unsigned long previousMillisTH = 0; 
unsigned long previousMillisI = 0; 
unsigned long previousMillisLux = 0; 
const long intervalLed = 500;  
const long intervalI= 1000;
const long intervalLux= 5000;
const long intervalTH = 2000;// interval at which to blink (milliseconds)




void setup() {
    pinMode(13, OUTPUT);          // sets the digital pin 13 as output
    pinMode(5, OUTPUT);          // sets the digital pin 13 as output
    pinMode(6, OUTPUT);          // sets the digital pin 13 as output
    pinMode(11, OUTPUT);          // sets the digital pin 13 as output
    pinMode(0xA1, INPUT); // PREPARATION BTN
    pinMode(2, OUTPUT); // PREPARATION BTN
       pinMode(LED_BUILTIN, OUTPUT);
       Wire.begin();
       Serial.begin(9600);
     sensor.begin();
     
     
    
    

  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(Tmax);
}

void loop() {
  dht.read(7);
  val = digitalRead(0xA1);  
  m = analogRead(1);
unsigned long currentMillis = millis();
unsigned long currentMillis2 = millis();
unsigned long currentMillis3 = millis();

  if (currentMillis - previousMillisLed >= intervalLed) {
    previousMillisLed = currentMillis;
    switchLight();
    parseBtn(m);
    }
    if (currentMillis2 - previousMillisTH >= intervalTH) {
    previousMillisTH = currentMillis2;
    humidity = (float)(dht.humidity);
    temperature = (float)(dht.temperature);
    }
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
  if (stringComplete) {
 
 delay(500);
    Serial.print('#');
    Serial.println(parseCommandLine(inputString));
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

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
       case 'P':
        aze = "P;";
        res = s[2];
        if(s[2] == '1')digitalWrite(2, HIGH);else if(s[2]=='0')digitalWrite(2, LOW);else return "?";
        break;
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
      case 'T':
        aze = "T;";
        res = temperature;
        break;
      case 'H':
        aze = "H;";
        res = humidity;
        break;
       case 'L':
        aze = "L;";
        light = sensor.getRawLight(); 
        res = light;
        break;
       case 'V':
        aze = "V;";
        volt = readPwrVoltage(); 
        res = volt;
        break;
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
        
    }
    rty = ";ok";
    return aze+res+rty;
  }else return "?";
 }

int8_t parseBtn(int s){
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

 String readbuttons(int s){
  if(s!=0){
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
    res = (s<375 && s>365)?1 // BP1
    :(s<210 && s>200)?2 // BP2
    :(s<150 && s>120)?4 // BP1+BP2
    :(s<470 && s>450)?3 // BP3
    :(s<300 && s>290)?6 // BP2+BP3
    :(s<435 && s>420)?5 // BP1+BP3
    :(s<510 && s>500)?7 // BP1+BP2+BP3
    :0;
        return res;
    }else return "0";

 

 }





/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
}
int16_t readPwrVoltage(){
  sensorValue = analogRead(0);
  sensorValue = sensorValue*(3300/1024);
  return sensorValue;
}

