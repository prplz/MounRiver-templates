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
#include "drv_int_ctrl_pub_api.h"
#include "pmsleep-irqs.h"
#include "registers.h"
#include "ms_platform.h"

struct gpio_int_handler_t {
	struct CamOsListHead_t		link;
	u32							nIRQNum;
	void						*pUsrParam;
	PfnIntcISR					pvIntIsrCB;
};

static Ms_Mutex_t	MsOS_VintPMMutex;
static u8 gbvIntPMMuxinit;
static struct CamOsListHead_t _vint_pm_list = {&_vint_pm_list,&_vint_pm_list};

#define PMGPIO_OEN              BIT0
#define PMGPIO_OUTPUT           BIT1
#define PMGPIO_INPUT            BIT2
#define PMGPIO_FIQ_MASK         BIT4
#define PMGPIO_FIQ_FROCE        BIT5
#define PMGPIO_FIQ_CLEAR        BIT6
#define PMGPIO_FIQ_POLARITY     BIT7
#define PMGPIO_FIQ_FINAL_STATUS BIT8
#define PMGPIO_FIQ_RAW_STATUS   BIT9

#ifdef CONFIG_PM_SLEEP
#define PM_FIQ_NUM              (INT_PMSLEEP_PM_SPI_DI - PMSLEEP_FIQ_START + 1)

/**
 * struct ms_pm_irq_priv - private pm interrupt data
 * @fiq_flags:      Flags of each pm-gpio fiq
 * @irq_polarity:   Polarity of pm-sleep irq
 */
struct ms_pm_irq_priv {
    U16     fiq_flags[PM_FIQ_NUM];
    U8      irq_polarity;
};

static struct ms_pm_irq_priv pm_irq_priv;
#endif

static void ss_pm_irq_ack(u32 ulPMIRQNum)
{
    U16 pmsleep_fiq = ulPMIRQNum;

    CamOsPrintf(KERN_DEBUG"[%s] hw:%d \n",__FUNCTION__, pmsleep_fiq);

    if(pmsleep_fiq < PMSLEEP_FIQ_END)
    {
        SETREG16(BASE_REG_PMGPIO_PA + ((pmsleep_fiq-PMSLEEP_FIQ_START) << 2), PMGPIO_FIQ_CLEAR);
        INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
    }
    else
    {
    	CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, pmsleep_fiq);
    }
}

static void ss_pm_irq_mask(u32 ulPMIRQNum)
{
    U16 pmsleep_fiq = ulPMIRQNum;

    CamOsPrintf(KERN_DEBUG"[%s] hw:%d \n",__FUNCTION__, pmsleep_fiq);

    if(pmsleep_fiq < PMSLEEP_FIQ_END)
    {
        SETREG16(BASE_REG_PMGPIO_PA + ((pmsleep_fiq-PMSLEEP_FIQ_START) << 2), PMGPIO_FIQ_MASK);
        INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
    }
    else if(pmsleep_fiq < PMSLEEP_IRQ_END)
    {
        SETREG8(BASE_REG_PMSLEEP_PA + REG_ID_08, 1<<(pmsleep_fiq-PMSLEEP_IRQ_START) );
        INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
    }
    else
    {
    	CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, pmsleep_fiq);
    }
}

static void ss_pm_irq_unmask(u32 ulPMIRQNum)
{
    U16 pmsleep_fiq = ulPMIRQNum;

    CamOsPrintf(KERN_DEBUG"[%s] hw:%d \n",__FUNCTION__, pmsleep_fiq);

    if(pmsleep_fiq < PMSLEEP_FIQ_END)
    {
        CLRREG16(BASE_REG_PMGPIO_PA + ((pmsleep_fiq-PMSLEEP_FIQ_START) << 2), PMGPIO_FIQ_MASK);
    }
    else if(pmsleep_fiq < PMSLEEP_IRQ_END)
    {
        CLRREG8(BASE_REG_PMSLEEP_PA + REG_ID_08, 1<<(pmsleep_fiq-PMSLEEP_IRQ_START) );
    }
    else
    {
    	CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, pmsleep_fiq);
        return;
    }

    CLRREG16(BASE_REG_INTRCTL_PA + REG_ID_54, BIT2);
    INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
}

