#include <Wire.h>
#include "bh1750.h"


BH1750 sensor;

void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(9600);

  sensor.begin();
 
    

}

void loop() {
delay(150);
uint16_t light = sensor.getRawLight(); 
Serial.println(light); 

if(light > 200){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
} else {
  digitalWrite(LED_BUILTIN, LOW);
}

}



