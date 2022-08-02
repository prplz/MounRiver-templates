/********************************** (C) COPYRIGHT *******************************
* File Name          : INT.C
* Author             : WCH
* Version            : V1.0
* Date               : 2021/01/26
* Description        : CH56X GPIO_INT DEMO
                     (1)演示printf功能
                     (2)演示外部PB IO口的IRQ中断
                     (3)演示定时器0的FIQ中断
*******************************************************************************/



/******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "CH563SFR.h"
#include "SYSFREQ.h"


/*LED灯管脚定义*/
#define LED                  1<<3

#define LED_OUT_INIT(  )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }          /* LED初始化 */
#define LED_OUT_ACT(  )      { R32_PB_CLR |= LED; }                             /* LED亮 */
#define LED_OUT_INACT(  )    { R32_PB_OUT |= LED; }                             /* LED灭 */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ中断函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 void IRQ_Handler( void )
{
    if((R32_INT_FLAG>>8) & RB_IF_PB)
    {
        printf("GPIO INT \r\n");
        R32_INT_STATUS_PB = 0xfffff;
    }
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : FIQ中断函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

 void FIQ_Handler( void )
{
     if(R32_INT_FLAG & RB_IF_TMR0)
     {
         printf("TIM0 INT\r\n");
          R8_TMR0_INT_FLAG |= 0xff;
    }
}

/*******************************************************************************
* Function Name  : FIQ_InitTim0
* Description    : 定时器FIQ中断初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


 void FIQ_InitTim0(void)
 {
     R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
     R32_TMR0_COUNT = 0x00000000;
     R32_TMR0_CNT_END = 0x186a0 * 100;                            /* 100ms定时*/
     R8_TMR0_INTER_EN |= RB_TMR_IE_CYC_END;
     R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
     R8_INT_EN_FIQ_0 |=RB_IE_IRQ_TMR0;
     R8_INT_EN_FIQ_GLOB |=RB_IE_FIQ_GLOB;
 }


 /*******************************************************************************
 * Function Name  : mInitSTDIO
 * Description    : printf初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / 115200;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;
    R8_UART0_LCR = 0x80;
    R8_UART0_DIV = 1;
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* 8位数据位 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* 串口0发送使能 */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 output enable */
}

/*******************************************************************************
* Function Name  : _write
* Description    : printf输出映射
* Input          : c-- writes the character specified by c 
*                  *f--the output stream pointed to by *f
* Output         : None
* Return         : None
*******************************************************************************/

int  _write(int fd, char *buf, int size)
{
    int i;

      for(i=0; i<size; i++)
      {
              while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );
              R8_UART0_THR = *buf++;
      }

        return size;

}



/*******************************************************************************
* Function Name  : IRQ_InitPB
* Description    : PB口IRQ中断初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void IRQ_InitPB( void )
{
    R32_PB_PU  |= 0x000000ff;                                                   /* GPIO B上拉 */
    R32_PB_DIR &= ~0x000000ff;                                                  /* GPIO B 方向设置为输入*/

    R32_INT_ENABLE_PB |= 0x000000ff;                                            /* GPIO B中断使能 */
    R32_INT_MODE_PB   |= 0x000000ff;                                            /* GPIO B中断触发方式为边沿触发 */
    R32_INT_POLAR_PB  &= ~0x000000ff;                                           /* GPIO B上升沿触发 */
    R32_INT_STATUS_PB  = 0xfffff;                                               /* 中断标志写1清0 */
    R8_INT_EN_IRQ_1   |= RB_IE_IRQ_PB;                                          /* GPIO B组IRQ中断使能 */
    R8_INT_EN_IRQ_GLOB|= RB_IE_IRQ_GLOB;                                        /* IRQ全局中断使能 */
}

/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main( void ) 
{ 
    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* LED点灯*/
    Delay_ms( 100 );
    LED_OUT_INACT( );                                                           /* LED灭灯*/
    mInitSTDIO( );                                                              /* printf初始化*/
    printf("INT DEMO.....\n");
    IRQ_InitPB( );                                                              /* PB口外部IO中断 */
    FIQ_InitTim0();
    while(1);
}

/*********************************** endfile **********************************/
