#ifndef _CH32V_NRF24L01_H_
#define _CH32V_NRF24L01_H_

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

#define RX_DR	    0x40
#define TX_DS	    0x20
#define MAX_RT      0x10

// SPI(nRF24L01) commands
#define READ_REG    0x00		// Define read command to register
#define WRITE_REG   0x20		// Define write command to register
#define RD_RX_PLOAD 0x61		// Define RX payload register address
#define WR_TX_PLOAD 0xA0		// Define TX payload register address
#define FLUSH_TX    0xE1		// Define flush TX register command
#define FLUSH_RX    0xE2		// Define flush RX register command
#define REUSE_TX_PL 0xE3		// Define reuse TX payload register command
#define NOP         0xFF		// Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      0x00		// 'Config' register address
#define EN_AA       0x01		// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02		// 'Enabled RX addresses' register address
#define SETUP_AW    0x03		// 'Setup address width' register address
#define SETUP_RETR  0x04		// 'Setup Auto. Retrans' register address
#define RF_CH       0x05		// 'RF channel' register address
#define RF_SETUP    0x06		// 'RF setup' register address
#define STATUS      0x07		// 'Status' register address
#define OBSERVE_TX  0x08		// 'Observe TX' register address
#define CD          0x09		// 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A		// 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B		// 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C		// 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D		// 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E		// 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F		// 'RX address pipe5' register address
#define TX_ADDR     0x10		// 'TX address' register address
#define RX_PW_P0    0x11		// 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12		// 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13		// 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14		// 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15		// 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16		// 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17		// 'FIFO Status Register' register address

#define TX_ADR_WIDTH   5		// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32		// 数据通道有效数据宽度

#define NRF_HARDSPI     0
#define NRF_SOFTSPI     1

class NRF24L01
{
public:
    //使用模拟spi
    NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq, uint8_t sck, uint8_t miso, uint8_t mosi); //引脚分配
    //使用硬件spi
    NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq);
    void begin();     //初始化
    void set_rx_ch(uint8_t channel); //设置接收通道
    void set_rx_addr(uint8_t* address);   //设置接收地址
    void rx_setup();   //配置接收
    void set_tx_ch(uint8_t channel); //设置发送通道
    void set_tx_addr(uint8_t* address);  //设置发送地址
    void tx_setup(); //配置发送
    void write(uint8_t * BUF);   //发送数组
    void write(int shu);    //发送数值
    void write(String str);  //发送字符串
    void write(String name,int shu);    //发送变量
    uint8_t read(uint8_t* buf); //是否接收到数据并保存到buf中
    uint8_t read(); //是否接收到数据
    int read_num();  //读取数值
    String read_str();   //读取字符串
    int read_name(String name);  //读取变量的值

private:
    uint8_t _CE_pin;
    uint8_t _CSN_pin;
    uint8_t _IRQ_pin; 
    uint8_t _SCK_pin;
    uint8_t _MISO_pin;
    uint8_t _MOSI_pin;

    uint8_t _connect;

    uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// 定义一个静态发送地址
    uint8_t RX_ADDRESS[TX_ADR_WIDTH] = { 0x00, 0x66, 0x66, 0x66, 0x66 };	// 定义一个静态发送地址

    uint8_t RX_BUF[TX_PLOAD_WIDTH]; //接收缓存
    uint8_t ch=7;   //默认发送通道
    uint8_t rch=7;   //默认接收通道

    uint8_t SPI_RW(uint8_t dat);
    uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);
    uint8_t SPI_Read(uint8_t reg);
    uint8_t SPI_Read_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes);
    uint8_t SPI_Write_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes);
};

//========================================================================
// 描述: 引脚分配.
// 参数: none
// 返回: none.
//========================================================================
NRF24L01::NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq, uint8_t sck, uint8_t miso, uint8_t mosi)
{
    _connect = NRF_SOFTSPI;
    _CE_pin = ce;
    _CSN_pin = csn;
    _IRQ_pin = irq; 
    _SCK_pin = sck;
    _MISO_pin = miso;
    _MOSI_pin = mosi;
}

//========================================================================
// 描述: 引脚分配.
// 参数: none
// 返回: none.
//========================================================================
NRF24L01::NRF24L01(uint8_t ce, uint8_t csn, uint8_t irq)
{
    _connect = NRF_HARDSPI;
    _CE_pin = ce;
    _CSN_pin = csn;
    _IRQ_pin = irq; 
}

