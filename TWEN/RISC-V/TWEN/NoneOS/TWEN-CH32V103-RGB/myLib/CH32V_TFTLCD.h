/**********************************************************
 * 该库为CH32V103的8位或者16位ili9341驱动的tft彩屏库.
 * *******************************************************/
#ifndef _CH32V_TFTLCD_H
#define _CH32V_TFTLCD_H

#include "CH32V103.h"
#include "twen_gfx.h"

//颜色
#define TFT_LCD_WHITE         	 0xFFFF
#define TFT_LCD_BLACK         	 0x0000	  
#define TFT_LCD_BLUE             0x001F  
#define TFT_LCD_BRED             0XF81F
#define TFT_LCD_GBLUE            0X07FF
#define TFT_LCD_RED           	 0xF800
#define TFT_LCD_MAGENTA       	 0xF81F
#define TFT_LCD_GREEN         	 0x07E0
#define TFT_LCD_CYAN          	 0x7FFF
#define TFT_LCD_YELLOW        	 0xFFE0
#define TFT_LCD_BROWN            0XBC40 //棕色
#define TFT_LCD_BRRED            0XFC07 //棕红色
#define TFT_LCD_GRAY             0X8430 //灰色
#define TFT_LCD_DARKBLUE      	 0X01CF	//深蓝色
#define TFT_LCD_LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define TFT_LCD_GRAYBLUE       	 0X5458 //灰蓝色
#define TFT_LCD_LIGHTGREEN     	 0X841F //浅绿色
#define TFT_LCD_LGRAY            0XC618 //浅灰色(PANNEL),窗体背景色
#define TFT_LCD_LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define TFT_LCD_LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define USE_HORIZONTAL      0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

//LCD重要参数集
typedef struct
{
	uint16_t width;		//LCD 宽度
	uint16_t height;	//LCD 高度
	uint16_t id;	//LCD ID
	uint8_t  dir;	//彩屏显示方向。	
	uint16_t  wramcmd;	//开始写gram指令
	uint16_t  rramcmd;   	//开始读gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev;

class TFTLCD : public TWEN_GFX
{
public:
	TFTLCD(uint16_t w, uint16_t h, uint8_t cs_pin, uint8_t wr_pin, uint8_t rs_pin,  uint8_t rd_pin, 
				uint8_t rst_pin, uint8_t data_width);
	void init();
	void start_write();	//开始写入
	void end_write();	//结束写入
	void backlight(uint8_t bright);	//控制背光
	void set_direction(uint8_t direction);	//设置显示方向
	void set_windows(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); //设置显示窗口
	void clear(uint16_t color);	//清屏
	void draw_point(int16_t x, int16_t y, uint16_t color); //画点
	void show_picture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);	//显示图片
	void fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color);//填充矩形


	uint16_t read_point(uint16_t x, uint16_t y);
	_lcd_dev lcddev;	//管理LCD重要参数
private:
	uint8_t _cs_pin;
	uint8_t _wr_pin;
    uint8_t _rs_pin;
    uint8_t _rd_pin;
	uint8_t _rst_pin;
	uint8_t _data_width;

    void write_8bit(uint8_t dat);	//写入8位数据
	void write_16bit(uint16_t Data);  //写入16位数据
	void write_cmd(uint8_t Reg);	//写命令
	uint16_t read_data();
	uint16_t read_reg(uint16_t reg);
};

//========================================================================
// 描述: 设置输出引脚.
// 参数: none.
// 返回: none.
//========================================================================
TFTLCD::TFTLCD(uint16_t w, uint16_t h, uint8_t cs_pin, uint8_t wr_pin, uint8_t rs_pin,  uint8_t rd_pin, 
				uint8_t rst_pin, uint8_t data_width):TWEN_GFX(w, h)
{
    _data_width = data_width;
    lcddev.width = w;
	lcddev.height = h;
	_cs_pin = cs_pin;
	_wr_pin = wr_pin;
    _rs_pin = rs_pin;
    _rd_pin = rd_pin;
	_rst_pin = rst_pin;
}

//========================================================================
// 描述: LCD开始写入数据.
// 参数: none.
// 返回: none.
//========================================================================
void TFTLCD::start_write()
{
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_cs_pin/16)))->BCR = 1<<(_cs_pin%16);	// digitalWrite(_cs_pin,0);
}

//========================================================================
// 描述: LCD结束写入数据.
// 参数: none.
// 返回: none.
//========================================================================
void TFTLCD::end_write()
{
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_cs_pin/16)))->BSHR = 1<<(_cs_pin%16);	//digitalWrite(_cs_pin,1);
}

