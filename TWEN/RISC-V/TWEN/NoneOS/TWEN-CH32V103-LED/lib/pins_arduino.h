/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of HDUSB - http://www.haohaodada.com/

  Copyright (c) 2019 Blue

*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "ch32v10x.h"

	
#define NUM_DIGITAL_PINS            51
#define NUM_PWM_PINS                8
#define NUM_ANALOG_INPUTS           16
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 18 : (p) - 6)

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1 || (p) == 2 || (p) == 3 || (p) == 4 || (p) == 5 || (p) == 6 || (p) == 7)


#define PIN_SPI_SS    (11)
#define PIN_SPI_MOSI  (12)
#define PIN_SPI_MISO  (13)
#define PIN_SPI_SCK   (14)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (25)
#define PIN_WIRE_SCL        (24)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 17

#define PIN_A0   (0)
#define PIN_A1   (1)
#define PIN_A2   (2)
#define PIN_A3   (3)
#define PIN_A4   (4)
#define PIN_A5   (5)
#define PIN_A6   (6)
#define PIN_A7   (7)
#define PIN_A8   (8)
#define PIN_A9   (9)
#define PIN_A10   (10)
#define PIN_A11   (11)
#define PIN_A12   (12)
#define PIN_A13   (13)
#define PIN_A14   (14)
#define PIN_A15   (15)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;
static const uint8_t A11 = PIN_A11;
static const uint8_t A12 = PIN_A12;
static const uint8_t A13 = PIN_A13;
static const uint8_t A14 = PIN_A14;
static const uint8_t A15 = PIN_A15;

static const uint32_t DIGITAL_PIN_PORT[NUM_DIGITAL_PINS]={
  GPIOA_BASE,  //PA0
  GPIOA_BASE,  //PA1
  GPIOA_BASE,  //PA2
  GPIOA_BASE,  //PA3
  GPIOA_BASE,  //PA4
  GPIOA_BASE,  //PA5
  GPIOA_BASE,  //PA6
  GPIOA_BASE,  //PA7
  GPIOA_BASE,  //PA8
  GPIOA_BASE,  //PA9
  GPIOA_BASE,  //PA10
  GPIOA_BASE,  //PA11
  GPIOA_BASE,  //PA12
  GPIOA_BASE,  //PA13
  GPIOA_BASE,  //PA14
  GPIOA_BASE,  //PA15
  GPIOB_BASE,  //PB0
  GPIOB_BASE,  //PB1
  GPIOB_BASE,  //PB2
  GPIOB_BASE,  //PB3
  GPIOB_BASE,  //PB4
  GPIOB_BASE,  //PB5
  GPIOB_BASE,  //PB6
  GPIOB_BASE,  //PB7
  GPIOB_BASE,  //PB8
  GPIOB_BASE,  //PB9
  GPIOB_BASE,  //PB10
  GPIOB_BASE,  //PB11
  GPIOB_BASE,  //PB12
  GPIOB_BASE,  //PB13
  GPIOB_BASE,  //PB14
  GPIOB_BASE,  //PC15
  GPIOC_BASE,  //PC0
  GPIOC_BASE,  //PC1
  GPIOC_BASE,  //PC2
  GPIOC_BASE,  //PC3
  GPIOC_BASE,  //PC4
  GPIOC_BASE,  //PC5
  GPIOC_BASE,  //PC6
  GPIOC_BASE,  //PC7
  GPIOC_BASE,  //PC8
  GPIOC_BASE,  //PC9
  GPIOC_BASE,  //PC10
  GPIOC_BASE,  //PC11
  GPIOC_BASE,  //PC12
  GPIOC_BASE,  //PC13
  GPIOC_BASE,  //PC14
  GPIOC_BASE,  //PC15
  GPIOD_BASE,  //PD0
  GPIOD_BASE,  //PD1
  GPIOD_BASE
};

const uint32_t DIGITAL_PIN_NUM[NUM_DIGITAL_PINS]={
  0, //PA0
  1, //PA1
  2, //PA2
  3, //PA3
  4, //PA4
  5, //PA5
  6, //PA6
  7, //PA7
  8, //PA8
  9, //PA9
  10,//PA10
  11,//PA11
  12,//PA12
  13,//PA13
  14,//PA14
  15,//PA15
  0, //PB0
  1, //PB1
  2, //PB2
  3, //PB3
  4, //PB4
  5, //PB5
  6, //PB6
  7, //PB7
  8, //PB8
  9, //PB9
  10,//PB10
  11,//PB11
  12,//PB12
  13,//PB13
  14,//PB14
  15,//PB15
  0, //PC0
  1, //PC1
  2, //PC2
  3, //PC3
  4, //PC4
  5, //PC5
  6, //PC6
  7, //PC7
  8, //PC8
  9, //PC9
  10,//PC10
  11,//PC11
  12,//PC12
  13,//PC13
  14,//PC14
  15,//PC15
  0, //PD0
  1, //PD1
  2, //PD2
};

const uint32_t ANALOG_PIN_NUM[NUM_ANALOG_INPUTS]={
  0,//PA0
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  16,
  17,
  32,
  33,
  34,
  35,
  36,
  37
};	
typedef enum
{
    PA0 = 0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PA8,
    PA9,
    PA10,
    PA11,
    PA12,
    PA13,
    PA14,
    PA15,
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PB5,
    PB6,
    PB7,
    PB8,
    PB9,
    PB10,
    PB11,
    PB12,
    PB13,
    PB14,
    PB15,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PC8,
    PC9,
    PC10,
    PC11,
    PC12,
    PC13,
    PC14,
    PC15,
    PD0,
    PD1,
    PD2
} PIN_Name;



#endif