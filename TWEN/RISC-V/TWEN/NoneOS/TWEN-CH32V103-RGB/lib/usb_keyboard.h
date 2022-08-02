
#ifndef usb_keyboard_h
#define usb_keyboard_h
#include <CH32V103.h>



#define DevEP0SIZE 0x40
#define KEY_NONE 0x00
#define KEY_UP 0x1A
#define KEY_DOWN 0x16
#define KEY_LEFT 0x04
#define KEY_RIGHT 0x07
#define KEY_A 0x18
#define KEY_B 0x0C
#define KEY_C 0x0D
#define KEY_D 0x0E

#define KEY_L_CTRL 0x01
#define KEY_L_SHIFT 0x02
#define KEY_L_ALT 0x04
#define KEY_L_WIN 0x08
#define KEY_R_CTRL 0x10
#define KEY_R_SHIFT 0x20
#define KEY_R_ALT 0x40
#define KEY_R_WIN 0x80
#define MY_VID  		0x0808
#define MY_PID  		0x0606

const char jk[96][2] =
    {
        {0, 44}, //(space) 32
        {2, 30}, //! 33
        {2, 52}, //" 34
        {0, 50}, //# 35
        {2, 33}, //$ 36
        {2, 34}, //% 37
        {2, 36}, //& 38
        {0, 52}, //' 39
        {2, 38}, //( 40
        {2, 39}, //) 41
        {2, 37}, //* 42
        {2, 46}, //+ 43
        {0, 54}, //, 44
        {0, 45}, //- 45
        {0, 55}, //. 46
        {0, 56}, /// 47
        {0, 39}, //0 48
        {0, 30}, //1 49
        {0, 31}, //2 50
        {0, 32}, //3 51
        {0, 33}, //4 52
        {0, 34}, //5 53
        {0, 35}, //6 54
        {0, 36}, //7 55
        {0, 37}, //8 56
        {0, 38}, //9 57
        {2, 51}, //: 58
        {0, 51}, //; 59
        {2, 54}, //< 60
        {0, 46}, //= 61
        {2, 55}, //> 62
        {2, 56}, //? 63
        {2, 31}, //@ 64
        {2, 4},  //A 65
        {2, 5},  //B 66
        {2, 6},  //C 67
        {2, 7},  //D 68
        {2, 8},  //E 69
        {2, 9},  //F 70
        {2, 10}, //G 71
        {2, 11}, //H 72
        {2, 12}, //I 73
        {2, 13}, //J 74
        {2, 14}, //K 75
        {2, 15}, //L 76
        {2, 16}, //M 77
        {2, 17}, //N 78
        {2, 18}, //O 79
        {2, 19}, //P 80
        {2, 20}, //Q 81
        {2, 21}, //R 82
        {2, 22}, //S 83
        {2, 23}, //T 84
        {2, 24}, //U 85
        {2, 25}, //V 86
        {2, 26}, //W 87
        {2, 27}, //X 88
        {2, 28}, //Y 89
        {2, 29}, //Z 90
        {0, 47}, //[ 91
        {0, 49}, //\ 92
        {0, 48}, //] 93
        {2, 35}, //^ 94
        {2, 45}, //_ 95
        {0, 53}, //` 96
        {0, 4},  //a 97
        {0, 5},  //b 98
        {0, 6},  //c 99
        {0, 7},  //d 100
        {0, 8},  //e 101
        {0, 9},  //f 102
        {0, 10}, //g 103
        {0, 11}, //h 104
        {0, 12}, //i 105
        {0, 13}, //j 106
        {0, 14}, //k 107
        {0, 15}, //l 108
        {0, 16}, //m 109
        {0, 17}, //n 110
        {0, 18}, //o 111
        {0, 19}, //p 112
        {0, 20}, //q 113
        {0, 21}, //r 114
        {0, 22}, //s 115
        {0, 23}, //t 116
        {0, 24}, //u 117
        {0, 25}, //v 118
        {0, 26}, //w 119
        {0, 27}, //x 120
        {0, 28}, //y 121
        {0, 29}, //z 122
        {2, 47}, //{ 123
        {2, 49}, //| 124
        {2, 48}, //} 125
        {2, 53}, //~ 126
};
/* Device Descriptor */
const UINT8 MyDevDescr[] =
    {
    0x12,
    0x01, 
    0x10,
    0x01, 
    0x00, 
    0x00, 
    0x00,
    0x40/*bMaxPacketSize*/,
    (MY_VID>>0)&0xFF/*idVendor*/,
    (MY_VID>>8)&0xFF/*idVendor*/,
    (MY_PID>>8)&0xFF/*idProduct*/,
    (MY_PID>>8)&0xFF/*idProduct*/, 
    0x00/*bcdDev*/, 
    0x01/*bcdDev*/,
    0x01/*iManugacturer*/, 
    0x02/*iProduct*/, 
    0x03/*iSerialNum*/, 
    0x01/*bNumConfig*/
};

