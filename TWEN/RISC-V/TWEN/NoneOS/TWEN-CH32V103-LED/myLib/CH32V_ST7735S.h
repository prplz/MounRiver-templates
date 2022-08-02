/**********************************************************
 * 该库为ST7735芯片的SPI彩屏.
 * *******************************************************/
#ifndef _ST7735S_H
#define _ST7735S_H

#include <CH32V103.h>
#include "CH32V_SOFTSPI.h"
#include "CH32V_HARDSPI.h"
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
	uint16_t id;			//LCD ID
	uint8_t  dir;			//彩屏显示方向。	
	uint16_t  wramcmd;	//开始写gram指令
	uint16_t  rramcmd;   	//开始读gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev_s;

#define  TFT_HARD_SPI 	0
#define  TFT_SOFT_SPI 	1

class SPITFT : public TWEN_GFX
{
public:
	//硬件spi
	SPITFT(uint16_t w, uint16_t h, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t blk_pin=-1);
	//模拟spi
	SPITFT(uint16_t w, uint16_t h, int8_t cs_pin, int8_t sck_pin, int8_t mosi_pin,  int8_t dc_pin, 
				int8_t rst_pin, int8_t blk_pin=-1);
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
		void write_8bit(uint8_t dat);	//写入8位数据
	void write_16bit(uint16_t Data);  //写入16位数据
	void write_cmd(uint8_t Reg);	//写命令
private:
	uint8_t _sck_pin;	//引脚
	uint8_t _mosi_pin;
	uint8_t _cs_pin;
	uint8_t _dc_pin;	
	uint8_t _rst_pin;
	uint8_t _blk_pin;

	uint8_t connection;      //保存连接方式，1为模拟SPI，0为硬件SPI

	_lcd_dev_s lcddev;	//管理LCD重要参数


};

//========================================================================
// 描述: 硬件SPI设置输出引脚.
// 参数: none.
// 返回: none.
//========================================================================
SPITFT::SPITFT(uint16_t w, uint16_t h, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t blk_pin):TWEN_GFX(w, h)
{
	connection = TFT_HARD_SPI;
	lcddev.width = w;
	lcddev.height = h;
	_cs_pin = cs_pin;
	_dc_pin = dc_pin;
	_rst_pin = rst_pin;
	if(blk_pin != -1)
	{
		_blk_pin = blk_pin;
	}
}

//========================================================================
// 描述: 模拟SPI设置输出引脚.
// 参数: none.
// 返回: none.
//========================================================================
SPITFT::SPITFT(uint16_t w, uint16_t h, int8_t cs_pin, int8_t sck_pin, int8_t mosi_pin, int8_t dc_pin, 
					int8_t rst_pin, int8_t blk_pin):TWEN_GFX(w, h)
{
	connection = TFT_SOFT_SPI;
	lcddev.width = w;
	lcddev.height = h;
	_cs_pin = cs_pin;
	_sck_pin = sck_pin;
	_mosi_pin = mosi_pin;
	_dc_pin = dc_pin;
	_rst_pin = rst_pin;
	if(blk_pin != -1)
	{
		_blk_pin = blk_pin;
	}
}

//========================================================================
// 描述: LCD开始写入数据.
// 参数: none.
// 返回: none.
//========================================================================
void SPITFT::start_write()
{
	digitalWrite(_cs_pin,0);
}

//========================================================================
// 描述: LCD结束写入数据.
// 参数: none.
// 返回: none.
//========================================================================
void SPITFT::end_write()
{
	digitalWrite(_cs_pin,1);
}

//========================================================================
// 描述: LCD写入8位数据.
// 参数: VAL:写入的字节数据.
// 返回: none.
//========================================================================
void SPITFT::write_8bit(uint8_t dat)
{
    if(connection == TFT_HARD_SPI)	//
	{
		hardspi.wr_data(dat);
	}else if (connection == TFT_SOFT_SPI){
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (dat & 0x80)
				digitalWrite(_mosi_pin,1);
			else
				digitalWrite(_mosi_pin,0);
			digitalWrite(_sck_pin,1);
			dat <<= 1;
			digitalWrite(_sck_pin,0);
		}
	}
}

//========================================================================
// 描述: LCD写入16位数据.
// 参数: Data:写入的数据.
// 返回: none.
//========================================================================
void SPITFT::write_16bit(uint16_t Data)
{
    write_8bit(Data>>8);
	write_8bit(Data);
}

