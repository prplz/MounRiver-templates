/**********************************************************
 * 该库为CH32V103的模拟SPI库.
 * *******************************************************/
#ifndef    _CH32V_SOFTSPI_H_
#define    _CH32V_SOFTSPI_H_ 

#include <CH32V103.h>

#define     SOFTSPI_CPHA 0
#define     SOFTSPI_CPOL 0

class SOFTSPI{
public:
    SOFTSPI();
    void begin(uint8_t sckpin, uint8_t mosipin, uint8_t misopin); //SOFTSPI初始化

    uint8_t wr_data(uint8_t data); //读/写函数
    void write_data(uint8_t data); //写函数
    uint8_t read_data(); //读函数

private:
    uint8_t _sckpin;
    uint8_t _mosipin;
    uint8_t _misopin;
};

//========================================================================
// 描述: 构造函数.
// 参数: none
// 返回: none.
//========================================================================
SOFTSPI::SOFTSPI()
{}

//========================================================================
// 描述: 初始化函数.
// 参数: sckpin:sck引脚; mosipin: mosi引脚; misopin：miso引脚.
// 返回: none.
//========================================================================
void SOFTSPI::begin(uint8_t sckpin, uint8_t mosipin, uint8_t misopin)
{
    _sckpin = sckpin;
    pinMode(_sckpin, GPIO_Mode_Out_PP);
    _mosipin = mosipin;
    pinMode(_mosipin, GPIO_Mode_Out_PP);
    _misopin = misopin;
    pinMode(_misopin, GPIO_Mode_IN_FLOATING);
}

//========================================================================
// 描述: 读写函数.
// 参数: sckpin:sck引脚; mosipin: mosi引脚; misopin：miso引脚.
// 返回: 读到的数据.
//========================================================================
uint8_t SOFTSPI::wr_data(uint8_t data)
{
    uint8_t datavalue=0,i,wr_data;
    wr_data = data;
   #if SOFTSPI_CPHA==0 && SOFTSPI_CPOL==0
    for(i=0;i<8;i++){
        if(wr_data&0x80){
            digitalWrite(_mosipin, 1);
        }else{
            digitalWrite(_mosipin, 0);
        }
        wr_data <<=1;
        digitalWrite(_sckpin, 1);
        datavalue <<=1;
        if(digitalRead(_misopin)){                  
            datavalue |= 0x01;
        }
        digitalWrite(_sckpin, 0);
        //delay(1);
    }
    
    #elif SOFTSPI_CPHA==0 && SOFTSPI_CPOL==1
    for(i=0;i<8;i++){
        if(wr_data&0x80){
            digitalWrite(_mosipin, 1);
        }else{
            digitalWrite(_mosipin, 0);
        }
        wr_data <<=1;
        digitalWrite(_sckpin, 0);
        datavalue <<=1;
        if(digitalRead(_misopin)){
            datavalue |= 0x01;
        }
        digitalWrite(_sckpin, 1);
        //delay(1);
    }  
    
    #elif SOFTSPI_CPHA==1 && SOFTSPI_CPOL==0
    for(i=0;i<8;i++){
        digitalWrite(_sckpin, 1);
        if(wr_data&0x80){
            digitalWrite(_mosipin, 1);
        }else{
            digitalWrite(_mosipin, 0);
        }
        wr_data <<=1;
        digitalWrite(_sckpin, 0);
        datavalue <<=1;
        if(digitalRead(_misopin)){
            datavalue |= 0x01;
        }
        //delay(1);
    } 
    
    #elif SOFTSPI_CPHA==1 && SOFTSPI_CPOL==1
    for(i=0;i<8;i++){
        digitalWrite(_sckpin, 0);
        if(wr_data&0x80){
            digitalWrite(_mosipin, 1);
        }else{
            digitalWrite(_mosipin, 0);
        }
        wr_data <<=1;
        digitalWrite(_sckpin, 1);
        datavalue <<=1;
        if(digitalRead(_misopin)){
            datavalue |= 0x01;
        }
        digitalWrite(_sckpin, 1);
        // delay(1);
    } 
    #endif
    return datavalue;
}

//========================================================================
// 描述: 写函数.
// 参数: data：数据.
// 返回: none.
//========================================================================
void SOFTSPI::write_data(uint8_t data)
{
    wr_data(data);
}

//========================================================================
// 描述: 读函数.
// 参数: none.
// 返回: 读到的数据.
//========================================================================
uint8_t SOFTSPI::read_data()
{
    return wr_data(0xff);
}

#endif //  HDUSB_softspi.h
