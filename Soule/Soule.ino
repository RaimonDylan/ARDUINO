#include "bh1750.h"


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int Tmax = 300;
char CF = '\n'; 

void setup() {
    pinMode(11, OUTPUT);          // sets the digital pin 13 as output
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(Tmax);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    //
    if(stringComplete == "!R"){
      digitalWrite(11, HIGH);
    }
     //
    Serial.println(parseCommandLine(inputString));
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

int8_t parseCommandLine(String s){
  if (s =="Meh")
    return 0;


  else return -1;
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