//========================================================================
// 描述: LCD写入命令.
// 参数: Reg:写入的命令.
// 返回: none.
//========================================================================
void SPITFT::write_cmd(uint8_t Reg)	 
{	
	digitalWrite(_dc_pin,0);
	write_8bit(Reg);
    digitalWrite(_dc_pin,1);
}

//========================================================================
// 描述: LCD设置窗口.
// 参数: x0,y0:起始坐标; x1,y1:终点坐标.
// 返回: none.
//========================================================================
void SPITFT::set_windows(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	if(lcddev.dir == 2){
		write_cmd(lcddev.setxcmd);
		write_16bit(x0);
		write_16bit(x1);
		write_cmd(lcddev.setycmd);
		write_16bit(160-lcddev.height+y0);
		write_16bit(160-lcddev.height+y1);
	}else if(lcddev.dir == 3){
		write_cmd(lcddev.setxcmd);
		write_16bit(160-lcddev.height+x0);
		write_16bit(160-lcddev.height+x1);
		write_cmd(lcddev.setycmd);
		write_16bit(y0);
		write_16bit(y1);
	}else{
		write_cmd(lcddev.setxcmd);
		write_16bit(x0);
		write_16bit(x1);
		write_cmd(lcddev.setycmd);
		write_16bit(y0);
		write_16bit(y1);	
	}
	write_cmd(lcddev.wramcmd);						
}

//========================================================================
// 描述: LCD初始化.
// 参数: none.
// 返回: none.
//========================================================================
void SPITFT::init()
{
	if(connection == TFT_SOFT_SPI)
	{
		pinMode(_sck_pin, GPIO_Mode_Out_PP);
		pinMode(_mosi_pin, GPIO_Mode_Out_PP);
	}else if(connection == TFT_HARD_SPI)
	{
		hardspi.begin();
	}
	pinMode(_rst_pin, GPIO_Mode_Out_PP);
	pinMode(_cs_pin, GPIO_Mode_Out_PP);
	pinMode(_dc_pin, GPIO_Mode_Out_PP);

    digitalWrite(_rst_pin,1);
    delay(50);
	digitalWrite(_rst_pin,0);
	delay(100);
	digitalWrite(_rst_pin,1);
	delay(50);

    write_cmd(0x11); //Exit Sleep
    delay(50);	
    write_cmd(0xB1);
    write_8bit(0x02);
    write_8bit(0x35);
    write_8bit(0x36);

    write_cmd(0xB2);
    write_8bit(0x02);
    write_8bit(0x35);
    write_8bit(0x36);

    write_cmd(0xB3);
    write_8bit(0x02);
    write_8bit(0x35);
    write_8bit(0x36);
    write_8bit(0x02);
    write_8bit(0x35);
    write_8bit(0x36);
		
    //------------------------------------End ST7735S Frame Rate-----------------------------------------//
    write_cmd(0xB4); //Dot inversion
    write_8bit(0x03); //03

    write_cmd(0xC0);
    write_8bit(0xa2);
    write_8bit(0x02);
    write_8bit(0x84);

    write_cmd(0xC1);
    write_8bit(0XC5);

    write_cmd(0xC2);
    write_8bit(0x0D);
    write_8bit(0x00);

    write_cmd(0xC3);
    write_8bit(0x8D);
    write_8bit(0x2A);

    write_cmd(0xC4);
    write_8bit(0x8D);
    write_8bit(0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    write_cmd(0xC5); //VCOM
    write_8bit(0x03);
	
	write_cmd(0x3a); //Set Color Format
	write_8bit(0x05);
	
	// write_cmd(0x36); //Set Scanning Direction
	// if(USE_HORIZONTAL==0)write_8bit(0x08);
	// else if(USE_HORIZONTAL==1)write_8bit(0xC8);
	// else if(USE_HORIZONTAL==2)write_8bit(0x78);
	
    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    write_cmd(0xE0);
    write_8bit(0x12);
    write_8bit(0x1C);
    write_8bit(0x10);
    write_8bit(0x18);
    write_8bit(0x33);
    write_8bit(0x2c);
    write_8bit(0x25);
    write_8bit(0x28);
    write_8bit(0x28);
    write_8bit(0x27);
    write_8bit(0x2f);
    write_8bit(0x3C);
    write_8bit(0x00);
    write_8bit(0x03);
    write_8bit(0x03);
    write_8bit(0x10);

    write_cmd(0xE1);
    write_8bit(0x12);
    write_8bit(0x1c);
    write_8bit(0x10);
    write_8bit(0x18);
    write_8bit(0x2d);
    write_8bit(0x28);
    write_8bit(0x23);
    write_8bit(0x28);
    write_8bit(0x28);
    write_8bit(0x26);
    write_8bit(0x2f);
    write_8bit(0x3B);
    write_8bit(0x00);
    write_8bit(0x03);
    write_8bit(0x03);
    write_8bit(0x10);

	write_cmd(0x30);             
	write_8bit(0x00);             
	write_8bit(0x00);             
	write_8bit(0x00);             
	write_8bit(0x80);
	 
	write_cmd(0x12);
	write_cmd(0x29); // Display On
	set_direction(USE_HORIZONTAL);
}

//========================================================================
// 描述: LCD背光控制.
// 参数: 0：关闭背光；其他值：打开背光.
// 返回: none.
//========================================================================
void SPITFT::backlight(uint8_t bright)
{
    pinMode(_blk_pin,GPIO_Mode_Out_PP);
	if(bright == 0)
	{
		digitalWrite(_blk_pin,0);
	}else{
		digitalWrite(_blk_pin,1);
	}

}

//========================================================================
// 描述: 设置显示方向.
// 参数: direction:0~3;
// 返回: none.
//========================================================================
void SPITFT::set_direction(uint8_t direction)
{
	lcddev.setxcmd = 0x2A;
	lcddev.setycmd = 0x2B;
	lcddev.wramcmd = 0x2C;
	lcddev.rramcmd = 0x2E;
	lcddev.dir = direction;
	start_write();
	switch (lcddev.dir) {
		case 2:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 7) | (0 << 4)| (1 << 6));
			break;
		case 3:
			lcddev.width = lcddev.height;
			lcddev.height = lcddev.width;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 7) | (1 << 5) | (0 << 4) | (0 << 6));
			break;
		case 0:
			lcddev.width = lcddev.width;
			lcddev.height = lcddev.height;
			write_cmd(0x36);
			write_8bit((1<<3)|(0<<6));			
			break;
		case 1:
			lcddev.width = lcddev.height;
			lcddev.height = lcddev.width;
			write_cmd(0x36);
			write_8bit((1 << 3) | (1 << 5)|(1<<6));
			break;
		default:
			break;
	}
	end_write();
}


