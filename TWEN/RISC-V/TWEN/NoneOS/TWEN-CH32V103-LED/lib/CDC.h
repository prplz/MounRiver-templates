#ifndef CDC_h
#define CDC_h
#include "CH32V103.h"
#include <inttypes.h>
#include "usb_com.h"
#include "Stream.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

class CDCSerial : public Stream
{
private:
  int head;

public:
  CDCSerial();
  int read(uint8_t *temp, int size);
  void begin(uint32_t);
  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual void flush(void);
  operator bool();

  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *, size_t);
  using Print::write; // pull in write(str) and write(buf, size) from Print
};

// Constructors ////////////////////////////////////////////////////////////////

CDCSerial::CDCSerial()
{
}

// Public Methods //////////////////////////////////////////////////////////////

void CDCSerial::begin(uint32_t speed)
{
  usb_com();
  head = 0;
}

int CDCSerial::available(void)
{
  //if(count_out<=head)delayMicroseconds(20);

  return (count_out);
}

int CDCSerial::peek(void)
{
  if (count_out == 0)
  {
    return -1;
  }
  else
  {
    return buffer_out[count_out - 1];
  }
}

int CDCSerial::read(void)
{
  int ch;

  if (head < (count_out))
  {
    ch = buffer_out[head];
    head++;
    if (head >= count_out)
    {
      head = 0;
      count_out = 0;
    }

    return ch;
  }

  return -1;
}
int CDCSerial::read(uint8_t *temp, int size)
{

  int rsize = count_out - head;
  for (int i = head; i < count_out; i++)
    temp[i - head] = buffer_out[i];
  head = 0;
  count_out = 0;

  return (rsize);
}

void CDCSerial::flush()
{
  count_out = 0;
}

CDCSerial::operator bool()
{
  // bool result = false;
  // if (_usbLineInfo.lineState > 0)
  // 	result = true;
  // delay(10);
  // return result;
  bool _configured = 0;
  while (bDeviceState != CONFIGURED)
    ;
  if (_configured == 0)
  {
    delay(2000);
    _configured = 1;
  }
  return _configured;
}

size_t CDCSerial::write(uint8_t ch)
{
  /* while (status_UsbUart != 0);
    buffer_in[count_in++] = ch;
    
    return 1;*/

  return write(&ch, 1);
}

size_t CDCSerial::write(const uint8_t *buffer, size_t size)
{
  int ssize = size;
  int ep2_cout = 0;
  while (1)
  {
    while (Endp2Busy != 0)
      ;
    if (ssize <= 64)
    {
      NVIC_DisableIRQ(USBHD_IRQn);
      for (int i = 0; i < ssize; i++)
        EP2_Databuf[64 + i] = buffer[ep2_cout + i];

      DevEP2_IN_Deal(ssize);
      Endp2Busy = 1;
      NVIC_EnableIRQ(USBHD_IRQn);
      return 1;
    }
    else
    {
      NVIC_DisableIRQ(USBHD_IRQn);
      for (int i = 0; i < 64; i++)
        EP2_Databuf[64 + i] = buffer[ep2_cout + i];
      DevEP2_IN_Deal(64);
      Endp2Busy = 1;
      NVIC_EnableIRQ(USBHD_IRQn);
      ssize = ssize - 64;
      ep2_cout=ep2_cout+64;
    }
  }

  return 1;
}

CDCSerial SerialUSB;
#endif
