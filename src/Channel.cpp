#include "Channel.h"

Channel::Channel(char _name,uint8_t _currentPin,uint8_t _voltagePin,uint8_t _outPin){
  name=_name;
  outPin=_outPin;
  digitalWrite(outPin,LOW);
  voltagePin = _voltagePin;
  currentPin = _currentPin;
  minVoltage = 1.2;
  maxVoltage = 20.0;
  set(VOLTAGE,1.2);
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
  interrupt();
}

void Channel::setAtual(Type t,double val){
  char auxStr[NUM_STR_SIZE+2];
  if(t==VOLTAGE){
    voltage=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, auxStr);
    strcat(auxStr,"V");
    fillZero(auxStr,NUM_STR_SIZE);

    // gslc_ElemSetRedraw(pGui, txtVoltage, GSLC_REDRAW_INC);
    // snprintf(acTxt,MAX_STR,"%u",m_nCount/5);
    gslc_ElemSetTxtStr(pGui,txtVoltage,auxStr);

  }else{
    current=val;
    dtostrf(val, NUM_STR_SIZE, NUM_STR_DECIMAL, auxStr);
    strcat(auxStr,"A");
    fillZero(auxStr,NUM_STR_SIZE);
    // gslc_ElemSetRedraw(pGui, txtCurrent, GSLC_REDRAW_INC);
    gslc_ElemSetTxtStr(pGui,txtCurrent,auxStr);
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

void Channel::setup(gslc_tsGui *_pGui,gslc_tsElemRef* volt,gslc_tsElemRef* cur,gslc_tsElemRef* pot){
  txtVoltage = volt;
  txtCurrent = cur;
  txtPotenci = pot;
  pGui = _pGui;
  //INIT TIMERS
  if(outPin==44||outPin==45||outPin==46){
    TCCR5B = (TCCR5B & 0xF8) |0x01;
  }else{
    Serial.print("Erro Pino PWM HF");
  }


  //INIT PORTS
  pinMode(currentPin,INPUT);
  pinMode(voltagePin, INPUT);
  analogWrite(outPin,0);
  pinMode(outPin,OUTPUT);

}

void Channel::interrupt(){
  static unsigned long next=0;
  static unsigned int interval=1000;
  if(millis()<next)return;
  next = millis() + interval;
  analogReference(INTERNAL1V1);
  unsigned long v=0,c=0;
  static double pwm=0.0;
  double vd,cd;

  for(int i=0;i<ANALOG_READS;i++){
    v+=analogRead(voltagePin);
    c+=analogRead(currentPin);
  }
  Serial.print(name);
  Serial.println(c/ANALOG_READS);
  cd=mapFloat(c/ANALOG_READS, 0, 1023.0, 0.0, 3.0) ;
  vd=mapFloat(v/ANALOG_READS, 0, 1023.0, 0.0+1.03, 1.1*21.0+2.16);
  setAtual(CURRENT, cd);
  setAtual(VOLTAGE, vd);


  pwm = mapFloat(voltageSet,minVoltage,maxVoltage,0.0,255.0);
  Serial.println(pwm);
  if(pwm>255)pwm=255;
  if(pwm<0)pwm=0;
  analogWrite(outPin,pwm);
  analogReference(DEFAULT);
}


double Channel::mapFloat(double x, double in_min, double in_max, double out_min, double out_max)
{
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
