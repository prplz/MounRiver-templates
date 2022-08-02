/**********************************************************
 * 该库为CH32V103的RTC库.
 * *******************************************************/
#ifndef _CH32V_RTC_H
#define _CH32V_RTC_H

#include "CH32V103.h"

typedef struct
{
	__IO uint8_t hour;
	__IO uint8_t min;
	__IO uint8_t sec;

	__IO uint16_t w_year;
	__IO uint8_t  w_month;
	__IO uint8_t  w_date;
	__IO uint8_t  week;
}_calendar_obj;

uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
uint8_t const mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

class INTERNAL_RTC{
public:
    INTERNAL_RTC();
    uint8_t begin();

    uint8_t is_leap_year(uint16_t year);
    uint8_t set_time(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,
                        uint8_t min,uint8_t sec);
    uint8_t set_alarm(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,
                        uint8_t min,uint8_t sec);

    uint8_t get_time(void);
    uint8_t get_week(uint16_t year,uint8_t month,uint8_t day);
	_calendar_obj calendar;

private:
    void NVIC_config(void);
};

//========================================================================
// 描述: 构造函数.
// 参数: none
// 返回: none.
//========================================================================
INTERNAL_RTC::INTERNAL_RTC()
{

}

//========================================================================
// 描述: 初始化.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::begin()
{
    u8 temp=0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    /* Is it the first configuration */
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA1A1)   //读后备寄存器
	{
		BKP_DeInit();
		RCC_LSEConfig(RCC_LSE_ON);
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)
		{
			temp++;
			delay(20);
		}
		if(temp>=255)return 1;
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();
        // RTC_ITConfig(RTC_IT_ALR, ENABLE);	//闹钟中断使能
		// RTC_ITConfig(RTC_IT_SEC, ENABLE);	//秒中断使能
		RTC_WaitForLastTask();
		RTC_EnterConfigMode();
		RTC_SetPrescaler(32767);
		RTC_WaitForLastTask();
		set_time(2019,10,8,13,58,55);              /* Setup Time */
		RTC_ExitConfigMode();
		BKP_WriteBackupRegister(BKP_DR1, 0XA1A1);
	}else
	{
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        PWR_WakeUpPinCmd(DISABLE);
		RTC_WaitForSynchro();
        // RTC_ITConfig(RTC_IT_ALR, ENABLE);	//闹钟中断使能
        // RTC_ITConfig(RTC_IT_SEC, ENABLE);	//秒中断使能
        RTC_WaitForLastTask();
	}
	// NVIC_config();
	get_time();
    return 0;
}

//========================================================================
// 描述: 中断配置.
// 参数: none
// 返回: none.
//========================================================================
void INTERNAL_RTC::NVIC_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//========================================================================
// 描述: 判断是不是闰年.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::is_leap_year(uint16_t year)
{
	if(year%4==0)
	{
		if(year%100==0)
		{
			if(year%400==0)return 1;
			else return 0;
		}else return 1;
	}else return 0;
}

//========================================================================
// 描述: 设置时间.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::set_time(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,
                        uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(year < 1970||year > 2099)return 1;
	for(t=1970;t<year;t++)
	{
		if(is_leap_year(t))seccount+=31622400;
		else seccount+=31536000;
	}
	mon-=1;
	for(t=0;t<mon;t++)
	{
		seccount+=(uint32_t)mon_table[t]*86400;
		if(is_leap_year(year)&&t==1)seccount+=86400;
	}
	seccount+=(uint32_t)(day-1)*86400;
	seccount+=(uint32_t)hour*3600;
    seccount+=(uint32_t)min*60;
	seccount+=sec;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetCounter(seccount);
	RTC_WaitForLastTask();
	return 0;
}

//========================================================================
// 描述: 获取时间.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::get_time(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0;
	uint32_t temp=0;
	uint16_t temp1=0;
    timecount = RTC_GetCounter();
 	temp=timecount/86400;
	if(daycnt!=temp)
	{
		daycnt=temp;
		temp1=1970;
		while(temp>=365)
		{
			if(is_leap_year(temp1))
			{
				if(temp>=366)temp-=366;
				else {temp1++;break;}
			}
			else temp-=365;
			temp1++;
		}
		calendar.w_year=temp1;
		temp1=0;
		while(temp>=28)
		{
			if(is_leap_year(calendar.w_year)&&temp1==1)
			{
				if(temp>=29)temp-=29;
				else break;
			}
			else
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];
				else break;
			}
			temp1++;
		}
		calendar.w_month=temp1+1;
		calendar.w_date=temp+1;
	}
	temp=timecount%86400;
	calendar.hour=temp/3600;
	calendar.min=(temp%3600)/60;
	calendar.sec=(temp%3600)%60;
	calendar.week = get_week(calendar.w_year,calendar.w_month,calendar.w_date);
	return 0;
}

//========================================================================
// 描述: 获取星期.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::get_week(uint16_t year,uint8_t month,uint8_t day)
{
	uint16_t temp2;
	uint8_t yearH,yearL;

	yearH=year/100;	yearL=year%100;
	if (yearH>19)yearL+=100;
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}

//========================================================================
// 描述: 设置闹钟时间.
// 参数: none
// 返回: none.
//========================================================================
uint8_t INTERNAL_RTC::set_alarm(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,
                            uint8_t min,uint8_t sec)
{
	u16 t;
	u32 seccount=0;
	if(year < 1970||year > 2099)return 1;
	for(t=1970;t<year;t++)
	{
		if(is_leap_year(t))seccount+=31622400;
		else seccount+=31536000;
	}
	mon-=1;
	for(t=0;t<mon;t++)
	{
		seccount+=(uint32_t)mon_table[t]*86400;
		if(is_leap_year(year)&&t==1)seccount+=86400;
	}
	seccount+=(u32)(day-1)*86400;
	seccount+=(u32)hour*3600;
    seccount+=(u32)min*60;
	seccount+=sec;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetAlarm(seccount);
	RTC_WaitForLastTask();

	RTC_ITConfig(RTC_IT_ALR, ENABLE);	//闹钟中断使能
	NVIC_config();

	return 0;
}

//========================================================================
// 描述: RTC中断函数.
// 参数: none
// 返回: none.
//========================================================================
extern "C"{
#if defined (ALARM_ENABLE)
	extern void alarm_callback(void);
#endif
	void RTC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
	void RTC_IRQHandler(void)
	{
		if (RTC_GetITStatus(RTC_IT_SEC) != RESET)   //秒中断
		{

		}
		if(RTC_GetITStatus(RTC_IT_ALR)!= RESET) //闹钟中断
		{
#if defined (ALARM_ENABLE)
			alarm_callback();
#endif
			RTC_ClearITPendingBit(RTC_IT_ALR);
		}

		RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);	//清除中断
		RTC_WaitForLastTask();
	}
}

#endif
