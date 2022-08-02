
#ifndef _CH32V_MATH_
#define _CH32V_MATH_

#include "stdlib.h"

#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void randomSeed( uint32_t dwSeed )
{
  if ( dwSeed != 0 )
  {
    srand( dwSeed ) ;
  }
}

long random( long howsmall, long howbig )
{
  long diff;
  if (howsmall >= howbig)
  {
    return howsmall;
  }

  diff = howbig - howsmall;

  if ( diff == 0 )
  {
    return 0 ;
  }

  return rand() % diff + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif