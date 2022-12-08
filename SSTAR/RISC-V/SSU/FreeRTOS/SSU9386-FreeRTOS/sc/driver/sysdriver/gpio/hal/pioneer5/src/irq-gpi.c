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
#include "gpi-irqs.h"
#include "registers.h"
#include "ms_platform.h"

struct gpio_int_handler_t {
	struct CamOsListHead_t		link;
	u32							nIRQNum;
	void						*pUsrParam;
	PfnIntcISR					pvIntIsrCB;
};

static Ms_Mutex_t	MsOS_VintGPIMutex;
static u8 gbvIntGPIMuxinit;
static struct CamOsListHead_t _vint_gpi_list = {&_vint_gpi_list,&_vint_gpi_list};

#ifdef CONFIG_PM_SLEEP
/**
 * struct ss_gpi_irq_priv - private gpi interrupt data
 * @polarity:   fiq polarity
 */
struct ss_gpi_irq_priv {
    U16     gpi_polarity[(GPI_FIQ_NUM+15)>>4];
    U16     gpic_polarity[(GPI_GPIC_NUM+15)>>4];
};

static struct ss_gpi_irq_priv gpi_irq_priv;
#endif

static void ss_gpi_irq_ack(u32 ulGPIIRQNum)
{
    U16 gpi_irq = ulGPIIRQNum;

    if( gpi_irq >= 0 && gpi_irq < (GPI_FIQ_NUM + GPI_GPIC_NUM) )
    {
        if(gpi_irq < GPI_GPIC_START)
        {
            SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_20 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
            INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
        }
        else
        {
            SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_7A + ((gpi_irq - GPI_GPIC_START)/16)*4 ) , (1 << ((gpi_irq - GPI_GPIC_START)%16)) );
            INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
        }

    }
    else
    {
        CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, gpi_irq);
    }
}

static void ss_gpi_irq_mask(u32 ulGPIIRQNum)
{
    U16 gpi_irq = ulGPIIRQNum;

    CamOsPrintf(KERN_DEBUG"[%s] hw:%d \n",__FUNCTION__, gpi_irq);

    if( gpi_irq >= 0 && gpi_irq < (GPI_FIQ_NUM + GPI_GPIC_NUM) )
    {
        if(gpi_irq < GPI_GPIC_START)
        {
            SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_00 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
            INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
        }
        else
        {
            SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_78 + ((gpi_irq - GPI_GPIC_START)/16)*4 ) , (1 << ((gpi_irq - GPI_GPIC_START)%16)) );
            INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
        }
    }
    else
    {
        CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, gpi_irq);
    }
}

