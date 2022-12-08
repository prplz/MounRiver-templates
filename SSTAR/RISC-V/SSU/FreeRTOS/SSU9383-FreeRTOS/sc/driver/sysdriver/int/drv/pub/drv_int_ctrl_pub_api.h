/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/


#ifndef __DRV_INT_CTRL_PUB_API_H__
#define __DRV_INT_CTRL_PUB_API_H__

/*=============================================================*/
// Include files
/*=============================================================*/

#include "hal_int_ctrl_pub.h"
#include "cam_os_wrapper.h"

/*=============================================================*/
// Macro definition
/*=============================================================*/

#define VINT_TO_HW_INT_MASK     (0x0000FFFF)
#define VINT_NUM_OFFSET         (16)

/**
 *Virtual Interrupt Request type
 */
typedef s32 (*PfnVIntRequest)(IntInitParam_u *puParam, IntNumber_e eIntNum);

/**
 *Virtual Interrupt Polarity type
 */
typedef void (*PfnVIntPolarity)(u32 type, u32 eIntNum);

/**
 *Virtual Interrupt Free function
 */
typedef void (*PfnVIntFree)(u32 eIntNum);

struct vint_handler_t
{
    struct CamOsListHead_t  link;
    u32                     nHWIRQNum;
    void                    *pParam;
    PfnIntcISR              pvIntIsrCB;
    PfnVIntRequest          pvIntRequest;
    PfnVIntPolarity         pvIntSetPolarity;
    PfnVIntFree             pvIntFree;
};

/*=============================================================*/
// Variable definition
/*=============================================================*/

extern struct vint_handler_t ss_pm_vintc;
extern struct vint_handler_t ss_gpi_vintc;

/*=============================================================*/
// Global function definition
/*=============================================================*/
/**
 *
 * \brief Initialize the interrupt controller. The interrupts are initialized to be automatically
 *  acknowledged. All pending acknowledged interrupts before this call are cleared
 */
void DrvInitInterruptController(void);

#if defined(CONFIG_POWER_MANAG_SUPPORT)
void DrvResumeInterruptController(void);
#endif

/**
  *
  * \brief Initialize a interrupt and mask it
  * \param[in] psParam Parameters to initialize the interrupt
  * \param[in] u8IntNum Interrupt number
  * \return 0 if the result is OK. -1 otherwise.
  */
s32 DrvInitInterrupt(IntInitParam_u *puParam, IntNumber_e eIntNum);

/**
  *
  * \brief register virtual interrupt callback on a specified HW IRQ
  * \param[in] psParam Parameters to initialize the interrupt
  * \param[in] pNewVintHandler->nHWIRQNum HW interrupt number
  * \param[in] pNewVintHandler->pvIntIsrCB real HW interrupt callback
  * \param[in] pNewVintHandler->pvIntRequest IRQ request will be redirect to this callback function
  * \return 0 if the result is OK. -1 otherwise.
  */
int DrvRegisterVirtualInterrupt(IntInitParam_u *puParam, struct vint_handler_t * pNewVintHandler);

/**
  * \brief Clear a interrupt
  * \param[in] u8IntNum Interrupt line number
  */
void DrvClearInterrupt(IntNumber_e eIntNum);

/**
  * \brief Mask a interrupt
  * \param[in] u8IntNum Interrupt line number
  */
void DrvMaskInterrupt(IntNumber_e eIntNum);

/**
  * \brief Unmask a interrupt
  * \param[in] u8IntNum Interrupt line number
  */
void DrvUnmaskInterrupt(IntNumber_e eIntNum);

/**
  * \brief check an interrupt mask
  * \param[in] u8IntNum Interrupt line number
  * \retval    bool     TRUE: interrupt not masked, FALSE: interrupt masked.
  */
bool DrvIsMaskInterrupt(IntNumber_e eIntNum);

/**
  * \brief Free interrupt ISR callback
  * \param[in] nIntcNumber is interrupt number
  * \param[in] pDevId is device ID
  * \retval    N/A
  */
void DrvIntcFreeIsr(u32 nIntcNumber, void* pDevId);

/**
  * \brief Report statistics of interrupt
  */
void DrvShowInterruptCount(void);

/** DrvIntcSetPolarity
* \brief        Setup polarity register on INTC
*
* \param[in]    ePolarity           Polarity.
* \param[in]    nIntcNumber         Interrupt number.
*
* \return       N/A
*
*/
void DrvIntcSetPolarity(IntcPolarity_e ePolarity, u32 nIntcNumber);

/**
  * \brief Get interrupt number of current ISR
  * \param[in] N/A
  * \retval    interrupt number
  */
u32 IntGetCurrentIrqNum(void);

/**
  * \brief Send SGI
  * \param[in] cpu    CPU ID.
  * \param[in] no     Interrupt number.
  * \retval    N/A
  */
void DrvIntSendSGI(u32 cpu, u32 no);

#endif // __DRV_INT_CTRL_PUB_API_H__
