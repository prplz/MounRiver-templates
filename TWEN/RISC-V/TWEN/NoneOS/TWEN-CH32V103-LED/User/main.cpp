#include <CH32V103.h>
#include "CDC.h"
int main(void)
{
   CH32_Init();
   pinMode(PC8, GPIO_Mode_Out_PP);
   digitalWrite(PC8, 1);
   SerialUSB.begin(9600);
   while (!SerialUSB);


  while(1){
    SerialUSB.println("∞Â‘ÿµ∆ ‰≥ˆøÿ÷∆µ„¡¡∞Â‘ÿµ∆");
    delay(1000);
  }
  return 1;
}
