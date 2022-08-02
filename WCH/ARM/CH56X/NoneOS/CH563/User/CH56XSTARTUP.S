/********************************** (C) COPYRIGHT *******************************
* File Name          : startup_ch56x.s
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/12/22
* Description        : CH56x vector table for GCC toolchain.
*******************************************************************************/
.equ SWI_USE_ASM,          1           /*ASM code for SoftwareInterrupt*/
.equ IRQ_USE_ASM,          0           ;//ASM code for IRQ_Handler
.equ FIQ_USE_ASM,          0           ;//ASM code for FIQ_Handler
.equ RESERVED_VECTOR,      0x00000042  ;//reserved vector
/*;                                      if bit31:30=0:1, then
;                                        bit29=0 for 32K-CODE-RAM, bit29=1 for 64K/96K-CODE-RAM, default is 1
;                                        bit28=0 for disable ICE/debug, bit28=1 for enable ICE/debug, default is 0
;                                      if bit23:16=0xFF, then enable code ROM reading by external master, else disable
;
; define stack size*/
.equ UND_STACK_SIZE,    0x00000000  ;//stack size for UndefineInstruction
.equ ABT_STACK_SIZE,     0x00000000  //;stack size for PrefetchAbort/DataAbort
.equ SVC_STACK_SIZE,     0x00000010  ;//stack size for SoftwareInterrupt
.equ IRQ_STACK_SIZE,    0x00000100  ;//stack size for IRQ_Handler
.equ FIQ_STACK_SIZE,     0x00000100  ;//stack size for FIQ_Handler
.equ SYS_STACK_SIZE,     0x00000400  ;//stack size for system/user
.equ ALL_STACK_SIZE,     (UND_STACK_SIZE+ABT_STACK_SIZE+SVC_STACK_SIZE+IRQ_STACK_SIZE+FIQ_STACK_SIZE+SYS_STACK_SIZE)
.equ HEAP_SIZE,     0x0000000  ;//heap size

.section .init, "ax"
.code 32
.align 1
.globl _start
_start:



Reset:
                B       ResetInit
                B       UndefineInstruction
  #if SWI_USE_ASM == 0
  				//ldr  pc,   _SoftwareInterrupt
                //_SoftwareInterrupt: .word SoftwareInterrupt

  #endif

                B       SoftwareInterrupt
                B       PrefetchAbort
                B       DataAbort
               .word    RESERVED_VECTOR
                B       IRQ
                B       FIQ
IRQ:
				SUB     LR, LR, #4
 				STMFD   SP!, {R0-R7, LR}
                BL      IRQ_Handler
                LDMFD   SP!, {R0-R7, PC}^
FIQ:
                SUB     LR, LR, #4
                STMFD   SP!, {R0-R7, LR}
;                MRS     R1, SPSR
;                STMFD   SP!, {R1}
                BL      FIQ_Handler
;                LDMFD   SP!, {R1}
               /* MSR     SPSR_CXSF, R1*/
                LDMFD   SP!, {R0-R7, PC}^

;





UndefineInstruction:
/*;                STMFD   SP!, {R0-R12, LR}
;;                MRS     R1, SPSR
;;                STMFD   SP!, {R1}
;                BL      UndefineInstrHand
;;                LDMFD   SP!, {R1}
;;                MSR     SPSR_CXSF, R1
;                LDMFD   SP!, {R0-R12, PC}^*/
                B       UndefineInstruction

SoftwareInterrupt:

                 B       SoftwareInterrupt
;
PrefetchAbort:
/*;                SUB     LR, LR, #4
;                STMFD   SP!, {R0-R12, LR}
;;                MRS     R1, SPSR
;;                STMFD   SP!, {R1}
;                BL      PrefetchAbortHand
;;                LDMFD   SP!, {R1}
;;                MSR     SPSR_CXSF, R1
;                LDMFD   SP!, {R0-R12, PC}^*/
                B       PrefetchAbort
