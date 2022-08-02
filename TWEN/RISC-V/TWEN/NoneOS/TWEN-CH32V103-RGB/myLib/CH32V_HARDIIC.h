/**********************************************************
 * 该库为CH32V103的硬件I2C库.
 * 默认引脚为PB6(SCL),PB7(SDA)
 * *******************************************************/
#ifndef _CH32V_HARDIIC_H_
#define _CH32V_HARDIIC_H_

#include "CH32V103.h"

class KHardIIC{
public:
    KHardIIC();
    void begin(uint32_t bound); //初始化
    uint8_t read(uint8_t last=0); //读函数
    bool restart(uint8_t addr); //重复起始信号
    bool start(uint8_t addr); //起始信号
    void stop(void);  //停止信号
    bool write(uint8_t b);  //写函数
private:
    //SoftI2cMaster() {}
    uint8_t _sdapin;
    uint8_t _sclpin;
};

//========================================================================
// 描述: 构造函数.
// 参数: none
// 返回: none.
//========================================================================
KHardIIC::KHardIIC()
{

}

//========================================================================
// 描述: 初始化函数.
// 参数: none.
// 返回: none.
//========================================================================
void KHardIIC::begin(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitTSturcture;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	I2C_InitTSturcture.I2C_ClockSpeed = bound;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitTSturcture.I2C_OwnAddress1 = 0;
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );

	I2C_Cmd( I2C1, ENABLE );

	I2C_AcknowledgeConfig( I2C1, ENABLE );	
}

//========================================================================
// 描述: 发送起始信号以及设备地址.
// 参数: addr：设备地址.
// 返回: 0,没有接收到应答;1,接收到应答.
//========================================================================
bool KHardIIC::start(uint8_t addr)
{
    uint16_t i=0;
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
	I2C_GenerateSTART( I2C1, ENABLE );
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );//BUSY, MSL and SB flag
	I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Transmitter );   
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )   /* BUSY, MSL, ADDR, TXE and TRA flags */
    {
        i++;
        delayMicroseconds(50);
        if(i >= 2000)
        {
            i = 0;
            return 0;
        }
    }
    return 1;
}

//========================================================================
// 描述: 发送重复起始信号.
// 参数: none.
// 返回: none.
//========================================================================
bool KHardIIC::restart(uint8_t addr)
{
    uint16_t i=0;
	I2C_GenerateSTART( I2C1, ENABLE );
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, addr&0xFE, I2C_Direction_Receiver );
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) )
    {
        i++;
        delayMicroseconds(50);
        I2C_AcknowledgeConfig( I2C1, DISABLE );
        if(i>=2000)
        {
            return 0;
        }
    }
    i=0;
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) ==  RESET )
    {
        i++;
        delayMicroseconds(50);
        I2C_AcknowledgeConfig( I2C1, DISABLE );
        if(i>=2000)
        {
            return 0;
        }
    }
	return 1;
}

//========================================================================
// 描述: 发送停止信号.
// 参数: none.
// 返回: none.
//========================================================================
void KHardIIC::stop()
{
    I2C_GenerateSTOP( I2C1, ENABLE );
}

//========================================================================
// 描述: SOFTIIC读函数.
// 参数: last:0表示应答,1表示非应答.
// 返回: 读取到的数据.
//========================================================================
uint8_t KHardIIC::read(uint8_t last) {
    uint8_t b = 0;
    if(last == 0) {
        I2C_AcknowledgeConfig( I2C1, ENABLE );
    }else{
        I2C_AcknowledgeConfig( I2C1, DISABLE );
    }
    b = I2C_ReceiveData( I2C1 );
    return b;
}

//========================================================================
// 描述: 写函数.
// 参数: data：数据.
// 返回: 0,没有接收到应答;1,接收到应答.
//========================================================================
bool KHardIIC::write(uint8_t data) {
    uint16_t i=0;
	I2C_SendData( I2C1, data );
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ){    /* TRA, BUSY, MSL, TXE and BTF flags */
        i++;
        delayMicroseconds(50);
        if(i>=2000){
            return 0;
        }
    }
    return 1;
}

KHardIIC hardiic;

#endif
