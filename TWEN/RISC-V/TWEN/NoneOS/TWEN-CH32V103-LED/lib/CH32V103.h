#ifndef CH32V103_h
#define CH32V103_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch32v10x.h"



#include "WString.h"
#include "itoa.h"
#include "WMath.h"

#ifdef __cplusplus
extern "C"{
#endif
#include "wiring.h"
#include "pins_arduino.h"
//#include "debug.h"
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#define SystemCoreClock 72000000L
#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )


#define HIGH 0x1
#define LOW  0x0


#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0

#define uchar unsigned char

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

// #define interrupts() sei()
// #define noInterrupts() cli()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean;
typedef uint8_t byte;

// void attachInterrupt(uint8_t, void (*)(void), int mode);
// void detachInterrupt(uint8_t);
int atexit(void (*func)()) __attribute__((weak));

void 	pinMode(uint8_t,uint8_t);
void 	digitalWrite(uint8_t,uint8_t);
void 	digitalToggle(uint8_t pin);
uint8_t digitalRead(uint8_t);
int 	analogRead(uint8_t);
void 	analogWrite(uint8_t, int);


unsigned long millis(void);
unsigned long micros(void);
void delay(uint32_t );
void delayMicroseconds(uint32_t us);



void setup(void);
void loop(void);

#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"

#define word(...) makeWord(__VA_ARGS__)


// #define D0  25
// #define D1  24
// #define D2  2
// #define D3  3
// #define D4  26
// #define D5  27
// #define D6  6
// #define D7  7
// #define D8  28
// #define D9  9
// #define D10 10
// #define D11  11
// #define D12  12
// #define D13  13



#endif
//#define HDUSB_BADGEDISPLAY_BOARD


#endif