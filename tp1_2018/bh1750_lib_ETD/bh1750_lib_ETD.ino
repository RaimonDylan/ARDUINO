#include <Wire.h>
#include "bh1750.h"
#include "bh1750.cpp"


BH1750 sensor;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  sensor.begin();
  uint8_t byteW ;
  delay(1000);
}

void loop() {

uint16_t data ;
data = sensor.getRawLight();

Serial.println(data); 
if(data > 200){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
} else {
  digitalWrite(LED_BUILTIN, LOW);
}




}



