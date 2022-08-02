#ifndef _CH32V_NTC_H_
#define _CH32V_NTC_H_


#include <CH32V103.h>

class CH32V_NTC
{
public:
	CH32V_NTC(ADC_Name pin,float vref);
    void init();
	float read();
private:
    uint8_t ntc_find_r(uint16_t data);
    float ntc_get_value(uint16_t data);
	ADC_Name _ntc_pin;
    float _vref;
};


CH32V_NTC::CH32V_NTC(ADC_Name pin,float vref)
{
    _ntc_pin = pin;
    _vref = vref;
}

//_ch32v_ntc 型号 QN0603X103J3380HB
//数据放大1000倍方便整数计算
const uint16_t _ch32v_ntc[51]=
{
    42889,//-10
    40967,
    39142,
    37408,
    35761,
    34196,
    32707,
    31291,
    29945,
    28664,
    27445,//0
    26283,
    25177,
    24124,
    23121,
    22165,
    21253,
    20384,
    19555,
    18764,
    18010,
    17290,
    16602,
    15946,
    15319,
    14720,
    14148,
    13601,
    13078,
    12578,
    12099,
    11642,
    11204,
    10785,
    10384,
    10000,
    9632, 
    9280, 
    8943, 
    8619, 
    8309, 
    8012, 
    7727, 
    7453, 
    7191, 
    6939, 
    6698, 
    6466, 
    6243, 
    6029, 
    5824 //40
};
//========================================================================
// 描述: ntc初始化.
// 参数: none.
// 返回: none.
//========================================================================
void CH32V_NTC::init()
{
    CH32V_ADC_Init(_ntc_pin, ADC_SAMPLE_55_5, ADC_BIT_12);//ADC初始化
}

//========================================================================
// 描述: ntc从电阻分度表格里查找最靠近的位置.
// 参数: 电阻值*1000.
// 返回: 分度表格里最靠近的位置.
//========================================================================
uint8_t CH32V_NTC::ntc_find_r(uint16_t data)
{
    uint8_t  start_index, mid = 0, end_index;
    uint16_t  dat;
    
    start_index = 0;
    end_index = 50;
      
    // 2. 开始二分法查找
    while (start_index < end_index)
    {
        mid = (end_index + start_index) >> 1;
        dat = _ch32v_ntc[mid];
        
        if (data < dat)
        {
            start_index = mid + 1;
        }
        else if (data > dat)
        {
            if (mid)
            {
                end_index = mid;
            }
            else
            {
                break;
            }
        }
        else
        {
            return (mid);
        }
    }
    
    return (mid);
}

//========================================================================
// 描述: 根据查表结果计算温度.
// 参数: 电阻值*1000.
// 返回: 温度.
//========================================================================
float CH32V_NTC::ntc_get_value(uint16_t data)
{
  uint8_t index;
  float temp;
  data = (data > 42889) ? 42889 : data;
  data = (data < 5824) ? 5824 : data;
  
  index = ntc_find_r(data);

  if(_ch32v_ntc[index]>data)
  {
    temp = index + (float)(_ch32v_ntc[index]-data)/(_ch32v_ntc[index]-_ch32v_ntc[index+1]);//线性插值
    
    return temp-9;//偏移
  }
  else if(_ch32v_ntc[index]<data)
  {
    temp =  index-(float)(data-_ch32v_ntc[index])/(_ch32v_ntc[index-1]-_ch32v_ntc[index]);//线性插值
    return temp-11;//偏移
  }
  else
  {
    return index-10;
  }
}

//========================================================================
// 描述: 获取ntc温度.
// 参数: None.
// 返回: 温度.
//========================================================================
float CH32V_NTC::read()
{
    uint16_t adc,sum=0;
    uint8_t i;
    float V,R,T;
    for(i=0;i<10;i++)
    {
        sum+=ADC_Read(_ntc_pin);
    }
    adc = sum/10;//采样十次取平均值
    V=adc*_vref/4096;
    R=V*10/(_vref-V);
    T = ntc_get_value(R*1000);
    T = (int)(10.0 * T + 0.5) / 10.0;//保留一位小数
    return T*1.0;
}

#endif


