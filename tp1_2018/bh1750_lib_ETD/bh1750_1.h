#ifndef BH1750_h
#define BH1750_h

#include "Arduino.h"

#include "Wire.h"

#define ADDR 0x23
#define POWER_ON 0x01
#define RESET 0x07
#define CNT_HR 0x10
#define BH1750_TIME_OUT 200 // temps max d'attente de la réponse (ms)
#define BH1750_ERROR 0xFFFF // valeur retournée si le circuit ne répond pas


class BH1750 {    
  public:
   BH1750();  // constructeur  
   void begin(void); // initialisation du circuit
   void setAddress( uint8_t address = 0x23) ; // fixe adresse du circuit
   void setMode(uint8_t mode ); // change le mode de fonctionnement  
   void reset(void); // reset du circuit
   uint16_t getRawLight(void); // lit le résultat brut d'une mesure
  private:
   void i2cWrite8( uint8_t data ) ; // écrit un octet
   uint16_t i2cRead16( ) ; // lit 16 bits
   uint8_t address ;
   uint8_t mode ;
  };
  #endif