//========================================================================
// 描述: LCD写入8位数据.
// 参数: VAL:写入的字节数据.
// 返回: none.
//========================================================================
void TFTLCD::write_8bit(uint8_t dat)
{
    uint16_t temp;
    if(_data_width == 16){
        GPIOB->OUTDR = ((uint16_t)dat);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,0);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,1);
    }else{
        temp = GPIOB->OUTDR & 0x00ff;
        temp |= ((uint16_t)dat<<8);
        GPIOB->OUTDR = temp;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,0);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);	 // digitalWrite(_wr_pin,1);
    }
}

//========================================================================
// 描述: LCD写入16位数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void TFTLCD::write_16bit(uint16_t dat)
{
	uint16_t temp;
    if(_data_width == 16){
        GPIOB->OUTDR = dat;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);	 // digitalWrite(_wr_pin,0);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,1);
    }else{
        temp = GPIOB->OUTDR & 0x00ff;
        temp |= dat&0xff00;
        GPIOB->OUTDR = temp;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,0);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,1);
        
        temp = GPIOB->OUTDR & 0x00ff;
        temp |= (dat&0x00ff)<<8;
        GPIOB->OUTDR = temp;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,0);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);	// digitalWrite(_wr_pin,1);
    }
}

//========================================================================
// 描述: LCD写入命令.
// 参数: Reg:写入的命令.
// 返回: none.
//========================================================================
void TFTLCD::write_cmd(uint8_t Reg)	 
{	
	uint16_t temp;
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rs_pin/16)))->BCR = 1<<(_rs_pin%16);	// digitalWrite(_rs_pin,0);
																					//write_8bit(Reg);
	if(_data_width == 16){
		GPIOB->OUTDR = ((uint16_t)Reg);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);
	}else{
        temp = GPIOB->OUTDR & 0x00ff;
        temp |= ((uint16_t)Reg<<8);
        GPIOB->OUTDR = temp;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BCR = 1<<(_wr_pin%16);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_wr_pin/16)))->BSHR = 1<<(_wr_pin%16);
	}
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rs_pin/16)))->BSHR = 1<<(_rs_pin%16);	// digitalWrite(_rs_pin,1);
}

//========================================================================
// 描述: LCD读数据.
// 参数: none.
// 返回: 读到的值.
//========================================================================
uint16_t TFTLCD::read_data()
{
	uint16_t t;
	if(_data_width == 16){
		for(int8_t i=PB0;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_IPU);	//上拉模式
		} 
		GPIOB->OUTDR = 0x0000;	//端口清零
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		t = GPIOB-> INDR;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);
		for(int8_t i=PB0;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_Out_PP);	//推挽输出
		} 
	}else{
		for(int8_t i=PB8;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_IPU);	//上拉模式
		} 
		GPIOB->OUTDR &= ~0xff00;	//端口清零
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delayMicroseconds(1);
		t = (uint16_t)(GPIOB-> INDR)>>8;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);
		for(int8_t i=PB8;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_Out_PP);	//推挽输出
		} 
	}
	return t;
}

//========================================================================
// 描述: LCD读寄存器.
// 参数: reg:寄存器编号.
// 返回: 读到的值.
//========================================================================
uint16_t TFTLCD::read_reg(uint16_t reg)
{
	write_cmd(reg);
	return read_data();
}

//========================================================================
// 描述: 读取个某点的颜色值.
// 参数: x,y:坐标.
// 返回: 此点的颜色.
//========================================================================
uint16_t TFTLCD::read_point(uint16_t x, uint16_t y)
{
	uint16_t r,g,b;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回	
	start_write();
	set_windows(x, y, x, y);
	write_cmd(0x2E);
	if(_data_width == 16){
		for(int8_t i=PB0;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_IPU);	//上拉模式
		} 
		GPIOB->OUTDR = 0xffff;	//全部输出高
		//dummy data
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delay(2);
		r = GPIOB-> INDR;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		//
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delay(2);
		r = GPIOB-> INDR;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		//
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delay(2);
		b = GPIOB-> INDR;
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		for(int8_t i=PB0;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_Out_PP);	//推挽输出
		} 
		GPIOB->OUTDR = 0xffff;	//全部输出高
		end_write();
		g = r&0xff;
		g<<=8;
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
	}else{
		for(int8_t i=PB8;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_IPU);	//上拉模式
		} 
		GPIOB->OUTDR |= 0xff00;	//全部输出高
		//dummy data
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delayMicroseconds(5);
		r = (GPIOB->INDR & 0xff00);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		//
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delayMicroseconds(5);
		r = (GPIOB->INDR & 0xff00);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		//
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delayMicroseconds(5);
		g = (GPIOB->INDR & 0xff00);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		//
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BCR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,0);
		delayMicroseconds(5);
		b = (GPIOB->INDR & 0xff00);
		((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_rd_pin/16)))->BSHR = 1<<(_rd_pin%16);	// digitalWrite(_rd_pin,1);

		for(int8_t i=PB8;i<=PB15;i++)
		{
			pinMode(i, GPIO_Mode_Out_PP);	//推挽输出
		} 
		GPIOB->OUTDR |= 0xff00;	//全部输出高
		end_write();
		r >>= 8;
		g >>= 8;
		b >>= 8;
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
	}
}

