/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIRING_MATH_
#define _WIRING_MATH_

// extern long random( long ) ;
// extern long random( long, long ) ;
// extern void randomSeed( uint32_t dwSeed ) ;
// extern long map( long, long, long, long, long ) ;

extern uint16_t makeWord( uint16_t w ) ;
extern uint16_t makeWord( uint8_t h, uint8_t l ) ;

#define word(...) makeWord(__VA_ARGS__)

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

#endif /* _WIRING_MATH_ */
