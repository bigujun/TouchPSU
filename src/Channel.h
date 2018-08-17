#ifndef  Channel_h
#define Channel_h

#define NUM_STR_SIZE 5
#define NUM_STR_DECIMAL 2
#define NUM_STR_FILL true

#define ANALOG_READS 100
// #define USE_TIMER 3
#include "GUIslice.h"
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
  Channel(char _name,uint8_t _currentPin,uint8_t _voltagePin,uint8_t _outPin);
  char name;
  double voltage,current,voltageSet,currentSet;
  char voltageStr[NUM_STR_SIZE+2],currentStr[NUM_STR_SIZE+2],voltageSetStr[NUM_STR_SIZE+2],currentSetStr[NUM_STR_SIZE+2];
  char statusStr[3];
  uint8_t currentPin,voltagePin,outPin; //TODO
  double maxCurrent,maxVoltage,minVoltage; //TODO

  gslc_tsGui *pGui;
  gslc_tsElemRef* txtVoltage=NULL;
  gslc_tsElemRef* txtCurrent=NULL;
  // gslc_tsElemRef* txtVoltageSet=NULL;
  // gslc_tsElemRef* txtCurrentSet=NULL;
  gslc_tsElemRef* txtPotenci=NULL;
  // gslc_tsElemRef* btnOn=NULL;


  void toogleStatus();
  void set(Type t, char* valStr);
  void set(Type t, double val);
  void setAtual(Type t,double val);
  // void setAtual(Type t,)
  void setup(gslc_tsGui *_pGui, gslc_tsElemRef* volt,gslc_tsElemRef* cur,gslc_tsElemRef* pot);
  void interrupt();

private:
  static double mapFloat(double x, double in_min, double in_max, double out_min, double out_max);
  void updateStatusStr();

  void fillZero(char* str, uint8_t size);

};

#endif
