
#ifndef usb_midi_h
#define usb_midi_h
#include <CH32V103.h>


#define DevEP0SIZE 0x40
#define KEY_NONE 0x00
#define KEY_UP 0x1A
#define KEY_DOWN 0x16

#define MY_VID  		0x0808
#define MY_PID  		0x0606

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
    0x05/*iSerialNum*/, 
    0x01/*bNumConfig*/
};

const UINT8 Midi_ReportDescriptor[143] =
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
    0x09, /* bLength: Configuation Descriptor size */
    0x02, /* bDescriptorType: Configuration */
    101,
    /* wTotalLength: Bytes returned */
    0x00,
    0x02,         /*bNumInterfaces: 2 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0x80,         /*bmAttributes: bus powered */
    0xC8,         /*MaxPower 400 mA: this current is used for detecting Vbus*/

    /************** Descriptor of standard audio control interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    0x04,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x00,         /*bNumEndpoints: no endpoints*/
    0x01,         /*bInterfaceClass: Audio device class*/
    0x01,         /*bInterfaceSubClass : Audio control*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0x00,            /*iInterface: Index of string descriptor*/
//class_specific_interface_descritor        //类特殊接口描述符：类特殊音频控制接口
 0x09,                          //类特殊接口描述符的字节数大小
 0x24,               //类特殊接口描述符类型编号
 0x01,                                //描述符子类
 0x00,
 0x01,                        //子类版本，1.0
 0x09,
 0x00,                        //类特殊描述大小
 0x01,                        //流接口数量
 0x01,                       //MIDI 流接口1属于此音频控制接口

//Standard MS Interface Descriptor
 0x09,                             //接口描述符的字节数大小
 0x04,             //接口描述符类型编号
 0x01,                             //接口编号为1
 0x00,                             //备用编号为0
 0x02,                             //非0端点数量为2
 0x01,                            //音频设备（AUDIO）类
 0x03,                    //使用的子类：MIDI流(MIDI STREAMING)类
 0x00,                             //未用
 0x00,                              //未用


//Class-specific MS Interface Descriptor
 0x07,                      //类特殊MS接口描述符的字节数大小
 0x24,   //类特殊接口描述符类型编号
 0x01,                 //描述符子类
 0x00,
 0x01,                     //子类版本，1.0
 0x41,
 0x00,                    //类特殊描述总长度

//MIDI IN Jack Descriptor(Embedded)
 0x06,                          //MIDI输入插孔描述符
 0x24,       //类特殊接口
 0x02,                  //MIDI_IN_JACK子类
 0x01,                      //EMBEDDED插孔类
 0x01,                          //该插口的ID
 0x00,                          //未用

//MIDI IN Jack Descriptor (External)
 0x06,                          //MIDI输入插孔描述符
 0x24,       //类特殊接口
 0x02,                  //MIDI_IN_JACK子类
 0x02,                      //EXTERNAL插孔类
 0x02,                          //该插口的ID
 0x00,                          //未用

//MIDI OUT Jack Descriptor
 0x09,                          //MIDI输入插孔描述符
 0x24,       //类特殊接口
 0x03,                 //MIDI_OUT_JACK子类
 0x01,                      //EMBEDDED插孔类
 0x03,                          //该插口的ID
 0x01,                          //该插口的输入引脚数
 0x02,                          //该引脚所在实体的ID
 0x01,                          //连接到该实体输出脚的输入脚的编号
 0x00,                          //未用

//MIDI OUT Jack Descriptor (External)
 0x09,                          //MIDI输入插孔描述符
 0x24,       //类特殊接口
 0x03,                 //MIDI_OUT_JACK子类
 0x02,                      //EXTERNAL插孔类
 0x04,                          //该插口的ID
 0x01,                          //该插口的输入引脚数
 0x02,                          //该引脚所在实体的ID
 0x01,                          //连接到该实体输出脚的输入脚的编号
 0x00,    

//Standard Bulk OUT Endpoint Descriptor
 0x09,                          //端点描述符的字节数大小
 0x05,           //端点描述符类型编号
 0x01,                //端点号，主输出端点
 0x02,            //使用的传输类型：批量传输
 0x40,
 0x00,                          //该端点支持的最大包尺寸，64字节
 0x0A,                          //中断扫描时间，无效
 0x00,                          //未用
 0x00,                          //未用

//Class-specific MS Bulk OUT Endpoint Descriptor
 0x05,                          //端点描述符的字节数大小
 0x25,        //端点描述符类型编号
 0x01,                    //MS_GENERAL子类
 0x01,                          //MIDI输入插孔数目
 0x01,                          //Embedded MIDI 输入插孔的ID

//Standard Bulk IN Endpoint Descriptor                                                                //主端点输出描述
 0x09,                          //端点描述符的字节数大小
 0x05,           //端点描述符类型编号
 0x81,                 //端点号，主输入端点
 0x02,            //使用的传输类型：批量传输
 0x40,
 0x00,                          //该端点支持的最大包尺寸，64字节
 0x0A,                          //中断扫描时间，无效
 0x00,                          //未用
 0x00,                          //未用

//Class-specific MS Bulk IN Endpoint Descriptor
 0x05,                          //端点描述符的字节数大小
 0x25,        //端点描述符类型编号
 0x01,                    //MS_GENERAL子类
 0x01,                          //MIDI输入插孔数目
 0x03     
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
const UINT8 MyProdInfo[] = {0x0E, 0x03, 'T', 0, 'w', 0, 'e', 0, 'n', 0, '5', 0, '1', 0};

/**********************************************************/
volatile UINT8 Ready = 0;
volatile UINT8 Endp1Busy = 0; //传输完成控制标志位
volatile UINT8 Endp2Busy = 0;
UINT8 DevConfig;
UINT8 SetupReqCode;
UINT16 SetupReqLen;
UINT8 Midi_data[4];

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
                                pDescr = Midi_ReportDescriptor; //数据准备上传
                                len = sizeof(Midi_ReportDescriptor);
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



void Midi_Send(uint8_t key1,uint8_t key2,uint8_t key3,uint8_t key4)
{
    Midi_data[0] = key1; //id=1
    Midi_data[1] = key2;
    Midi_data[2] = key3;
    Midi_data[3] = key4;


    while (Endp1Busy)
    {
        ; //如果忙（上一包数据没有传上去），则等待。
    }
    Endp1Busy = 1; //设置为忙状态
    memcpy(pEP1_IN_DataBuf, Midi_data, 4);
    DevEP1_IN_Deal(4);
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

void usb_midi_init()
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