/*

*/

#ifndef Wiring_h
#define Wiring_h




#ifdef __cplusplus
extern "C"{
#endif
	#include "ch32v10x_conf.h"

/*
#define    GPIO_Mode_AIN         0 //模拟输入
#define    GPIO_Mode_Out_PP      1 //通用推免输出
#define    GPIO_Mode_IPU         2 //上拉输入
#define    GPIO_Mode_IPD         3 //下拉输入
#define    GPIO_Mode_IN_FLOATING 4 //浮空输入
#define    GPIO_Mode_Out_OD      5 //通用开漏输出
#define    GPIO_Mode_AF_PP       6 //复用推免输出
#define    GPIO_Mode_AF_OD       7 //复用开漏输出

#define    GPIO_SPEED_FREQ_DEFAULT    0x00
#define    GPIO_SPEED_FREQ_LOW        0x01
#define    GPIO_SPEED_FREQ_MEDIUM     0x02
#define    GPIO_SPEED_FREQ_HIGH       0x03
*/

#define  ADC_BIT_12          0 //12位分辨率
#define  ADC_BIT_11          1 //11位分辨率
#define  ADC_BIT_10          2 //10位分辨率
#define  ADC_BIT_9           3 //9位分辨率
#define  ADC_BIT_8           4 //8位分辨率

#define  ADC_SAMPLE_1_5      ADC_SampleTime_1Cycles5 //1.5采样周期 
#define  ADC_SAMPLE_7_5      ADC_SampleTime_7Cycles5 //7.5采样周期
#define  ADC_SAMPLE_13_5     ADC_SampleTime_13Cycles5 //13.5采样周期
#define  ADC_SAMPLE_28_5     ADC_SampleTime_28ycles5 //28.5采样周期
#define  ADC_SAMPLE_41_5     ADC_SampleTime_41Cycles5 //41.5采样周期
#define  ADC_SAMPLE_55_5     ADC_SampleTime_55Cycles5 //55.5采样周期
#define  ADC_SAMPLE_71_5     ADC_SampleTime_71Cycles5 //71.5采样周期
#define  ADC_SAMPLE_239_5    ADC_SampleTime_1Cycles5 //239.5采样周期

typedef enum
{
	ADC_PA0 = 0,
	ADC_PA1,
	ADC_PA2, 
	ADC_PA3,
	ADC_PA4,
	ADC_PA5,
	ADC_PA6,
	ADC_PA7,
	ADC_PB0,
	ADC_PB1,
	ADC_PC0,
	ADC_PC1,
	ADC_PC2,
	ADC_PC3,
	ADC_PC4,
	ADC_PC5
} ADC_Name;

void CH32V_ADC_Init(ADC_Name adcn, uint32_t sample, uint32_t resolution);
uint16_t ADC_Read(ADC_Name adcn);


void CH32_Init(void);
void CH32V_UART1_Init(uint32_t BAUD);
void CH32V_UART1_SendCh(unsigned char c);
void CH32V_UART2_Init(uint32_t BAUD);
void CH32V_UART2_SendCh(unsigned char c);
void CH32V_UART3_Init(uint32_t BAUD);
void CH32V_UART3_SendCh(unsigned char c);
void CH32V_PFIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_SubPriority,uint8_t NVIC_Channel,uint8_t NVIC_Group);
void CH32V_PFIC_Init_pro(uint8_t NVIC_PreemptionPriority, uint8_t NVIC_Channel);
void CH32V_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t mode, uint8_t speed);
void CH32V_GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void CH32V_GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void CH32V_GPIOX_Init(GPIO_TypeDef *GPIOx,  GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);



#ifdef __cplusplus
} // extern "C"
#endif


#endif