//========================================================================
// 描述: 根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01读出一字节.
// 参数: 写入的字节数据.
// 返回: 读出的字节数据.
//========================================================================
uint8_t NRF24L01::SPI_RW(uint8_t dat)
{
    uint8_t i;
    uint8_t datavalue=0;
    if(_connect == NRF_HARDSPI)     //硬件SPI
    {
        return hardspi.wr_data(dat);
    }else{                          //模拟SPI
        for(i=0;i<8;i++){
            if(dat&0x80){
                digitalWrite(_MOSI_pin, 1);
            }else{
                digitalWrite(_MOSI_pin, 0);
            }
            dat <<=1;
            digitalWrite(_SCK_pin, 1);
            datavalue <<=1;
            if(digitalRead(_MISO_pin)){                  
                datavalue |= 0x01;
            }
            digitalWrite(_SCK_pin, 0);
        }
        return datavalue;
    }
}

//========================================================================
// 描述: 写数据value到reg寄存器.
// 参数: reg:寄存器;value:数据.
// 返回: 状态.
//========================================================================
uint8_t NRF24L01::SPI_RW_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	digitalWrite (_CSN_pin, LOW);
	// CSN置低，开始传输数据
	status = SPI_RW (reg);		// 选择寄存器，同时返回状态字
	SPI_RW (value);				// 然后写数据到该寄存器
	digitalWrite (_CSN_pin, HIGH);		// CSN拉高，结束数据传输
	return (status);			// 返回状态寄存器
}

//========================================================================
// 描述: 从reg寄存器读一字节数据.
// 参数: reg:寄存器.
// 返回: 寄存器数据.
//========================================================================
uint8_t NRF24L01::SPI_Read(uint8_t reg)
{
	uint8_t reg_val;

	digitalWrite (_CSN_pin, LOW);		// CSN置低，开始传输数据
	SPI_RW (reg);				// 选择寄存器
	reg_val = SPI_RW (0);		// 然后从该寄存器读数据
	digitalWrite (_CSN_pin, HIGH);		// CSN拉高，结束数据传输
	return (reg_val);			// 返回寄存器数据
}

//========================================================================
// 描述: 从reg寄存器读出bytes个字节，通常用来读取接收通道数据或接收/发送地址.
// 参数: reg:寄存器;pBuf:返回的数据指针;bytes:需要读取的字节数;.
// 返回: 状态寄存器.
//========================================================================
uint8_t NRF24L01::SPI_Read_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes)
{
	uint8_t status, i;

	digitalWrite (_CSN_pin, LOW);		// CSN置低，开始传输数据
	status = SPI_RW (reg);		// 选择寄存器，同时返回状态字
	for (i = 0; i < bytes; i++)
    {
		pBuf[i] = SPI_RW (0);	// 逐个字节从nRF24L01读出
    }
	digitalWrite (_CSN_pin, HIGH);		// CSN拉高，结束数据传输
	return (status);			// 返回状态寄存器
}

//========================================================================
// 描述: 把pBuf缓存中的数据写入到nRF24L01，通常用来写入发射通道数据或接收/发送地址.
// 参数: reg:寄存器;pBuf:写入的数据指针;bytes:需要写入的字节数.
// 返回: 状态寄存器.
//========================================================================
uint8_t NRF24L01::SPI_Write_Buf (uint8_t reg, uint8_t * pBuf, uint8_t bytes)
{
	uint8_t status, i;

	digitalWrite (_CSN_pin, LOW);		// CSN置低，开始传输数据
	status = SPI_RW (reg);		// 选择寄存器，同时返回状态字
	for (i = 0; i < bytes; i++)
    {
        SPI_RW (pBuf[i]);		// 逐个字节写入nRF24L01
    }
	digitalWrite (_CSN_pin, HIGH);		// CSN拉高，结束数据传输
	return (status);			// 返回状态寄存器
}

