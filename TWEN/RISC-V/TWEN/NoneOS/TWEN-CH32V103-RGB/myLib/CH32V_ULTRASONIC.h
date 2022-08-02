#ifndef _CH32V_ULTRASONIC_H_
#define _CH32V_ULTRASONIC_H_


#include <CH32V103.h>

class CH32V_Ultrasonic
{
public:
	CH32V_Ultrasonic(uint8_t pin);//三线超声波，收发同一个引脚
    CH32V_Ultrasonic(uint8_t trig, uint8_t echo);//四线超声波，收发不同引脚
	double distance(uint16_t MAXcm=400);
private:
	uint8_t _Ultrasonic_pin,_Ultrasonic_pin2;
	long measure(unsigned long timeout);
    volatile bool _measureFlag;
    volatile long _lastEnterTime;
    volatile float _measureValue;
};


CH32V_Ultrasonic::CH32V_Ultrasonic(uint8_t pin)
{
  _Ultrasonic_pin = pin;
  _Ultrasonic_pin2 = pin;
  _lastEnterTime = millis();
  _measureFlag = true;
  _measureValue = 0;
}

CH32V_Ultrasonic::CH32V_Ultrasonic(uint8_t trig, uint8_t echo)
{
  _Ultrasonic_pin = trig;
  _Ultrasonic_pin2 = echo;
  _lastEnterTime = millis();
  _measureFlag = true;
  _measureValue = 0;
}

long CH32V_Ultrasonic::measure(unsigned long timeout)
{
  long duration;
  
  if(millis() - _lastEnterTime > 23)
  {
    _measureFlag = true; 
  }

  if(_measureFlag == true)
  {
    _lastEnterTime = millis();
    _measureFlag = false;
    pinMode(_Ultrasonic_pin, GPIO_Mode_Out_PP);
    digitalWrite(_Ultrasonic_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(_Ultrasonic_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(_Ultrasonic_pin,LOW);
    pinMode(_Ultrasonic_pin2, GPIO_Mode_IPU);
    duration = pulseIn(_Ultrasonic_pin2, HIGH, timeout);
    _measureValue = duration;
  }
  else
  {
    duration = _measureValue;
  }
  return(duration);
}

double CH32V_Ultrasonic::distance(uint16_t MAXcm)
{
    long distance = measure(MAXcm * 55 + 200);
  if(distance == 0)
  {
    distance = MAXcm * 58;
  }
  return( (double)distance / 58.0);
}


#endif