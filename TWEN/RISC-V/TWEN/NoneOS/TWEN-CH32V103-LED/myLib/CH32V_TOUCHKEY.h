/**********************************************************
 * 该库为CH32V103的触摸库.
 * *******************************************************/
#ifndef _CH32V_TOUCHKEY_H
#define _CH32V_TOUCHKEY_H

#include "CH32V103.h"

class TOUCHKEY{
public:
    TOUCHKEY(ADC_Name adcn);
    void begin();
    uint16_t value();

private:
    ADC_Name _adcn;
};

//========================================================================
// 描述: 构造函数.
// 参数: none
// 返回: none.
//========================================================================
TOUCHKEY::TOUCHKEY(ADC_Name adcn)
{
    _adcn = adcn;
}

//========================================================================
// 描述: 初始化.
// 参数: none
// 返回: none.
//========================================================================
void TOUCHKEY::begin()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    if (_adcn < 8)
    {
        pinMode(_adcn, GPIO_Mode_AIN);
    }
    else if (_adcn < 10)
    {
        pinMode(_adcn + 8, GPIO_Mode_AIN);
    }
    else if (_adcn < 15)
    {
        pinMode(_adcn + 14, GPIO_Mode_AIN);
    }

    ADC_Cmd(ADC1, ENABLE);
    
	ADC1->CTLR1 |= 0x51000000;   //开启TKEY_V单元
}

//========================================================================
// 描述: 获取触摸值.
// 参数: ADC_Channel_0 ~ ADC_Channel_15
// 返回: none.
//========================================================================
uint16_t TOUCHKEY::value()
{
    uint16_t val;
    ADC1->RSQR3 = _adcn;
    while(!(ADC1->CTLR1 & 0x08000000)); //等待转换完成
    val = (uint16_t)ADC1->RDATAR;
    return val;
}

#endif





