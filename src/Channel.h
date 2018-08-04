#ifndef  Channel_h
#define Channel_h

#define NUM_STR_SIZE 5
#define NUM_STR_DECIMAL 2
#define NUM_STR_FILL true
// #include "GUIslice.h"
#include <Arduino.h>


class Channel{
public:
  enum Status{
    OFF,
    ON,
  }status = OFF;
  enum Type{
    VOLTAGE,
    CURRENT
  };
  Channel(char _name);
  char name;
  double voltage,current,voltageSet,currentSet;
  char voltageStr[NUM_STR_SIZE+2],currentStr[NUM_STR_SIZE+2],voltageSetStr[NUM_STR_SIZE+2],currentSetStr[NUM_STR_SIZE+2];
  char statusStr[3];

  // gslc_tsElemRef* txtVoltage=NULL;
  // gslc_tsElemRef* txtCurrent=NULL;
  // gslc_tsElemRef* txtVoltageSet=NULL;
  // gslc_tsElemRef* txtCurrentSet=NULL;
  // gslc_tsElemRef* txtPotenci=NULL;
  // gslc_tsElemRef* btnOn=NULL;


  void toogleStatus();
  void set(Type t, char* valStr);
  void set(Type t, double val);
  void setAtual(Type t,double val);
  // void setAtual(Type t,)
private:
  void updateStatusStr();
  void fillZero(char* str, uint8_t size);
};

#endif
