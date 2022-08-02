#ifndef __USB_com_H
#define __USB_com_h

#include "CH32V103.h"
#define DevEP0SIZE 0x40
uint32_t SOF_Count=0;
extern "C"
{

  uint8_t buffer_out[129];
  uint8_t buffer_in[129];
  uint8_t len_UsbToUart;
  uint8_t len_UartToUsb;
  uint8_t dtime = 0;
  __IO uint8_t bIntPackSOF;

  __IO uint32_t count_out;
  __IO uint32_t count_in;
  __IO uint8_t empty_flag;
  __IO uint8_t status_UsbUart = 0;
  const UINT8 MyDevDescr[] =
      {
          0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, DevEP0SIZE,
          0xc4, 0x10, 0x60, 0xea, 0x00, 0x01, 0x01, 0x02,
          0x03, 0x01

  };
  const UINT8 MyCfgDescr[] =
      {
          0x09, 0x02, 0x20, 0,
          0x01, 0x01, 0x00, 0x80, 0x32, //配置描述符（1个接口）
          //以下为接口0（数据接口）描述符
          0x09, 0x04, 0x00, 0x00, 0x02, 0xff, 0x00, 0x00, 0x02, //数据接口描述符
          0x07, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00,             //端点描述符 EP1 BULK OUT
          0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,             //端点描述符 EP2 BULK OUT
                                                                //以下为接口01（数据接口）描述符
                                                                //0x09, 0x04, 0x01, 0x00, 0x02, 0xff, 0xff, 0xff, 0x00,	 //数据接口描述符
                                                                //0x07, 0x05, 0x83, 0x02, 0x40, 0x00, 0x00,				 //端点描述符 EP3 BULK IN
                                                                //0x07, 0x05, 0x04, 0x02, 0x40, 0x00, 0x00,				 //端点描述符 EP4 BULK OUT

  };
  const UINT8 MyProductIDInfo[] = {0x16,0x03,0x38,0x00,0x39,0x00,0x33,0x00,0x32,0x00,0x31,0x00,0x39,0x00,0x33,0x00,0x36,0x00,0x41,0x00,0x32,0x00};

  /* Language Descriptor */
  const UINT8 MyLangDescr[] = {0x04, 0x03, 0x09, 0x04};

  /* Manufactor Descriptor */
  const UINT8 MyManuInfo[] = {0x0E, 0x03, 'T', 0, 'w', 0, 'e', 0, 'n', 0, '5', 0, '1', 0};

  /* Product Information */
  const UINT8 MyProdInfo[] = {0x0E, 0x03, 'T', 0, 'w', 0, 'e', 0, 'n', 0, '5', 0, '1', 0};

  const UINT8 databuf[] =
      {
          0x42, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x1B, 0xB7, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3F, 0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x07, 0x10, 0x0F, 0x00,
          0x07, 0x1F, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x2E,
          0x00, 0x30, 0x00};

  typedef enum _DEVICE_STATE
  {
    UNCONNECTED,
    ATTACHED,
    POWERED,
    SUSPENDED,
    ADDRESSED,
    CONFIGURED
  } DEVICE_STATE;

  __IO uint32_t bDeviceState; /* USB device status */
  volatile UINT8 Ready = 0;
  volatile UINT8 Endp1Busy = 0; //传输完成控制标志位
  volatile UINT8 Endp2Busy = 0;
  UINT8 DevConfig;
  UINT8 SetupReqCode;
  UINT16 SetupReqLen;

  const UINT8 *pDescr;

  /* Endpoint Buffer */
  __attribute__((aligned(4))) UINT8 EP0_Databuf[64];      //ep0(64)
  __attribute__((aligned(4))) UINT8 EP1_Databuf[64 + 64]; //ep1_out(64)+ep1_in(64)
  __attribute__((aligned(4))) UINT8 EP2_Databuf[64+64];      //ep2_out(64)+ep2_in(64)

  void USBHD_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
}