const UINT8 Joystick_ReportDescriptor[] =
    {
   0x05,          /*Usage Page(Generic Desktop)*/
   0x01,
   0x09,          /*Usage(Mouse)*/
   0x02,
   0xA1,          /*Collection(Logical)*/
   0x01,
	 0x85,0x01,
   0x09,          /*Usage(Pointer)*/
   0x01,
   /* 8 */
   0xA1,          /*Collection(Linked)*/
   0x00,
   0x05,          /*Usage Page(Buttons)*/
   0x09,
   0x19,          /*Usage Minimum(1)*/
   0x01,
   0x29,          /*Usage Maximum(3)*/
   0x03,
   /* 16 */
   0x15,          /*Logical Minimum(0)*/
   0x00,
   0x25,          /*Logical Maximum(1)*/
   0x01,
   0x95,          /*Report Count(3)*/
   0x03,
   0x75,          /*Report Size(1)*/
   0x01,
   /* 24 */
   0x81,          /*Input(Variable)*/
   0x02,
   0x95,          /*Report Count(1)*/
   0x01,
   0x75,          /*Report Size(5)*/
   0x05,
   0x81,          /*Input(Constant,Array)*/
   0x01,
   /* 32 */
   0x05,          /*Usage Page(Generic Desktop)*/
   0x01,
   0x09,          /*Usage(X axis)*/
   0x30,
   0x09,          /*Usage(Y axis)*/
   0x31,
   0x09,          /*Usage(Wheel)*/
   0x38,
   /* 40 */
   0x15,          /*Logical Minimum(-127)*/
   0x81,
   0x25,          /*Logical Maximum(127)*/
   0x7F,
   0x75,          /*Report Size(8)*/
   0x08,
   0x95,          /*Report Count(3)*/
   0x03,
   /* 48 */
   0x81,          /*Input(Variable, Relative)*/
   0x06,
   0xC0,          /*End Collection*/
   0x09,
   0x3c,
   0x05,
   0xff,
   0x09,
   /* 56 */
   0x01,
   0x15,
   0x00,
   0x25,
   0x01,
   0x75,
   0x01,
   0x95,
   /* 64 */
   0x02,
   0xb1,
   0x22,
   0x75,
   0x06,
   0x95,
   0x01,
   0xb1,
   /* 72 */
   0x01,
   0xc0,
	 
	  0x05,
    0x01,
    0x09,
    0x06,
    0xa1,
    0x01,
		0x85,0x02,
    0x05,
    0x07,
    0x19,
    0xe0,
    0x29,
    0xe7,
    0x15,
    0x00,
    0x25,
    0x01,
    0x95,
    0x08,
    0x75,
    0x01,
    0x81,
    0x02,
    0x95,
    0x01,
    0x75,
    0x08,
    0x81,
    0x03,
    0x95,
    0x06,
    0x75,
    0x08,
    0x15,
    0x00,
    0x25,
    0xff,
    0x05,
    0x07,
    0x19,
    0x00,
    0x29,
    0x65,
    0x81,
    0x00,
    0x25,
    0x01,
    0x95,
    0x05,
    0x75,
    0x01,
    0x05,
    0x08,
    0x19,
    0x01,
    0x29,
    0x05,
    0x91,
    0x02,
    0x95,
    0x01,
    0x75,
    0x03,
    0x91,
    0x03,
    0xc0,
};



