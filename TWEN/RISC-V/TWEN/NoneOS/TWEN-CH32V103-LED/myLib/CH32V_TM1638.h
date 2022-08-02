#ifndef    _CH32V_TM1638_H_
#define    _CH32V_TM1638_H_ 

#include <CH32V103.h>



class CH32V_TM1638{
public:
    CH32V_TM1638();
    void begin(uint8_t sckpin, uint8_t diopin, uint8_t stbpin); //TM1638初始化
    void  Write(unsigned char  DATA) ;//写函数
    uint8_t Read() ;//读函数
    void  Write_COM(unsigned char cmd);
    unsigned char  Read_key(void);
    void  Write_DATA(unsigned char add,unsigned char DATA); 
    void  Write_DATA(unsigned char add,uint8_t* date,uint8_t len) ;
    void  Write_allLED(unsigned char LED_flag); 




private:
    uint8_t _sckpin;
    uint8_t _diopin;
    uint8_t _stbpin;
};

//========================================================================
// 描述: 构造函数.
// 参数: none
// 返回: none.
//========================================================================
CH32V_TM1638::CH32V_TM1638()
{}

//========================================================================
// 描述: 初始化函数.
// 参数: sckpin:sck引脚; mosipin: mosi引脚; misopin：miso引脚.
// 返回: none.
//========================================================================
void CH32V_TM1638::begin(uint8_t sckpin, uint8_t diopin, uint8_t stbpin)
{
    _sckpin = sckpin;
    pinMode(_sckpin, GPIO_Mode_Out_PP);
    _diopin = diopin;
    pinMode(_diopin, GPIO_Mode_Out_PP);
    _stbpin = stbpin;
    pinMode(_stbpin, GPIO_Mode_Out_PP);
}

//========================================================================
// 描述: 写函数.
// 参数: sckpin:sck引脚; mosipin: mosi引脚; misopin：miso引脚.
// 返回: 读到的数据.
//========================================================================
void  CH32V_TM1638::Write(unsigned char  DATA)                        //写数据函数
{
        unsigned char i;
        pinMode(_diopin,GPIO_Mode_Out_PP);
        for(i=0;i<8;i++)
        {
            digitalWrite(_sckpin,LOW);
            if(DATA&0X01)
               digitalWrite(_diopin,HIGH);
            else
               digitalWrite(_diopin,LOW);
            DATA>>=1;
            digitalWrite(_sckpin,HIGH);
        }
}


//========================================================================
// 描述: 读函数.
// 参数: none.
// 返回: 读到的数据.
//========================================================================
uint8_t  CH32V_TM1638::Read()
{
        unsigned char i;
        unsigned char temp=0;;        
        pinMode(_diopin,GPIO_Mode_IPU);//设置为输入
        for(i=0;i<8;i++)
        {
              temp>>=1;
            digitalWrite(_sckpin,LOW);
              if(digitalRead(_diopin)==HIGH)
                temp|=0x80;
            digitalWrite(_sckpin,HIGH);
               
        }
        return temp;
}

void CH32V_TM1638::Write_COM(unsigned char cmd)                //发送命令字
{
        digitalWrite(_stbpin,LOW);
        Write(cmd);
        digitalWrite(_stbpin,HIGH);
}
unsigned char CH32V_TM1638::Read_key(void)
{
        uint32_t c=0,i,key_value=0;
        uint8_t temp;
        digitalWrite(_stbpin,LOW);
        Write(0x42);                           //读键扫数据 命令
        for(i=0;i<4;i++)                
        {
            temp=Read();
            c=c<<8;
            c|=temp;
           
 
        }
        digitalWrite(_stbpin,HIGH);                                                   //4个字节数据合成一个字节
        for(i=0;i<4;i++)
        {
            key_value|=c<<i;
        }        
        for(i=0;i<8;i++)
        {
            if((0x01<<i)==key_value)
            break;
        }
        return i;
}
void CH32V_TM1638::Write_DATA(unsigned char add,unsigned char DATA)                //指定地址写入数据
{
        Write_COM(0x44);
        digitalWrite(_stbpin,LOW);
        Write(0xc0|add);
        Write(DATA);
        digitalWrite(_stbpin,HIGH);
}
void CH32V_TM1638::Write_DATA(unsigned char add,uint8_t* date,uint8_t len)                //指定地址写入数据
{
        Write_COM(0x40);
        digitalWrite(_stbpin,LOW);
        Write(0xc0|add);
        for(int i=0;i<len;i++)
        {
           Write(date[i]);
           Write(0x00);
        }
        digitalWrite(_stbpin,HIGH);
}
void CH32V_TM1638::Write_allLED(unsigned char LED_flag)                                        //控制全部LED函数，LED_flag表示各个LED状态
{
        unsigned char i;
        for(i=0;i<8;i++)
        {
            if(LED_flag&(1<<i))
              Write_DATA(2*i+1,1);
            else
              Write_DATA(2*i+1,0);
        }
}

#endif //  HDUSB_softspi.h