//=======================================================================
// 描述: 无线初始化.
// 参数: none.
// 返回: 接收到字符串.
//========================================================================
void NRF24L01::begin()
{
	SPI_InitTypeDef  SPI_InitStructure;
    if(_connect == NRF_HARDSPI) //硬件spi初始化
    {
		hardspi.begin();
        hardspi.set_mode(0,0);
        hardspi.set_speed(2);   //nrf24l01的spi速率最大支持8Mbps，这里设为8分频
    }else{                      //模拟spi初始化
        pinMode(_SCK_pin, GPIO_Mode_Out_PP);
        pinMode(_MOSI_pin, GPIO_Mode_Out_PP);
        pinMode(_MISO_pin, GPIO_Mode_IN_FLOATING);
    }
	pinMode(_IRQ_pin,GPIO_Mode_IN_FLOATING);
    pinMode(_CE_pin,GPIO_Mode_Out_PP);
    pinMode(_CSN_pin,GPIO_Mode_Out_PP);
	digitalWrite(_IRQ_pin, 0);
	digitalWrite(_CE_pin, 0);		// chip enable  
	digitalWrite(_CSN_pin, 1);		// Spi disable     
}

/***************************************************无线接收配置************************************/
//=======================================================================
// 描述: 设置无线接收通道.
// 参数: 通道.
// 返回: none.
//========================================================================
void NRF24L01::set_rx_ch (uint8_t channel)
{
    rch=channel;
}

//========================================================================
// 描述: 设置无线接收地址.
// 参数: 发送地址指针.
// 返回: none.
//========================================================================
void NRF24L01::set_rx_addr (uint8_t* address)
{
 for(int i=0;i<5;i++)*(RX_ADDRESS+i)=*(address+i);
}

//========================================================================
// 描述: 配置无线接收.
// 参数: none.
// 返回: none.
//========================================================================
void NRF24L01::rx_setup ()
{
	digitalWrite (_CE_pin, LOW);

	SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, RX_ADDRESS, TX_ADR_WIDTH);	// 接收设备接收通道0使用和发送设备相同的发送地址
	SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	SPI_RW_Reg (WRITE_REG + RF_CH, rch);	// 选择射频通道0x40
	SPI_RW_Reg (WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);	// 接收通道0选择和发送通道相同有效数据宽度
	SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// 0x07数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	SPI_RW_Reg (WRITE_REG + CONFIG, 0x0f);	// CRC使能，16位CRC校验，上电，接收模式
    //SPI_RW_Reg (WRITE_REG + CONFIG, 0x03);	

	digitalWrite (_CE_pin, HIGH);// 拉高CE启动接收设备
}

/***************************************************无线发送配置************************************/
//=======================================================================
// 描述: 设置无线发送通道.
// 参数: 通道.
// 返回: none.
//========================================================================
void NRF24L01::set_tx_ch (uint8_t channel)
{
    ch=channel;
}

//=======================================================================
// 描述: 设置无线发送地址.
// 参数: 通道.
// 返回: none.
//========================================================================
void NRF24L01::set_tx_addr (uint8_t* address)
{
    for(int i=0;i<5;i++)*(TX_ADDRESS+i)=*(address+i);
}

//========================================================================
// 描述: 配置无线发送.
// 参数: none.
// 返回: none.
//========================================================================
void NRF24L01::tx_setup()
{
	digitalWrite (_CE_pin, LOW);

  	SPI_Write_Buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// 写入发送地址
	SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 为了应答接收设备，接收通道0地址和发送地址相同
	//SPI_Write_Buf (WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);	// 写数据包到TX FIFO
	//SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	SPI_RW_Reg (WRITE_REG + SETUP_RETR, 0x03);	// 自动重发延时等待250us+86us，自动重发10次
	SPI_RW_Reg (WRITE_REG + RF_CH, ch);	// 选择射频通道0x40
	SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	SPI_RW_Reg (WRITE_REG + CONFIG, 0x0e);	// CRC使能，16位CRC校验，上电
    //SPI_RW_Reg (WRITE_REG + CONFIG, 0x02);

	digitalWrite (_CE_pin, HIGH);
}

