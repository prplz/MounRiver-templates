/**********************************************************
 * 该库为CH32V103的16位彩屏触摸库(HR2046).
 * *******************************************************/
#ifndef _CH32V_SPITOUCH_H
#define _CH32V_SPITOUCH_H

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define CT_MAX_TOUCH  5    //电容屏支持的点数,固定为5点

#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值

class TOUCHPOINT
{
public:
    TOUCHPOINT(uint8_t tcs_pin, uint8_t pen_pin);
    void begin();
    uint16_t read_xoy(uint8_t xy);
    uint8_t read_xy(uint16_t *x,uint16_t *y);
    uint8_t read_xy2(uint16_t *x,uint16_t *y);
    uint8_t is_pressed();
	
private:
    uint8_t _tcs_pin;
    uint8_t _pen_pin;
	void write_byte(uint8_t num);
	uint16_t read_ad(uint8_t CMD);
	//默认为touchtype=0的数据.
    uint8_t CMD_RDX=0XD0;
    uint8_t CMD_RDY=0X90;
};

TOUCHPOINT::TOUCHPOINT(uint8_t tcs_pin, uint8_t pen_pin)
{
    _tcs_pin = tcs_pin;
    _pen_pin = pen_pin;
}

//========================================================================
// 描述: 初始化.
// 参数: none.
// 返回: none.
//========================================================================
void TOUCHPOINT::begin()
{
    pinMode(_pen_pin, GPIO_Mode_IPU);
    pinMode(_tcs_pin, GPIO_Mode_Out_PP);
    hardspi.begin();
}

//========================================================================
// 描述: 是否有按键按下.
// 参数: none.
// 返回: 0:有按键按下; 1:没有按键按下.
//========================================================================
uint8_t TOUCHPOINT::is_pressed() 
{ 
    if(digitalRead(_pen_pin) == 0){
        return 1;
    }else{
        return 0;
    }
}

//========================================================================
// 描述: 向触摸屏IC写数据.
// 参数: 写入的数据.
// 返回: none.
//========================================================================
void TOUCHPOINT::write_byte(uint8_t num)    
{  
	hardspi.wr_data(num);
} 

//========================================================================
// 描述: 从触摸屏IC读数据.
// 参数: 指令.
// 返回: 读到的数据.
//========================================================================
uint16_t TOUCHPOINT::read_ad(uint8_t CMD)	  
{ 	 
	uint16_t Num=0; 
	hardspi.set_speed(3);
	digitalWrite(_tcs_pin, 0); 		//选中触摸屏IC
	write_byte(CMD);   //发送命令字
	delayMicroseconds(6);   //ADS7846的转换时间最长为6us

    Num = hardspi.wr_data(0) << 8;
	Num = Num | hardspi.wr_data(0);
    Num >>= 3;  //只有高12位有效 
    digitalWrite(_tcs_pin, 1); 		////释放片选 
	hardspi.set_speed(1);
	return(Num);   
}

//========================================================================
// 描述: 读取一个坐标值(x或者y).
// 参数: 指令（CMD_RDX/CMD_RDY）.
// 返回: 读到的数据.
//========================================================================
uint16_t TOUCHPOINT::read_xoy(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=read_ad(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

//========================================================================
// 描述: 读取x,y坐标.
// 参数: x,y:读取到的坐标值.
// 返回: 0,失败;1,成功.
//========================================================================
uint8_t TOUCHPOINT::read_xy(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp = read_xoy(CMD_RDX);
	ytemp = read_xoy(CMD_RDY);	  												   
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}

//========================================================================
// 描述: 连续2次读取触摸屏IC,且这两次的偏差不能超过ERR_RANGE.
// 参数: x,y:读取到的坐标值.
// 返回: 0,失败;1,成功.
//========================================================================
#define ERR_RANGE 50 //误差范围 
uint8_t TOUCHPOINT::read_xy2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag = read_xy(&x1,&y1);   
    if(flag == 0)return(0);
    flag = read_xy(&x2,&y2);	   
    if(flag == 0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  

#endif
