static void ss_pm_irq_set_type(u32 type, u32 ulPMIRQNum)
{
    U16 pmsleep_fiq = ulPMIRQNum;
	struct gpio_int_handler_t *pTmpHandle;
	struct CamOsListHead_t *l;

    CamOsPrintf(KERN_DEBUG"%s %d type:0x%08x\n", __FUNCTION__, __LINE__, type);

    if(pmsleep_fiq < PMSLEEP_FIQ_END)
    {
        switch(type)
        {
        case INTC_POLARITY_LOW:
        	ss_pm_irq_mask(pmsleep_fiq);
            SETREG16(BASE_REG_PMGPIO_PA + ((pmsleep_fiq-PMSLEEP_FIQ_START) << 2), PMGPIO_FIQ_POLARITY);
            ss_pm_irq_ack(pmsleep_fiq);
            break;
        case INTC_POLARITY_HIGH:
        	ss_pm_irq_mask(pmsleep_fiq);
            CLRREG16(BASE_REG_PMGPIO_PA + ((pmsleep_fiq-PMSLEEP_FIQ_START) << 2), PMGPIO_FIQ_POLARITY);
            ss_pm_irq_ack(pmsleep_fiq);
            break;
        default:
            return;
        }
    }
    else if(pmsleep_fiq < PMSLEEP_IRQ_END)
    {
        switch(type)
        {
        case INTC_POLARITY_LOW:
        	ss_pm_irq_mask(pmsleep_fiq);
            SETREG8(BASE_REG_PMSLEEP_PA + REG_ID_09, 1<<(pmsleep_fiq-PMSLEEP_IRQ_START) );
            ss_pm_irq_ack(pmsleep_fiq);
            break;
        case INTC_POLARITY_HIGH:
        	ss_pm_irq_mask(pmsleep_fiq);
            CLRREG8(BASE_REG_PMSLEEP_PA + REG_ID_09, 1<<(pmsleep_fiq-PMSLEEP_IRQ_START) );
            ss_pm_irq_ack(pmsleep_fiq);
            break;
        default:
            return;
        }
    }
    else
    {
    	CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, pmsleep_fiq);
        return;
    }

	//unmask if gpio has been registered
    CAM_OS_LIST_FOR_EACH( l, &_vint_pm_list ) {
		pTmpHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pTmpHandle->nIRQNum == pmsleep_fiq) {
			ss_pm_irq_unmask(pmsleep_fiq);
			return;
		}
	}
}

static void ss_pm_virq_free(u32 eIntNum)
{
	struct gpio_int_handler_t *pRmHandle;
	struct CamOsListHead_t *l;

	CAM_OS_LIST_FOR_EACH( l, &_vint_pm_list ) {
		pRmHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pRmHandle->nIRQNum == eIntNum) {

			MsMutexLock(&MsOS_VintPMMutex);
			CAM_OS_LIST_DEL(&pRmHandle->link);
			MsMutexUnlock(&MsOS_VintPMMutex);
			CamOsMemRelease(pRmHandle);

			ss_pm_irq_mask(eIntNum);
			ss_pm_irq_ack(eIntNum);
			return;
		}
	}
}

