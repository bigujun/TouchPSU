#include "Channel.h"

Channel::Channel(char _name){
  name=_name;
  set(VOLTAGE,1.0);
  set(CURRENT,2.0);
  setAtual(VOLTAGE,3.0);
  setAtual(CURRENT,4.0);
  updateStatusStr();
}
void Channel::set(Type t, char* valStr){
  double val = atof(valStr);
  set(t,val);
}
void Channel::set(Type t, double val){
  // Serial.println(val);
  if(t==VOLTAGE){
    voltageSet=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, voltageSetStr);
    strcat(voltageSetStr,"V");
    fillZero(voltageSetStr,NUM_STR_SIZE);
  }else{
    currentSet=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, currentSetStr);
    strcat(currentSetStr,"A");
    fillZero(currentSetStr,NUM_STR_SIZE);
  }
}

void Channel::setAtual(Type t,double val){
  if(t==VOLTAGE){
    voltage=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, voltageStr);
    strcat(voltageStr,"V");
    fillZero(voltageStr,NUM_STR_SIZE);
  }else{
    current=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, currentStr);
    strcat(currentStr,"A");
    fillZero(currentStr,NUM_STR_SIZE);
  }
}

void Channel::fillZero(char* str, uint8_t size){
  for(int i=0;i<size;i++){
    if(str[i]==' ')
      str[i]='0';
  }
}

void Channel::toogleStatus(){
  status=(status==OFF)?ON:OFF;
  updateStatusStr();
}

void Channel::updateStatusStr(){
  if(status==ON){
    strcpy(statusStr,"ON");
  }else{
    strcpy(statusStr,"OFF");
  }
}