/* Configration Descriptor */
const UINT8 MyCfgDescr[] =
    {
    0x09,
    0x02,
    0x22,
    0x00,
    0x01,
    0x01,
    0x00,
    0xA0,
    0x32,
    0x09,
    0x04,
    0x00,
    0x00,
    0x01,
    0x03,
    0x01,
    0x01,
    0x00,
    0x09,
    0x21,
    0x10,
    0x01,
    0x21,
    0x01,
    0x22,
    143,
    0x00,
    0x07,
    0x05,
    0x81,
    0x03,
    0x40,
    0x00/*bInterval*/,
    0x05/*bInterval*/
};
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

const UINT8 MyProductIDInfo[] = {0x0E, 0x03, 'Z', 0, 'L', 0, 'Y', 0, '0', 0, '0', 0, '1', 0};

/* Language Descriptor */
const UINT8 MyLangDescr[] = {0x04, 0x03, 0x09, 0x04};

/* Manufactor Descriptor */
const UINT8 MyManuInfo[] = {0x0E, 0x03, 'T', 0, 'w', 0, 'e', 0, 'n', 0, '5', 0, '1', 0};

/* Product Information */
const UINT8 MyProdInfo[] = {0x0C, 0x03, 'C', 0, 'H', 0, '3', 0, '2', 0, 'V', 0};

/**********************************************************/
volatile UINT8 Ready = 0;
volatile UINT8 Endp1Busy = 0; //传输完成控制标志位
volatile UINT8 Endp2Busy = 0;
UINT8 DevConfig;
UINT8 SetupReqCode;
UINT16 SetupReqLen;
UINT8 HIDKey[9];
UINT8 HIDMouse[5];
const UINT8 *pDescr;

