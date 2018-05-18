#include <Wire.h>
#include "bh1750.h"


BH1750 sensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  sensor.begin();
 
    

}

void loop() {
delay(150);
uint16_t light = sensor.getRawLight(); 
Serial.println(light); 

}



