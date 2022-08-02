#include <CH32V103.h>
#include "CH32V_WS2812.h"

uint16_t i = 0;
uint16_t j = 0;
uint8_t pos = 0;

WS2812 WS2812_PD2(PD2);

int main(void)
{
  CH32_Init();

  WS2812_PD2.pixel_init(6);
  WS2812_PD2.setBrightness(50);
  while(1){
    for (j = 0; j < 256; j = j + 1) {
      for (i = 0; i < 6; i = i + 1) {
        pos = 255 - ((i + j)&255);
        if(pos < 85){
          WS2812_PD2.pixel_set_color(i,(255 - pos * 3),0,(pos * 3));
        }
        else if(pos < 170){
          pos = pos - 85;
          WS2812_PD2.pixel_set_color(i,0,(pos * 3),(255 - pos * 3));
        }
        else{
          pos = pos - 170;
          WS2812_PD2.pixel_set_color(i,(pos * 3),(255 - pos * 3),0);
        }
      }
      WS2812_PD2.pixel_show();
      delay(5);
    }

  }
  return 1;
}
