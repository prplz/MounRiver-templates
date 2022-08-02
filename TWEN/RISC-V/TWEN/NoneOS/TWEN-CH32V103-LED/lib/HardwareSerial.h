/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include "Stream.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "wiring.h"



#ifdef __cplusplus
}
#endif


#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

struct ring_buffer;

class HardwareSerial: public Stream
{
  private:
    

    void printNumber(unsigned long, uint8_t);


  public:
    HardwareSerial(ring_buffer *rx_buffer,USART_TypeDef *UART);
    __IO USART_TypeDef *_port;
    ring_buffer *_rx_buffer;
    void begin(uint32_t);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();



     // pull in write(str) and write(buf, size) from Print
};






extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;


#endif

