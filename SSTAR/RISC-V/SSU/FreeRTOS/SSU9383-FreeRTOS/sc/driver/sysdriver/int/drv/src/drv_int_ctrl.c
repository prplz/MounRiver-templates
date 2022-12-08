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

#define __DRV_INT_CTRL_C__

/*=============================================================*/
// Include files
/*=============================================================*/

#include "drv_int_ctrl_pub_api.h"
#include "hal_int_ctrl_pub.h"
#include "sys_MsWrapper_cus_os_sem.h"

/*=============================================================*/
// Variable definition
/*=============================================================*/

static Ms_Mutex_t MsOS_vIntMutex;
static u8 gbvIntMuxinit;
static struct CamOsListHead_t _vint_list = {&_vint_list, &_vint_list};

/*=============================================================*/
// Global function definition
/*=============================================================*/

void DrvInitInterruptController(void)
{
    HalIntInitIntController();
}

#if defined(CONFIG_POWER_MANAG_SUPPORT)
void DrvResumeInterruptController(void)
{
    HalIntResumeIntController();
}
#endif

s32 DrvInitInterrupt(IntInitParam_u *puParam, IntNumber_e eIntNum)
{
    struct vint_handler_t *h;
    struct CamOsListHead_t *l;

    // Redirect IRQ request to virtual IRQ callback function
    CAM_OS_LIST_FOR_EACH( l, &_vint_list )
    {
        h = CAM_OS_LIST_ENTRY( l, struct vint_handler_t, link );
        if (h && (h->nHWIRQNum == (eIntNum & VINT_TO_HW_INT_MASK)))
        {
            return h->pvIntRequest(puParam,eIntNum >> VINT_NUM_OFFSET);
        }
    }

    return HalIntInitInterrupt(puParam, eIntNum & VINT_TO_HW_INT_MASK);
}

int DrvRegisterVirtualInterrupt(IntInitParam_u *puParam, struct vint_handler_t * pNewVintHandler)
{
    struct vint_handler_t *h;
    struct CamOsListHead_t *l;

    if (!gbvIntMuxinit)
    {
        MsInitMutex(&MsOS_vIntMutex);
        gbvIntMuxinit = 1;
    }

    MsMutexLock(&MsOS_vIntMutex);

    // Check If IRQ Number exist
    CAM_OS_LIST_FOR_EACH( l, &_vint_list )
    {
        h = CAM_OS_LIST_ENTRY( l, struct vint_handler_t, link );
        if (pNewVintHandler->nHWIRQNum == h->nHWIRQNum)
        {
            MsMutexUnlock(&MsOS_vIntMutex);
            return -1;
        }
    }

    // Chain list
    CAM_OS_LIST_ADD_TAIL(&pNewVintHandler->link,&_vint_list);
    puParam->intc.pDevId = pNewVintHandler->pParam;
    MsMutexUnlock(&MsOS_vIntMutex);

    return HalIntInitInterrupt(puParam, pNewVintHandler->nHWIRQNum & VINT_TO_HW_INT_MASK);
}

void DrvAckInterrupt(IntNumber_e eIntNum)
{
    //HalAckInterrupt(eIntNum & VINT_TO_HW_INT_MASK);
}

void DrvClearInterrupt(IntNumber_e eIntNum)
{
    //HalClearInterrupt(eIntNum & VINT_TO_HW_INT_MASK);
}

void DrvMaskInterrupt(IntNumber_e eIntNum)
{
    HalIntMaskInterrupt(eIntNum & VINT_TO_HW_INT_MASK);
}

void DrvUnmaskInterrupt(IntNumber_e eIntNum)
{
    HalIntUnmaskInterrupt(eIntNum & VINT_TO_HW_INT_MASK);
}

bool DrvIsMaskInterrupt(IntNumber_e eIntNum)
{
    return HalIntIsMaskInterrupt(eIntNum & VINT_TO_HW_INT_MASK);
}

void DrvIntcFreeIsr(u32 nIntcNumber, void* pDevId)
{
    struct vint_handler_t *h;
    struct CamOsListHead_t *l;

    // IRQ free to virtual IRQ free function
    CAM_OS_LIST_FOR_EACH( l, &_vint_list )
    {
        h = CAM_OS_LIST_ENTRY( l, struct vint_handler_t, link );
        if (h && (h->nHWIRQNum == (nIntcNumber & VINT_TO_HW_INT_MASK)))
        {
            h->pvIntFree(nIntcNumber >> VINT_NUM_OFFSET);
            return;
        }
    }

    HalIntFreeIsr(nIntcNumber & VINT_TO_HW_INT_MASK, pDevId);
}

void DrvShowInterruptCount(void)
{
    HalIntShowInterruptStat();
}

void DrvIntcSetPolarity(IntcPolarity_e ePolarity, u32 nIntcNumber)
{
    struct vint_handler_t *h;
    struct CamOsListHead_t *l;

    // Redirect IRQ SetPolarity to virtual IRQ SetPolarity function
    CAM_OS_LIST_FOR_EACH( l, &_vint_list )
    {
        h = CAM_OS_LIST_ENTRY( l, struct vint_handler_t, link );
        if (h && (h->nHWIRQNum == (nIntcNumber & VINT_TO_HW_INT_MASK)))
        {
            h->pvIntSetPolarity(ePolarity,nIntcNumber >> VINT_NUM_OFFSET);
            return;
        }
    }

    HalIntSetInterruptPolarity(ePolarity, nIntcNumber & VINT_TO_HW_INT_MASK);
}

u32 IntGetCurrentIrqNum(void)
{
    return HalIntGetCurrentIrqNum();
}

void DrvIntSendSGI(u32 cpu, u32 no)
{
    if (MsIsCoreReady(cpu))
    {
        HalIntSendSGI(cpu, no);
    }
}

#if defined(__arm) && !defined(__GNUC__)
#pragma arm
#endif

void IntcIrqHandler(void)
{
    HalIntCommonHandler(INTC_MAP_IRQ);
}

void IntcFiqHandler(void)
{
    HalIntCommonHandler(INTC_MAP_FIQ);
}

#if defined(__arm) && !defined(__GNUC__)
#pragma thumb
#endif