//========================================================================
// 描述: LCD设置窗口.
// 参数: x0,y0:起始坐标; x1,y1:终点坐标.
// 返回: none.
//========================================================================
void TFTLCD::set_windows(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	write_cmd(0X2A);	// write_cmd(lcddev.setxcmd);
	write_8bit(x0>>8);
    write_8bit(x0&0xff);
	write_8bit(x1>>8);
    write_8bit(x1&0xff);
	write_cmd(0X2B);	//write_cmd(lcddev.setycmd);
	write_8bit(y0>>8);
    write_8bit(y0&0xff);
	write_8bit(y1>>8);	
    write_8bit(y1&0xff);
	write_cmd(0X2C);	// write_cmd(lcddev.wramcmd);
}

//========================================================================
// 描述: LCD初始化.
// 参数: none.
// 返回: none.
//========================================================================
void TFTLCD::init()
{
    uint8_t i;
    if(_data_width == 16){
        for(i=PB0;i<=PB15;i++)
        {
            pinMode(i, GPIO_Mode_Out_PP);
        }
    }else{
        for(i=PB8;i<=PB15;i++)
        {
            pinMode(i, GPIO_Mode_Out_PP);
        }
    }
	pinMode(_cs_pin, GPIO_Mode_Out_PP);
	pinMode(_wr_pin, GPIO_Mode_Out_PP);
    pinMode(_rs_pin, GPIO_Mode_Out_PP);
    pinMode(_rd_pin, GPIO_Mode_Out_PP);
    pinMode(_rst_pin, GPIO_Mode_Out_PP);

    digitalWrite(_rd_pin,1);

    digitalWrite(_rst_pin,1);
    delay(50);
	digitalWrite(_rst_pin,0);
	delay(100);
	digitalWrite(_rst_pin,1);
	delay(50);

	start_write();
	write_cmd(0xD3);
	read_data();	//dummy read 	
	read_data();	//读到0x00
  	lcddev.id=read_data();   	//读取93								   
 	lcddev.id<<=8;
	lcddev.id|=read_data();  	//读取41 

	write_cmd(0x01);		//softreset
	delay(50); 
	write_cmd(0x28);		//displayoff

	write_cmd(0xF6); 		//Interface Control needs EXTC=1 MV_EOR=0, TM=0, RIM=0
	write_8bit(0x01); 
	write_8bit(0x01); 
	write_8bit(0x00); 

	write_cmd(0xCF);  	 	//Power Control B [00 81 30]
	write_8bit(0x00); 
	write_8bit(0x81); 
	write_8bit(0X30);

	write_cmd(0xED);  		//Power On Seq [55 01 23 01]
	write_8bit(0x64); 
	write_8bit(0x03); 
	write_8bit(0X12); 
	write_8bit(0X81); 

	write_cmd(0xE8);  		//Driver Timing A [04 11 7A]
	write_8bit(0x85); 
	write_8bit(0x10); 
	write_8bit(0x78);

	write_cmd(0xCB);  		//Power Control A [39 2C 00 34 02]
	write_8bit(0x39); 
	write_8bit(0x2C); 
	write_8bit(0x00); 
	write_8bit(0x34); 
	write_8bit(0x02); 

	write_cmd(0xF7);  		//Pump Ratio [10]
	write_8bit(0x20);

	write_cmd(0xEA);  		//Driver Timing B [66 00]
	write_8bit(0x00); 
	write_8bit(0x00); 

	write_cmd(0xB0);    	//RGB Signal [00]
	write_8bit(0x00); 

	write_cmd(0xB4);    	 //Inversion Control [02] .kbv NLA=1, NLB=1, NLC=1
	write_8bit(0x00); 

	write_cmd(0xC0);    //Power Control 1 [26]
	write_8bit(0x21);   

	write_cmd(0xC1);    //Power Control 2 [00]
	write_8bit(0x11);    

	write_cmd(0xC5);    //VCM control 
	write_8bit(0x3F); 	 //3F
	write_8bit(0x3C); 	 //3C

	write_cmd(0xC7);    //VCM control2 
	write_8bit(0XB5); 

	write_cmd(0x36);    // Memory Access Control 
	write_8bit(0x48); 

	write_cmd(0x3A);    //Pixel read=565, write=565.
	write_8bit(0x55); 	

	write_cmd(0xB1);  	 //Frame Control [00 1B]
	write_8bit(0x00);   
	write_8bit(0x1B); 

	write_cmd(0xB6);    // Display Function Control 
	write_8bit(0x0A); 
	write_8bit(0xA2); 

	write_cmd(0xF2);    //Enable 3G [02]
	write_8bit(0x00); 

	write_cmd(0x26);    //Gamma Set [01]
	write_8bit(0x01); 

	write_cmd(0xE0);    //Set Gamma 
	write_8bit(0x0F); 
	write_8bit(0x26); 
	write_8bit(0x24); 
	write_8bit(0x0B); 
	write_8bit(0x0E); 
	write_8bit(0x09); 
	write_8bit(0x54); 
	write_8bit(0XA8); 
	write_8bit(0x46); 
	write_8bit(0x0C); 
	write_8bit(0x17); 
	write_8bit(0x09); 
	write_8bit(0x0F); 
	write_8bit(0x07); 
	write_8bit(0x00); 	

	write_cmd(0XE1);    //Set Gamma 
	write_8bit(0x00); 
	write_8bit(0x19); 
	write_8bit(0x1B); 
	write_8bit(0x04); 
	write_8bit(0x10); 
	write_8bit(0x07); 
	write_8bit(0x2A); 
	write_8bit(0x47); 
	write_8bit(0x39); 
	write_8bit(0x03); 
	write_8bit(0x06); 
	write_8bit(0x06); 
	write_8bit(0x30); 
	write_8bit(0x38); 
	write_8bit(0x0F); 

	write_cmd(0x2B); 
	write_8bit(0x00);
	write_8bit(0x00);
	write_8bit(0x01);
	write_8bit(0x3f);

	write_cmd(0x2A); 
	write_8bit(0x00);
	write_8bit(0x00);
	write_8bit(0x00);
	write_8bit(0xef);	

	write_cmd(0xB7); 
	write_8bit(0x07);

	write_cmd(0x11); //Exit Sleep
	delay(150);
	write_cmd(0x29); //display on
	set_direction(0);
	end_write();
}