//=======================================================================
// 描述: 无线发送32个字节数据.
// 参数: buf:数据.
// 返回: none.
//========================================================================
void  NRF24L01::write(uint8_t * BUF)
{
    uint8_t sta;

	digitalWrite (_CE_pin, LOW);
	//SPI_Write_Buf (WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	// 写入发送地址
	//SPI_Write_Buf (WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 为了应答接收设备，接收通道0地址和发送地址相同
	SPI_Write_Buf (WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);	// 写数据包到TX FIFO
	//SPI_RW_Reg (WRITE_REG + EN_AA, 0x01);	// 使能接收通道0自动应答
	//SPI_RW_Reg (WRITE_REG + EN_RXADDR, 0x01);	// 使能接收通道0
	//SPI_RW_Reg (WRITE_REG + SETUP_RETR, 0x0a);	// 自动重发延时等待250us+86us，自动重发10次
	//SPI_RW_Reg (WRITE_REG + RF_CH, ch);	// 选择射频通道0x40
	//SPI_RW_Reg (WRITE_REG + RF_SETUP, 0x07);	// 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	//SPI_RW_Reg (WRITE_REG + CONFIG, 0x0e);	// CRC使能，16位CRC校验，上电
	digitalWrite (_CE_pin, HIGH);
    //delay(1);

    while(digitalRead(_IRQ_pin)!=0);
    sta = SPI_Read(STATUS);	  // 读状态寄存器
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
    if(sta&MAX_RT){
         SPI_RW_Reg(FLUSH_TX, 0xff);  // 清除RX_DS中断标志
     }
}

//=======================================================================
// 描述: 无线发送数字.
// 参数: none.
// 返回: none.
//========================================================================
void NRF24L01::write (int shu)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+String(shu)+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
        if(i>32)break;
        buf[i]=tx.charAt(i);
    }
    write(buf);
}

//=======================================================================
// 描述: 无线发送字符串.
// 参数: none.
// 返回: none.
//========================================================================
void NRF24L01::write(String str)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+str+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
    if(i>32)break;
    buf[i]=tx.charAt(i);

    }
    write(buf);
}

//=======================================================================
// 描述: 无线发送变量.
// 参数: none.
// 返回: none.
//========================================================================
void NRF24L01::write (String name,int shu)
{
    uint8_t buf[32];
    String tx="";
    tx=tx+name+"="+String(shu)+"\r\n";
    uint8_t len = tx.length();
    for(int i=0;i<len;i++)
    {
        if(i>32)break;
        buf[i]=tx.charAt(i);
    }
    write(buf);
}

//=======================================================================
// 描述: 无线接收存入buf中(最多接收32字节).
// 参数: buf:接收到的数据指针.
// 返回: 1,接收到数据;0,没有接收到数据.
//========================================================================
uint8_t NRF24L01::read(uint8_t* buf)
{
    uint8_t sta;
    sta = SPI_Read(STATUS);	  // 读状态寄存器
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
    if(RX_DR&sta)				  // 判断是否接受到数据
	{
        SPI_Read_Buf(RD_RX_PLOAD, buf, TX_PLOAD_WIDTH);  // 从RX FIFO读出数据
        SPI_RW_Reg(FLUSH_RX, 0xff);  // 清除RX_DS中断标志 
        return 1;
    }else{
         return 0;
    }
}

//=======================================================================
// 描述: 无线是否接收到数据.
// 参数: none.
// 返回: 1,接收到数据;0,没有接收到数据.
//========================================================================
uint8_t NRF24L01::read()
{
    uint8_t sta;
    sta = SPI_Read(STATUS);	  // 读状态寄存器
    SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除RX_DS中断标志
	if(RX_DR&sta)				  // 判断是否接受到数据
    {
        SPI_Read_Buf(RD_RX_PLOAD,RX_BUF, TX_PLOAD_WIDTH);  // 从RX FIFO读出数据
        SPI_RW_Reg(FLUSH_RX, 0xff);  // 清除RX_DS中断标志
        return 1;
    }else{
        return 0;
    }
}

//=======================================================================
// 描述: 无线接收数字.
// 参数: none.
// 返回: 接收到数字.
//========================================================================
int NRF24L01::read_num()
{
    String s;
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }
    return s.toInt();
}

//=======================================================================
// 描述: 无线接收字符串.
// 参数: none.
// 返回: 接收到字符串.
//========================================================================
String NRF24L01::read_str()
{
    String s;
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }
    return s;    
}

//=======================================================================
// 描述: 无线接收变量.
// 参数: none.
// 返回: 接收到字符串.
//========================================================================
int NRF24L01::read_name(String name)
{
    String s;
    int value;
    int namelen=name.length();
    for(int i=0;i<32;i++)
    {
        if(RX_BUF[i]==0x0d||RX_BUF[i]==0x0a)break;
        s+=char(RX_BUF[i]);
    }

    if(s.indexOf(name)>-1)value=s.substring(namelen+1,s.length()).toInt();
    return value;       
}

#endif  //CH32V_NRF24L01.h