;
DataAbort:
/*;                SUB     LR, LR, #8
;                STMFD   SP!, {R0-R12, LR}
;;                MRS     R1, SPSR
;;                STMFD   SP!, {R1}
;                BL      DataAbortHand
;;                LDMFD   SP!, {R1}
;;                MSR     SPSR_CXSF, R1
;                LDMFD   SP!, {R0-R12, PC}^*/
                B       DataAbort
;
  #if SWI_USE_ASM != 0
SoftwareInterrupt:
/*;                STMFD   SP!, {R0-R12, LR}
;;                MRS     R1, SPSR
;;                STMFD   SP!, {R1}
;                BL      SoftwareInt
;;                LDMFD   SP!, {R1}
;;                MSR     SPSR_CXSF, R1
;                LDMFD   SP!, {R0-R12, PC}^*/
                B       SoftwareInterrupt
 #endif
;
  #if IRQ_USE_ASM !=  0
IRQ_Handler:
                SUB     LR, LR, #4
                STMFD   SP!, {R0-R12, LR}
//;                MRS     R1, SPSR
//;                STMFD   SP!, {R1}
                BL      IRQ_Exception
//;                LDMFD   SP!, {R1}
//;                MSR     SPSR_CXSF, R1
                LDMFD   SP!, {R0-R12, PC}^
 #endif

__rt_div0:
                B       __rt_div0

ResetInit:
.extern main
.extern SysFreq
                LDR     R0, Stack_Top
                MSR     CPSR_c, #0xDB
                MOV     SP, R0
                SUB     R0, R0, #UND_STACK_SIZE
                MSR     CPSR_c, #0xD7
                MOV     SP, R0
                SUB     R0, R0, #ABT_STACK_SIZE
                MSR     CPSR_c, #0xD3
                MOV     SP, R0
                SUB     R0, R0, #SVC_STACK_SIZE
                MSR     CPSR_c, #0xD2
                MOV     SP, R0
                SUB     R0, R0, #IRQ_STACK_SIZE
                MSR     CPSR_c, #0xD1
                MOV     SP, R0
                SUB     R0, R0, #FIQ_STACK_SIZE
//             MSR     CPSR_c, #0xDF
                MSR     CPSR_c, #0x1F
                MOV     SP, R0
                SUB     SL, SP, #SYS_STACK_SIZE
                BLX      SysFreq
               //  ldr  pc,   _SysFreq

               // _SysFreq: .word SysFreq
				LDR      sp,test
       movs  r1, #0
   b     LoopCopyDataInit

CopyDataInit:
  ldr   r3, =_sidata
  ldr   r3, [r3, r1]
  str   r3, [r0, r1]
  adds  r1, r1, #4

LoopCopyDataInit:
  ldr   r0, =_sdata
  ldr   r3, =_edata
  adds  r2, r0, r1
  cmp   r2, r3
  bcc   CopyDataInit
  ldr   r2, =_sbss
  b     LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs  r3, #0
  str   r3, [r2], #4

LoopFillZerobss:
  ldr   r3, = _ebss
  cmp   r2, r3
  bcc   FillZerobss

              //  BL     SysFreq





/*				LDR      r4,[pc,#32]
				LDR      r5,[pc,#32]
				B        0xbc
				 LDM      r4!,{r0-r3}
				 SUBS     r4,r4,#0x10
				 BLX      r3
				 ADDS     r4,r4,#0x10
				   CMP      r4,r5
				    BCC     0xac     */
				  //  LDR      r0,[pc,#0]
/*
				    movs  r1, #0
  b     LoopCopyDataInit

CopyDataInit:
  ldr   r3, =_sidata
  ldr   r3, [r3, r1]
  str   r3, [r0, r1]
  adds  r1, r1, #4

LoopCopyDataInit:
  ldr   r0, =_sdata
  ldr   r3, =_edata
  adds  r2, r0, r1
  cmp   r2, r3
  bcc   CopyDataInit*/

                BL      main

 test:
		       .word 0x81fc00
 Stack_Top:
  			   .word 0x820000

MainExit:
                B       MainExit

                .end
;
