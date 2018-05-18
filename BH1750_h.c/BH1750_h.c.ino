#define BH1750_h
#include "Arduino.h"
#include "Wire.h"
class BH1750 {
public:
BH1750(); // constructeur (rien à faire mais doit exister!)
void begin(void); // initialisation du circuit
void setAddress( uint8_t address = 0x23) ; // fixe l'adresse du
circuit
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