static s32 ss_pm_virq_request(IntInitParam_u *puParam, IntNumber_e eIntNum)
{
	struct gpio_int_handler_t *pNewHandle, *pTmpHandle;
	struct CamOsListHead_t *l;

	if(!gbvIntPMMuxinit) {
		MsInitMutex(&MsOS_VintPMMutex);
		gbvIntPMMuxinit = 1;
	}

	//not allow double request
	CAM_OS_LIST_FOR_EACH( l, &_vint_pm_list ) {
		pTmpHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pTmpHandle->nIRQNum == eIntNum) {
			return -1;
		}
	}

	pNewHandle = CamOsMemAlloc(sizeof(struct gpio_int_handler_t));
	if(!pNewHandle) return -1;

	pNewHandle->nIRQNum = eIntNum;
	pNewHandle->pvIntIsrCB = puParam->intc.pfnIsr;
	pNewHandle->pUsrParam = puParam->intc.pDevId;

	MsMutexLock(&MsOS_VintPMMutex);
	CAM_OS_LIST_ADD_TAIL(&pNewHandle->link,&_vint_pm_list);
	MsMutexUnlock(&MsOS_VintPMMutex);

	ss_pm_irq_mask(eIntNum);
	ss_pm_irq_ack(eIntNum);
	ss_pm_irq_unmask(eIntNum);

	return 0;
}

static void ss_handle_cascade_pm_irq(u32 ulHWIRQNum, void * pParam)
{
	struct gpio_int_handler_t *pTmpHandle;
	struct CamOsListHead_t *l;
    unsigned int cascade_irq = 0xFFFFFFFF, i;
    unsigned int final_status;

    INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted

    for(i=PMSLEEP_FIQ_START;i<PMSLEEP_FIQ_END;i++)
    {
        final_status = INREG16(BASE_REG_PMGPIO_PA+(i-PMSLEEP_FIQ_START)*4);
        if(final_status & PMGPIO_FIQ_FINAL_STATUS)
        {
            cascade_irq = i;
            CamOsPrintf(KERN_DEBUG"[%s] Get hwirq:%d, Reg:0x%04x\n", __FUNCTION__, cascade_irq, final_status);
            break;
        }
    }

    if(0xFFFFFFFF==cascade_irq)
    {
    	CamOsPrintf(KERN_ERR"[%s:%d] error final_status:%d 0x%04X\n", __FUNCTION__, __LINE__, cascade_irq, final_status);
        return;
    }

    ss_pm_irq_mask(cascade_irq);

    CAM_OS_LIST_FOR_EACH( l, &_vint_pm_list ) {
		pTmpHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pTmpHandle->nIRQNum == cascade_irq) {
			pTmpHandle->pvIntIsrCB((cascade_irq << 16 | ulHWIRQNum),pTmpHandle->pUsrParam);
		}
	}

	ss_pm_irq_ack(cascade_irq);
	ss_pm_irq_unmask(cascade_irq);
}

struct vint_handler_t ss_pm_vintc = {
	.link = {0,0},
    .nHWIRQNum = INT_IRQ_PM_SLEEP,
	.pParam = NULL,
	.pvIntIsrCB = ss_handle_cascade_pm_irq,
	.pvIntRequest = ss_pm_virq_request,
	.pvIntSetPolarity = ss_pm_irq_set_type,
	.pvIntFree = ss_pm_virq_free
};

#ifdef CONFIG_PM_SLEEP
static int ms_pm_intc_suspend(void)
{
    unsigned int i;

    for (i = 0; i < PM_FIQ_NUM; i++) {
        pm_irq_priv.fiq_flags[i] = INREG16(BASE_REG_PMGPIO_PA + (i << 2));
        pm_irq_priv.fiq_flags[i] |= PMGPIO_FIQ_MASK; // always set mask here, unmask will be set by irq/pm.c
    }
    pm_irq_priv.irq_polarity = INREG8(BASE_REG_PMSLEEP_PA + REG_ID_09);

    pr_debug("ms_pm_intc_suspend\n\n");
    return 0;
}

static void ms_pm_intc_resume(void)
{
    unsigned int i;

    for (i = 0; i < PM_FIQ_NUM; i++) {
        OUTREG16(BASE_REG_PMGPIO_PA + (i << 2), pm_irq_priv.fiq_flags[i]);
    }
    OUTREG8(BASE_REG_PMSLEEP_PA + REG_ID_09, pm_irq_priv.irq_polarity);
    pr_debug("ms_pm_intc_resume\n\n");
}

struct syscore_ops ms_pm_intc_syscore_ops = {
    .suspend = ms_pm_intc_suspend,
    .resume = ms_pm_intc_resume,
};
#endif