//========================================================================
// 描述: LCD背光控制.
// 参数: 0：关闭背光；其他值：打开背光.
// 返回: none.
//========================================================================
void TFTLCD::backlight(uint8_t bright)
{

}

//========================================================================
// 描述: 设置显示方向.
// 参数: direction:0~3;
// 返回: none.
//========================================================================
void TFTLCD::set_direction(uint8_t direction)
{
	lcddev.setxcmd = 0x2A;
	lcddev.setycmd = 0x2B;
	lcddev.wramcmd = 0x2C;
	lcddev.rramcmd = 0x2E;
	lcddev.dir = direction;
	switch (lcddev.dir) {
		case 0:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1<<3)|(0<<6));
			break;
		case 1:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 7) | (1 << 5) | (1 << 4) | (0 << 6));
			break;
		case 2:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 7) | (1 << 4)| (1 << 6));
			break;
		case 3:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 5)|(1<<6));
			break;
		default:
			break;
	}
}


//========================================================================
// 描述: LCD清屏.
// 参数: color:清屏的颜色.
// 返回: none.
//========================================================================
void TFTLCD::clear(uint16_t color)
{
	uint16_t i, j;
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_cs_pin/16)))->BCR = 1<<(_cs_pin%16);	// start_write();
	set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
	for (i = 0; i < lcddev.width; i++)
	{
		for (j = 0; j < lcddev.height; j++)
		{
			write_16bit(color);
		}
	}
	((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * (_cs_pin/16)))->BSHR = 1<<(_cs_pin%16);	// end_write();
}

//========================================================================
// 描述: LCD画点.
// 参数: x,y: 坐标.
// 返回: none.
//========================================================================
void TFTLCD::draw_point(int16_t x, int16_t y, uint16_t color)
{
	start_write();
	set_windows(x, y, x, y);//设置光标位置 
	write_16bit(color);
	end_write();
}

//========================================================================
// 描述: LCD填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色
// 返回: none.
//========================================================================
void TFTLCD::fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
	uint16_t i, j;
	uint16_t width = x1 - x0 + 1; 		//得到填充的宽度
	uint16_t height = y1 - y0 + 1;		//高度
	start_write();
	set_windows(x0, y0, x1, y1);//设置显示窗口
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
			write_16bit(color);
	}
	set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口设置为全屏
	end_write();
}

//========================================================================
// 描述: LCD显示图片.
// 参数: none.
// 返回: none.
//========================================================================
void TFTLCD::show_picture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	start_write();
	set_windows(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
            write_16bit(((uint16_t)pic[k*2+1]<<8)|pic[k*2]);
			k++;
		}
	}	
	end_write();		
}

#endif