/* Endpoint Buffer */
__attribute__((aligned(4))) UINT8 EP0_Databuf[64];      //ep0(64)
__attribute__((aligned(4))) UINT8 EP1_Databuf[64 + 64]; //ep1_out(64)+ep1_in(64)
__attribute__((aligned(4))) UINT8 EP2_Databuf[64 + 64]; //ep2_out(64)+ep2_in(64)
extern "C"
{
    void USBHD_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
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

    intflag = R8_USB_INT_FG;

    if (intflag & RB_UIF_TRANSFER)
    {
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
                    switch (SetupReqCode)
                    {
                    case 0x01: //GetReport
                        len = 1;
                        pEP0_DataBuf[0] = 0xaa;
                        break;
                    case 0x0a:
                        R8_UEP0_T_LEN = 0;
                        break; //这个一定要有
                    case 0x09:
                        Ready = 1;
                        break;
                    default:
                        errflag = 0xFF;
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

                        case USB_DESCR_TYP_REPORT:
                            if (((pSetupReqPak->wIndex) & 0xff) == 0) //接口0报表描述符
                            {
                                pDescr = Joystick_ReportDescriptor; //数据准备上传
                                len = sizeof(Joystick_ReportDescriptor);
                            }
                            else
                                len = 0xff; //本程序只有个接口，这句话正常不可能执行
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
                        bDeviceState=ADDRESSED;
                        break;

                    case USB_GET_CONFIGURATION:
                        pEP0_DataBuf[0] = DevConfig;
                        if (SetupReqLen > 1)
                            SetupReqLen = 1;
                        break;

                    case USB_SET_CONFIGURATION:
                        DevConfig = (pSetupReqPak->wValue) & 0xff;
                        bDeviceState=CONFIGURED;
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
            break;

        case UIS_TOKEN_OUT:
            len = R8_USB_RX_LEN;
            break;

        case UIS_TOKEN_OUT | 1:
            if (R8_USB_INT_ST & RB_UIS_TOG_OK)
            {
                len = R8_USB_RX_LEN;
                DevEP1_OUT_Deal(len);
            }
            break;

        case UIS_TOKEN_IN | 1:
            R8_UEP1_T_LEN = 0;
            Endp1Busy = 0;
            R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
            break;

        case UIS_TOKEN_OUT | 2:
            if (R8_USB_INT_ST & RB_UIS_TOG_OK)
            {
                len = R8_USB_RX_LEN;
                DevEP2_OUT_Deal(len);
            }
            break;

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
        R8_UEP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
        R8_UEP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
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

void keyBoard_value(char spcKey,char nomKey0,char nomKey1,char nomKey2,
					char nomKey3,char nomKey4,char nomKey5) 	
{
	  HIDKey[0] = 0x02; //id=2
    HIDKey[1] = spcKey;
    HIDKey[2] = 0;
    HIDKey[3] = nomKey0;
    HIDKey[4] = nomKey1;
    HIDKey[5] = nomKey2;
    HIDKey[6] = nomKey3;
    HIDKey[7] = nomKey4;
    HIDKey[8] = nomKey5;
    while (Endp1Busy)
    {
        ; //如果忙（上一包数据没有传上去），则等待。
    }
    Endp1Busy = 1; //设置为忙状态
    memcpy(pEP1_IN_DataBuf, HIDKey, 9);
    DevEP1_IN_Deal(9);
}
void Mouse_value(char nomKey1, char nomKey2,
                 char nomKey3, char nomKey4)
{
        HIDMouse[0] = 0x01; //id=1
    HIDMouse[1] = nomKey1;
    HIDMouse[2] = nomKey2;
    HIDMouse[3] = nomKey3;
    HIDMouse[4] = nomKey4;

    while (Endp1Busy)
    {
        ; //如果忙（上一包数据没有传上去），则等待。
    }
    Endp1Busy = 1; //设置为忙状态
    memcpy(pEP1_IN_DataBuf, HIDMouse, 5);
    DevEP1_IN_Deal(5);
}
void Moue_Send(char key1, char key2, char key3, char key4)
{

   
	Mouse_value(key1,key2,key3,key4);	

	Mouse_value(KEY_NONE,KEY_NONE,KEY_NONE,KEY_NONE);
	

}

void keyBoard_Send(char spcKey,char nomKey0,char nomKey1,char nomKey2,
					char nomKey3,char nomKey4,char nomKey5)
{
  
	keyBoard_value(spcKey,nomKey0,nomKey1,nomKey2,nomKey3,nomKey4,nomKey5);	//???д????????
   
	keyBoard_value(KEY_NONE,KEY_NONE,KEY_NONE,KEY_NONE,KEY_NONE,KEY_NONE,KEY_NONE);//????	

}

void keyBoard_Send_String(char* s,uint32_t len)
{ 

   int i=0; 
   do{ 

    keyBoard_Send(jk[s[i]-0x20][0],jk[s[i]-0x20][1],0,0,0,0,0);

    i++;
   }while(i<len);

}

void keyBoard_String(String s)
{
  char temp[s.length()];
  memset(temp,0,s.length());
  strcpy(temp,s.c_str());
  keyBoard_Send_String(temp,s.length());
 

}
void keyBoard_Run(String s)
{
keyBoard_Send(8,21,0,0,0,0,0);
delay(300);
keyBoard_String(s);
delay(100);
keyBoard_Send(0,40,0,0,0,0,0);
delay(300);
}
void keyBoard_Number(int s)
{
  String t="";
  keyBoard_String(t+s);
 
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

void usb_keymouse_init()
{
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    pEP2_RAM_Addr = EP2_Databuf;

    USBHD_ClockCmd(RCC_USBCLKSource_PLLCLK_1Div5, ENABLE);
    USB_DeviceInit();
    NVIC_EnableIRQ(USBHD_IRQn);
    bDeviceState=UNCONNECTED;
}


/*******************************************************************************
* Function Name  : DevEP1_OUT_Deal
* Description    : Deal device Endpoint 1 OUT.
* Input          : l: Data length.
* Return         : None
*******************************************************************************/
void DevEP1_OUT_Deal(UINT8 l)
{
    ;
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