//========================================================================
// 描述: LCD清屏.
// 参数: color:清屏的颜色.
// 返回: none.
//========================================================================
void SPITFT::clear(uint16_t color)
{
	uint16_t i, j;
	start_write();
	set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
	for (i = 0; i < lcddev.width; i++)
	{
		for (j = 0; j < lcddev.height; j++)
		{
		    write_8bit(color>>8);
	        write_8bit(color);
		}
	}
	end_write();
}

//========================================================================
// 描述: LCD画点.
// 参数: x,y: 坐标.
// 返回: none.
//========================================================================
void SPITFT::draw_point(int16_t x, int16_t y, uint16_t color)
{
	start_write();
	set_windows(x, y, x, y);//设置光标位置 
	write_8bit(color>>8);
	write_8bit(color);
	end_write();
}

//========================================================================
// 描述: LCD填充矩形.
// 参数: x0，y0:起始坐标; x1,y1:终止坐标; color:颜色
// 返回: none.
//========================================================================
void SPITFT::fill_rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
	uint16_t i, j;
	uint16_t width = x1 - x0 + 1; 		//得到填充的宽度
	uint16_t height = y1 - y0 + 1;		//高度
	start_write();
	set_windows(x0, y0, x1, y1);//设置显示窗口
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			write_8bit(color>>8);
			write_8bit(color); 
		}
	}
	set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);//恢复窗口设置为全屏
	end_write();
}

//========================================================================
// 描述: LCD显示图片.
// 参数: none.
// 返回: none.
//========================================================================
void SPITFT::show_picture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	start_write();
	set_windows(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			write_8bit(pic[k*2+1]);
			write_8bit(pic[k*2]);
			k++;
		}
	}	
	end_write();		
}

#endif  //asr_st7735s.h