const uint8_t HexToAscTab[] = "0123456789ABCDEF";

void uuidcpy(uint8_t *dest, uint8_t index, uint8_t len) /* 使用UUID生成USB Serial Number */
{
  uint8_t i;
  uint8_t p = 0; /* UUID格式, 十位十六进制数 */
  const uint8_t *puuid;
  for (i = index; i < (index + len); i++)
  {
    if (i == 0)
      dest[p++] = 22; //10 * 2 + 2
    else if (i == 1)
      dest[p++] = 0x03;
    else
    {
      if (i & 0x01) //奇数
      {
        dest[p++] = 0x00;
      }
      else
      {
        puuid = (const uint8_t *)(0x3ffa + (i - 2) / 4);
        if (i & 0x02)
          dest[p++] = HexToAscTab[(*puuid) >> 4];
        else
          dest[p++] = HexToAscTab[(*puuid) & 0x0f];
      }
    }
  }
}
/*******************************************************************************
* Function Name  : USB_DevTransProcess
* Description    : USB device transfer process.
* Input          : None
* Return         : None
*******************************************************************************/
void USB_DevTransProcess(void)
{
  UINT8 len, chtype;
  UINT8 intflag, errflag = 0;
  UINT8 soft_dtr;

  intflag = R8_USB_INT_FG;

  if (intflag & RB_UIF_TRANSFER)
  {
    if ((R8_USB_INT_ST & MASK_UIS_TOKEN) == UIS_TOKEN_SOF)
    {
      SOF_Count++;
    }
    switch (R8_USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
    {
    case UIS_TOKEN_SETUP:
      R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
      len = R8_USB_RX_LEN;

      if (len == sizeof(USB_SETUP_REQ))
      {
        SetupReqLen = pSetupReqPak->wLength;
        SetupReqCode = pSetupReqPak->bRequest;
        chtype = pSetupReqPak->bRequestType;

        len = 0;
        errflag = 0;

        if ((pSetupReqPak->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD)
        {
          if ((pSetupReqPak->bRequestType == 0xc0) && (SetupReqCode == 0xff))
          {
            EP0_Databuf[0] = 0x04;
            len = 1;
          }
          if ((pSetupReqPak->bRequestType == 0xc1) && (SetupReqCode == 0x04))
          {
            EP0_Databuf[0] = 0x00;
            EP0_Databuf[1] = 0x08;
            len = 2;
          }
          else if ((pSetupReqPak->bRequestType == 0xc1) && (SetupReqCode == 0x0f))
          {

            len = sizeof(databuf);
            len = SetupReqLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupReqLen; //本次传输长度
            memcpy(EP0_Databuf, databuf, len);                                          //加载上传数据

            //同步标志位翻转
          }
          else if ((pSetupReqPak->bRequestType == 0xc0) && (SetupReqCode == 0x10))
          {

            soft_dtr = EP0_Databuf[6];
            for (len = 0; len < soft_dtr; len++)
              EP0_Databuf[len] = 0;
            len = soft_dtr;
          }
          else if ((pSetupReqPak->bRequestType == 0xc1) && (SetupReqCode == 0x08))
          {
            soft_dtr = EP0_Databuf[6];
            for (len = 0; len < soft_dtr; len++)
              EP0_Databuf[len] = 0;
            len = soft_dtr;
          }
          else if ((pSetupReqPak->bRequestType == 0x41) && (SetupReqCode == 0x03)) //校验位处理，没有验证
          {
            if (EP0_Databuf[2] == 0x20) //0x10,0x20,0x30 奇、偶、1，0
            {
            }
            else
            {
            }
          }
          else if ((pSetupReqPak->bRequestType == 0x41) && (SetupReqCode == 0x07)) //RTS DTR处理
          {
            if (((pSetupReqPak->wValue) >> 8) & 0x01)
            {
              if (((pSetupReqPak->wValue) & 0xff) & 0x01) //DTR
              {
              }
              else
              {
              }
            }
            if (((pSetupReqPak->wValue) >> 8) & 0x02)
            {
              if (((pSetupReqPak->wValue) & 0xff) & 0x02) //RTS
              {
              }
              else
              {
              }
            }
          }
        }
        else
        {

          switch (SetupReqCode)
          {
          case USB_GET_DESCRIPTOR:
          {
            switch (((pSetupReqPak->wValue) >> 8))
            {
            case USB_DESCR_TYP_DEVICE:
              pDescr = MyDevDescr;
              len = MyDevDescr[0];
              break;

            case USB_DESCR_TYP_CONFIG:
              pDescr = MyCfgDescr;
              len = MyCfgDescr[2];
              break;

            case USB_DESCR_TYP_STRING:
              switch ((pSetupReqPak->wValue) & 0xff)
              {
              case 0:
                pDescr = MyLangDescr;
                len = MyLangDescr[0];
                break;

              case 1:
                pDescr = MyManuInfo;
                len = MyManuInfo[0];
                break;

              case 2:
                pDescr = MyProdInfo;
                len = MyProdInfo[0];
                break;

              case 3:
                pDescr = MyProductIDInfo;
                len = MyProductIDInfo[0];
                break;

              default:
                errflag = 0xFF;
                break;
              }
              break;

            default:
              errflag = 0xff;
              break;
            }

            if (SetupReqLen > len)
              SetupReqLen = len;
            len = (SetupReqLen >= DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
            memcpy(pEP0_DataBuf, pDescr, len);
            pDescr += len;
          }
          break;

          case USB_SET_ADDRESS:
            SetupReqLen = (pSetupReqPak->wValue) & 0xff;
            bDeviceState = ADDRESSED;
            break;

          case USB_GET_CONFIGURATION:
            pEP0_DataBuf[0] = DevConfig;
            if (SetupReqLen > 1)
              SetupReqLen = 1;
            break;

          case USB_SET_CONFIGURATION:
            DevConfig = (pSetupReqPak->wValue) & 0xff;
            bDeviceState = CONFIGURED;
            break;

          case USB_CLEAR_FEATURE:
            if ((pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)
            {
              switch ((pSetupReqPak->wIndex) & 0xff)
              {
              case 0x82:
                R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                break;

              case 0x02:
                R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                break;

              case 0x81:
                R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                break;

              case 0x01:
                R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                break;

              default:
                errflag = 0xFF;
                break;
              }
            }
            else
              errflag = 0xFF;
            break;

          case USB_GET_INTERFACE:
            pEP0_DataBuf[0] = 0x00;
            if (SetupReqLen > 1)
              SetupReqLen = 1;
            break;

          case USB_GET_STATUS:
            pEP0_DataBuf[0] = 0x00;
            pEP0_DataBuf[1] = 0x00;
            if (SetupReqLen > 2)
              SetupReqLen = 2;
            break;

          default:
            errflag = 0xff;
            break;
          }
        }
      }
      else
        errflag = 0xff;

      if (errflag == 0xff)
      {
        R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;
      }
      else
      {
        if (chtype & 0x80)
        {
          len = (SetupReqLen > DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
          SetupReqLen -= len;
        }
        else
          len = 0;

        R8_UEP0_T_LEN = len;
        R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;
      }
      break;

    case UIS_TOKEN_IN:
      if ((SetupReqCode == 0x0f))
      {
        memcpy(EP0_Databuf, databuf + 64, 2);
        R8_UEP0_T_LEN = 2;
        R8_UEP0_CTRL ^= RB_UEP_T_TOG;
      }
      else
      {
        switch (SetupReqCode)
        {
        case USB_GET_DESCRIPTOR:
          len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen;
          memcpy(pEP0_DataBuf, pDescr, len);
          SetupReqLen -= len;
          pDescr += len;
          R8_UEP0_T_LEN = len;
          R8_UEP0_CTRL ^= RB_UEP_T_TOG;
          break;

        case USB_SET_ADDRESS:
          R8_USB_DEV_AD = (R8_USB_DEV_AD & RB_UDA_GP_BIT) | SetupReqLen;
          R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
          break;

        default:
          R8_UEP0_T_LEN = 0;
          R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
          break;
        }
      }
      break;

    case UIS_TOKEN_OUT:
      len = R8_USB_RX_LEN;
      break;

    case UIS_TOKEN_OUT | 1:
      if (R8_USB_INT_ST & RB_UIS_TOG_OK)
      {
        R8_UEP1_CTRL = R8_UEP1_CTRL & ~MASK_UEP_R_RES | UEP_R_RES_ACK;
        len = R8_USB_RX_LEN;
        DevEP1_OUT_Deal(len);
      }

      break;

    // case UIS_TOKEN_IN | 1:
    //   R8_UEP1_T_LEN = 0;
    //   Endp1Busy = 0;
    //   R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;

    //   break;



    case UIS_TOKEN_IN | 2:
      R8_UEP2_T_LEN = 0;
      Endp2Busy = 0;
      R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
      break;

    default:
      break;
    }
    R8_USB_INT_FG = RB_UIF_TRANSFER;
  }
  else if (intflag & RB_UIF_BUS_RST)
  {
    R8_USB_DEV_AD = 0;
    R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
    R8_UEP1_CTRL = UEP_R_RES_ACK | RB_UEP_AUTO_TOG;
    R8_UEP2_CTRL = UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
    R8_USB_INT_FG |= RB_UIF_BUS_RST;
    bDeviceState = ATTACHED;
  }
  else if (intflag & RB_UIF_SUSPEND)
  {
    if (R8_USB_MIS_ST & RB_UMS_SUSPEND)
    {
      ;
    }
    else
    {
      ;
    }
    R8_USB_INT_FG = RB_UIF_SUSPEND;
    bDeviceState = ATTACHED;
  }
  else
  {
    R8_USB_INT_FG = intflag;
  }
}

/*******************************************************************************
* Function Name  : Set_USBConfig
* Description    : Set USB clock.
* Input          : None
* Return         : None
*******************************************************************************/
void USBHD_ClockCmd(UINT32 RCC_USBCLKSource, FunctionalState NewState)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, NewState);
  EXTEN->EXTEN_CTR |= EXTEN_USBHD_IO_EN;
  RCC_USBCLKConfig(RCC_USBCLKSource); //USBclk=PLLclk/1.5=48Mhz
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHD, NewState);
}

void usb_com()
{
  pEP0_RAM_Addr = EP0_Databuf;
  pEP1_RAM_Addr = EP1_Databuf;
  pEP2_RAM_Addr = EP2_Databuf;


  USBHD_ClockCmd(RCC_USBCLKSource_PLLCLK_1Div5, ENABLE);
  USB_DeviceInit();
  NVIC_EnableIRQ(USBHD_IRQn);
  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : DevEP1_OUT_Deal
* Description    : Deal device Endpoint 1 OUT.
* Input          : l: Data length.
* Return         : None
*******************************************************************************/
void DevEP1_OUT_Deal(UINT8 l)
{

  for(int i=0;i<l;i++)
  buffer_out[count_out+i]=EP1_Databuf[i];
  count_out=count_out+l;
 
  
}

/*******************************************************************************
* Function Name  : DevEP2_OUT_Deal
* Description    : Deal device Endpoint 2 OUT.
* Input          : l: Data length.
* Return         : None
*******************************************************************************/
void DevEP2_OUT_Deal(UINT8 l)
{
  ;
}

/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB exception.
* Input          : None
* Return         : None
*******************************************************************************/

void USBHD_IRQHandler(void)
{
  USB_DevTransProcess();
}

#endif