static void ss_gpi_irq_unmask(u32 ulGPIIRQNum)
{
    U16 gpi_irq = ulGPIIRQNum;

    CamOsPrintf(KERN_DEBUG"[%s] hw:%d \n",__FUNCTION__, gpi_irq);

    if( gpi_irq >= 0 && gpi_irq < (GPI_FIQ_NUM + GPI_GPIC_NUM) )
    {
        if(gpi_irq < GPI_GPIC_START)
        {
            CLRREG16( (BASE_REG_GPI_INT_PA + REG_ID_00 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
        }
        else
        {
            CLRREG16( (BASE_REG_GPI_INT_PA + REG_ID_78 + ((gpi_irq - GPI_GPIC_START)/16)*4 ) , (1 << ((gpi_irq - GPI_GPIC_START)%16)) );
        }
    }
    else
    {
        CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, gpi_irq);
        return;
    }
    CLRREG16(BASE_REG_INTRCTL_PA + REG_ID_57, BIT8);
    INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted
}


static void ss_gpi_irq_set_type(u32 type, u32 ulGPIIRQNum)
{
    U16 gpi_irq = ulGPIIRQNum;
	struct gpio_int_handler_t *pTmpHandle;
	struct CamOsListHead_t *l;

    CamOsPrintf(KERN_DEBUG"%s %d type:0x%08x\n", __FUNCTION__, __LINE__, type);

    if( gpi_irq >= 0 && gpi_irq < GPI_FIQ_END )
    {
        switch(type)
        {
            case INTC_POLARITY_LOW:
                ss_gpi_irq_mask(gpi_irq);
                if(gpi_irq < GPI_GPIC_START)
                {
                    CLRREG16((BASE_REG_GPI_INT_PA + REG_ID_40 + (gpi_irq / 16) * 4),
                             (1 << (gpi_irq % 16))); // clear the both edge setting
                    SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_30 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
                }
                else
                {
                    SETREG16( (BASE_REG_GPI2_INT_PA + REG_ID_7B + ((gpi_irq - GPI_GPIC_START)/16)*4 ) , (1 << ((gpi_irq - GPI_GPIC_START)%16)) );
                }
                ss_gpi_irq_ack(gpi_irq);
                break;
            case INTC_POLARITY_HIGH:
                ss_gpi_irq_mask(gpi_irq);
                if(gpi_irq < GPI_GPIC_START)
                {
                    CLRREG16((BASE_REG_GPI_INT_PA + REG_ID_40 + (gpi_irq / 16) * 4),
                             (1 << (gpi_irq % 16))); // clear the both edge setting
                    CLRREG16( (BASE_REG_GPI_INT_PA + REG_ID_30 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
                }
                else
                {
                    CLRREG16( (BASE_REG_GPI2_INT_PA + REG_ID_7B + ((gpi_irq - GPI_GPIC_START)/16)*4 ) , (1 << ((gpi_irq - GPI_GPIC_START)%16)) );
                }
                ss_gpi_irq_ack(gpi_irq);
                break;
            case INTC_POLARITY_BOTH:
                ss_gpi_irq_mask(gpi_irq);
                if(gpi_irq < GPI_GPIC_START)
                {
                    SETREG16( (BASE_REG_GPI_INT_PA + REG_ID_40 + (gpi_irq/16)*4 ) , (1 << (gpi_irq%16)) );
                }
                else
                {
                }
                ss_gpi_irq_ack(gpi_irq);
                break;
            default:
                return;

        }
    }
    else
    {
    	CamOsPrintf(KERN_ERR"[%s] Unknown hwirq %lu\n", __func__, gpi_irq);
        return;
    }

	//unmask if gpio has been registered
    CAM_OS_LIST_FOR_EACH( l, &_vint_gpi_list ) {
		pTmpHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pTmpHandle->nIRQNum == gpi_irq) {
			ss_gpi_irq_unmask(gpi_irq);
			return;
		}
	}
}

static void ss_gpi_virq_free(u32 eIntNum)
{
	struct gpio_int_handler_t *pRmHandle;
	struct CamOsListHead_t *l;

	CAM_OS_LIST_FOR_EACH( l, &_vint_gpi_list ) {
		pRmHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pRmHandle->nIRQNum == eIntNum) {

			MsMutexLock(&MsOS_VintGPIMutex);
			CAM_OS_LIST_DEL(&pRmHandle->link);
			MsMutexUnlock(&MsOS_VintGPIMutex);
			CamOsMemRelease(pRmHandle);

			ss_gpi_irq_mask(eIntNum);
			ss_gpi_irq_ack(eIntNum);
			return;
		}
	}
}

static s32 ss_gpi_virq_request(IntInitParam_u *puParam, u32 eIntNum)
{
	struct gpio_int_handler_t *pNewHandle, *pTmpHandle;
	struct CamOsListHead_t *l;

	if(!gbvIntGPIMuxinit) {
		MsInitMutex(&MsOS_VintGPIMutex);
		gbvIntGPIMuxinit = 1;
	}

	//not allow double request
	CAM_OS_LIST_FOR_EACH( l, &_vint_gpi_list ) {
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

	MsMutexLock(&MsOS_VintGPIMutex);
	CAM_OS_LIST_ADD_TAIL(&pNewHandle->link,&_vint_gpi_list);
	MsMutexUnlock(&MsOS_VintGPIMutex);

	ss_gpi_irq_mask(eIntNum);
	ss_gpi_irq_ack(eIntNum);
	ss_gpi_irq_unmask(eIntNum);

	return 0;
}

static void ss_handle_cascade_gpi(u32 ulHWIRQNum, void * pParam)
{
	struct gpio_int_handler_t *pTmpHandle;
	struct CamOsListHead_t *l;
    unsigned int cascade_irq = 0xFFFFFFFF, i, j;
    unsigned int final_status;

    INREG16(BASE_REG_MAILBOX_PA);//read a register make ensure the previous write command was compeleted

    for (j=0; j<=GPI_FIQ_NUM/16; j++)
    {
        final_status = INREG16(BASE_REG_GPI_INT_PA + REG_ID_50 + j*4);
        for(i=0; i<16 && final_status!=0; i++)
        {
            if(0 !=(final_status & (1<<i)))
            {
                cascade_irq = i + j*16;
                break;
            }
        }
    }

    for (j=0; j<=GPI_GPIC_NUM/16; j++)
    {
        final_status = INREG16(BASE_REG_GPI_INT_PA + REG_ID_7C + j*4);
        for(i=0; i<16 && final_status!=0; i++)
        {
            if(0 !=(final_status & (1<<i)))
            {
                cascade_irq = i + j*16;
                cascade_irq += GPI_GPIC_START;
                break;
            }
        }
    }

    if(0xFFFFFFFF==cascade_irq)
    {
    	CamOsPrintf(KERN_ERR"[%s:%d] error final_status:%d 0x%04X\n", __FUNCTION__, __LINE__, cascade_irq, final_status);
        return;
    }

    ss_gpi_irq_mask(cascade_irq);

    CAM_OS_LIST_FOR_EACH( l, &_vint_gpi_list ) {
		pTmpHandle = CAM_OS_LIST_ENTRY( l, struct gpio_int_handler_t, link );
		if(pTmpHandle->nIRQNum == cascade_irq) {
			pTmpHandle->pvIntIsrCB((cascade_irq << 16 | ulHWIRQNum),pTmpHandle->pUsrParam);
		}
	}

	ss_gpi_irq_ack(cascade_irq);
	ss_gpi_irq_unmask(cascade_irq);
}

struct vint_handler_t ss_gpi_vintc = {
	.link = {0,0},
    .nHWIRQNum = INT_IRQ_GPI_OUT,
	.pParam = NULL,
	.pvIntIsrCB = ss_handle_cascade_gpi,
	.pvIntRequest = ss_gpi_virq_request,
	.pvIntSetPolarity = ss_gpi_irq_set_type,
	.pvIntFree = ss_gpi_virq_free
};

#ifdef CONFIG_PM_SLEEP
static int ss_gpi_intc_suspend(void)
{
    unsigned int i, num;

    num = (GPI_FIQ_NUM + 15) >> 4;
    for (i = 0; i < num; i++) {
        gpi_irq_priv.gpi_polarity[i] = INREG16(BASE_REG_GPI_INT_PA + REG_ID_30 + (i << 2));
    }

    num = (GPI_GPIC_NUM + 15) >> 4;
    for (i = 0; i < num; i++) {
        gpi_irq_priv.gpic_polarity[i] = INREG16(BASE_REG_GPI2_INT_PA + REG_ID_7B + (i << 2));
    }

    pr_debug("ss_gpi_intc_suspend\n\n");
    return 0;
}

static void ss_gpi_intc_resume(void)
{
    unsigned int i, num;

    num = (GPI_FIQ_NUM + 15) >> 4;
    for (i = 0; i < num; i++) {
        OUTREG16(BASE_REG_GPI_INT_PA + REG_ID_30 + (i << 2), gpi_irq_priv.gpi_polarity[i]);
    }

    num = (GPI_GPIC_NUM + 15) >> 4;
    for (i = 0; i < num; i++) {
        OUTREG16(BASE_REG_GPI2_INT_PA + REG_ID_7B + (i << 2), gpi_irq_priv.gpic_polarity[i]);
    }
    pr_debug("ss_gpi_intc_resume\n\n");
}

struct syscore_ops ss_gpi_intc_syscore_ops = {
    .suspend = ss_gpi_intc_suspend,
    .resume = ss_gpi_intc_resume,
};
#endif
