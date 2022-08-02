/********************************** (C) COPYRIGHT *******************************
* File Name          : startup_ch579.s
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/12/22
* Description        : CH579 vector table for GCC toolchain.
*******************************************************************************/

  .syntax unified
  .cpu cortex-m0
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/*Check if boot space corresponds to test memory*/
 
    LDR R0,=0x00000004
    LDR R1, [R0]
    LSRS R1, R1, #24
    LDR R2,=0x1F
    CMP R1, R2
    BNE ApplicationStart

 /*SYSCFG clock enable*/

    LDR R0,=0x40021018
    LDR R1,=0x00000001
    STR R1, [R0]

/*Set CFGR1 register with flash memory remap at address 0*/
    LDR R0,=0x40010000
    LDR R1,=0x00000000
    STR R1, [R0]

ApplicationStart:
/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str  r3, [r2]
  adds r2, r2, #4


LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit
/* Call static constructors */
  /*  bl __libc_init_array*/
/* Call the application's entry point.*/
  bl main
  
LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word 0
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler
 /*******************************************************************************
 External Interrupts
*******************************************************************************/
                .word    TMR0_IRQHandler
                .word    GPIO_IRQHandler
                .word    SLAVE_IRQHandler
                .word    SPI0_IRQHandler
                .word    BB_IRQHandler
                .word    LLE_IRQHandler
                .word    USB_IRQHandler
                .word    ETH_IRQHandler
                .word    TMR1_IRQHandler
                .word    TMR2_IRQHandler
                .word    UART0_IRQHandler
                .word    UART1_IRQHandler
                .word     RTC_IRQHandler
                .word     ADC_IRQHandler
                .word    SPI1_IRQHandler
                .word    LED_IRQHandler
                .word    TMR3_IRQHandler
                .word    UART2_IRQHandler
                .word    UART3_IRQHandler
                .word     WDT_IRQHandler

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak TMR0_IRQHandler
  .thumb_set TMR0_IRQHandler,Default_Handler

  .weak GPIO_IRQHandler
  .thumb_set GPIO_IRQHandler,Default_Handler
  
  .weak SLAVE_IRQHandler
  .thumb_set SLAVE_IRQHandler,Default_Handler
  
  .weak SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler,Default_Handler
  
  .weak BB_IRQHandler
  .thumb_set BB_IRQHandler,Default_Handler
  
  .weak  LLE_IRQHandler
  .thumb_set  LLE_IRQHandler ,Default_Handler
  
  .weak USB_IRQHandler
  .thumb_set USB_IRQHandler,Default_Handler
  
  .weak ETH_IRQHandler
  .thumb_set ETH_IRQHandler,Default_Handler
  
  .weak TMR1_IRQHandler
  .thumb_set TMR1_IRQHandler,Default_Handler
  
  .weak TMR2_IRQHandler
  .thumb_set TMR2_IRQHandler,Default_Handler
  
  .weak  UART0_IRQHandler
  .thumb_set  UART0_IRQHandler,Default_Handler
  
  .weak UART1_IRQHandler
  .thumb_set UART1_IRQHandler,Default_Handler
  
  .weak RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler
   
  .weak ADC_IRQHandler
  .thumb_set ADC_IRQHandler ,Default_Handler
  
  .weak SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler ,Default_Handler
  
  .weak  LED_IRQHandler
  .thumb_set  LED_IRQHandler,Default_Handler
  
  .weak TMR3_IRQHandler
  .thumb_set TMR3_IRQHandler,Default_Handler
  
  .weak UART2_IRQHandler
  .thumb_set UART2_IRQHandler ,Default_Handler
  
  .weak UART3_IRQHandler
  .thumb_set UART3_IRQHandler,Default_Handler
  
  .weak WDT_IRQHandler
  .thumb_set WDT_IRQHandler,Default_Handler
  
  


