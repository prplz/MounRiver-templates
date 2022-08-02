/*****************************************************
 * 该库为CH32V的spi驱动的SD卡库
******************************************************/
#ifndef _CH32V_SDCARD_H
#define _CH32V_SDCARD_H

#include <CH32V103.h>
#include "CH32V_HARDSPI.h"

// SD卡类型定义
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

// SD卡指令表
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00

//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF


class SDCARD
{
public:
    SDCARD(uint8_t ss_pin);
    uint8_t begin(void);    //初始化
    uint8_t get_CID(uint8_t *cid_data);
    uint8_t get_CSD(uint8_t *csd_data);
    uint32_t get_sector_count(void);    //获取SD卡的总扇区数
    uint8_t read_disk(uint8_t *buf,uint32_t sector,uint8_t cnt);    //SD卡读数据
    uint8_t write_disk(uint8_t *buf,uint32_t sector,uint8_t cnt);   //SD卡写数据
    uint8_t wait_ready(void);
    void dis_select(void);  //取消片选
    uint8_t select(void);   //片选
private:
    uint8_t  SD_Type=0;//SD卡的类型
    uint8_t  _ss_pin;
    uint8_t wr_byte(uint8_t data);
    uint8_t detect(void);
    uint8_t get_response(uint8_t Response);
    uint8_t recv_data(uint8_t *buf,uint16_t len);
    uint8_t send_block(uint8_t *buf,uint8_t cmd);
    uint8_t send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc);
};

SDCARD::SDCARD(uint8_t ss_pin)
{
    _ss_pin = ss_pin;
}

//========================================================================
// 描述: SPI读写数据.
// 参数: 要写入的数据.
// 返回: 读到的数据.
//========================================================================
uint8_t SDCARD::wr_byte(uint8_t data)
{
    SPI_I2S_SendData(SPI1, data);
    return SPI_I2S_ReceiveData(SPI1);

    //return hardspi.wr_data(data);
}

//========================================================================
// 描述: 检测SD卡.
// 参数: none.
// 返回: 0：检测到SD卡；1：没有检测到sd卡.
//========================================================================
uint8_t SDCARD::detect(void)
{
    return 0;
}

//========================================================================
// 描述: 等待卡准备好.
// 参数: none.
// 返回: 0,准备好了;其他,错误代码.
//========================================================================
uint8_t SDCARD::wait_ready(void)
{
    uint32_t t=0;
    do
    {
        if(wr_byte(0XFF)==0XFF)return 0;//OK
        t++;
    }while(t<0XFFFFFF);//等待
    return 1;
}

//========================================================================
// 描述: 取消选择，释放SPI总线.
// 参数: none.
// 返回: none.
//========================================================================
void SDCARD::dis_select(void)
{
    digitalWrite(_ss_pin, 1); 
    wr_byte(0xff);//提供额外的8个时钟
}

//========================================================================
// 描述: 选择sd卡,并且等待卡准备OK.
// 参数: none.
// 返回: 0,成功;1,失败.
//========================================================================
uint8_t SDCARD::select(void)
{
    digitalWrite(_ss_pin, 0); 
    if(wait_ready()==0)return 0;//等待成功
    dis_select();
    return 1;//等待失败
}

//========================================================================
// 描述: 等待SD卡回应.
// 参数: 要得到的回应值.
// 返回: 0,成功得到了该回应值;其他,得到回应值失败.
//========================================================================
uint8_t SDCARD::get_response(uint8_t Response)
{
    uint16_t Count=0xFFFF;//等待次数
    while ((wr_byte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应
    if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败
    else return MSD_RESPONSE_NO_ERROR;//正确回应
}

//========================================================================
// 描述: 从sd卡读取一个数据包的内容.
// 参数: buf:数据缓存区;
//       len:要读取的数据长度.   
// 返回: 0,成功;其他,失败.
//========================================================================
uint8_t SDCARD::recv_data(uint8_t *buf,uint16_t len)
{
    if(get_response(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf = wr_byte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    wr_byte(0xFF);
    wr_byte(0xFF);
    return 0;//读取成功
}

//========================================================================
// 描述: 向sd卡写入一个数据包的内容 512字节.
// 参数: buf:数据缓存区;
//       cmd:指令.   
// 返回: 0,成功;其他,失败.
//========================================================================
uint8_t SDCARD::send_block(uint8_t *buf,uint8_t cmd)
{
    uint16_t t;
    if(wait_ready())return 1;//等待准备失效
    wr_byte(cmd);
    if(cmd!=0XFD)//不是结束指令
    {
        //for(t=0;t<512;t++)hardspi.wr_data(buf[t]);//提高速度,减少函数传参时间
        for(t=0;t<512;t++)wr_byte(buf[t]);//提高速度,减少函数传参时间
        wr_byte(0xFF);//忽略crc
        wr_byte(0xFF);
        t = wr_byte(0xFF);//接收响应
        if((t&0x1F)!=0x05)return 2;//响应错误
    }
    return 0;//写入成功
}

//========================================================================
// 描述: 向SD卡发送一个命令.
// 参数: cmd:命令
//       arg:命令参数  
//       crc:校验值
// 返回: SD卡返回的响应.
//========================================================================
uint8_t SDCARD::send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
    uint8_t Retry=0;
    dis_select();//取消上次片选
    if(select())return 0XFF;//片选失效
    //发送
    wr_byte(cmd | 0x40);//分别写入命令
    wr_byte(arg >> 24);
    wr_byte(arg >> 16);
    wr_byte(arg >> 8);
    wr_byte(arg);
    wr_byte(crc);
    if(cmd==CMD12)wr_byte(0xff);//Skip a stuff byte when stop reading
    //等待响应，或超时退出
    Retry=0X1F;
    do
    {
        r1=wr_byte(0xFF);
    }while((r1&0X80) && Retry--);
    //返回状态值
    return r1;
}

//========================================================================
// 描述: 获取SD卡的CID信息，包括制造商信息.
// 参数: cid_data:存放CID的内存，至少16Byte
// 返回: SD卡返回的响应.
//========================================================================
uint8_t SDCARD::get_CID(uint8_t *cid_data)
{
    uint8_t r1;
    //发CMD10命令，读CID
    r1 = send_cmd(CMD10,0,0x01);
    if(r1 == 0x00)
    {
        r1 = recv_data(cid_data,16);//接收16个字节的数据
    }
    dis_select();//取消片选
    if(r1)return 1;
    else return 0;
}

//========================================================================
// 描述: 获取SD卡的CSD信息，包括容量和速度信息.
// 参数: csd_data:存放CSD的内存，至少16Byte
// 返回: SD卡返回的响应.
//========================================================================
uint8_t SDCARD::get_CSD(uint8_t *csd_data)
{
    uint8_t r1;
    r1 = send_cmd(CMD9,0,0x01);//发CMD9命令，读CSD
    if(r1 == 0)
    {
        r1 = recv_data(csd_data, 16);//接收16个字节的数据
    }
    dis_select();//取消片选
    if(r1)return 1;
    else return 0;
}

//========================================================================
// 描述: 获取SD卡的总扇区数（扇区数）.
// 参数: none
// 返回: 0： 取容量出错; 其他:SD卡的容量(扇区数/512字节).
//========================================================================
uint32_t SDCARD::get_sector_count(void)
{
    uint8_t csd[16];
    uint64_t Capacity;
    uint8_t n;
    uint16_t csize;
    //取CSD信息，如果期间出错，返回0
    if(get_CSD(csd)!=0) return 0;
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)  //V2.00的卡
    {
        csize = csd[9] + ((u16)csd[8] << 8) + 1;
        Capacity = (u32)csize << 10;//得到扇区数
    }else//V1.XX的卡
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
        Capacity= (u32)csize << (n - 9);//得到扇区数
    }
    return Capacity;
}

//========================================================================
// 描述: 初始化SD卡.
// 参数: none
// 返回: 0： 取容量出错; 其他:SD卡的容量(扇区数/512字节).
//========================================================================
uint8_t SDCARD::begin(void)
{
    uint8_t r1;      // 存放SD卡的返回值
    uint16_t retry;  // 用来进行超时计数
    uint8_t buf[4];
    uint16_t i;

    pinMode(_ss_pin, GPIO_Mode_Out_PP);
    hardspi.begin();
    //hardspi.set_speed(0);
    digitalWrite(_ss_pin, 1);  

    for(i=0;i<10;i++)wr_byte(0XFF);//发送最少74个脉冲
    retry=20;
    do
    {
        r1 = send_cmd(CMD0,0,0x95);//进入IDLE状态
    }while((r1!=0X01) && retry--);
    SD_Type=0;//默认无卡
    if(r1 == 0X01)
    {
        if(send_cmd(CMD8,0x1AA,0x87)==1)//SD V2.0
        {
            for(i=0;i<4;i++) //Get trailing return value of R7 resp
            {
                buf[i]=wr_byte(0XFF);
            }
            if(buf[2]==0X01 && buf[3]==0XAA)//卡是否支持2.7~3.6V
            {
                retry = 0XFFFE;
                do
                {
                    send_cmd(CMD55,0,0X01);   //发送CMD55
                    r1 = send_cmd(CMD41,0x40000000,0X01);//发送CMD41
                }while(r1&&retry--);
                if(retry&&send_cmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
                {
                    for(i=0;i<4;i++)buf[i]=wr_byte(0XFF);//得到OCR值
                    if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //检查CCS
                    else SD_Type=SD_TYPE_V2;
                }
            }
        }else//SD V1.x/ MMC V3
        {
            send_cmd(CMD55,0,0X01);       //发送CMD55
            r1 = send_cmd(CMD41,0,0X01);    //发送CMD41
            if(r1<=1)
            {
                SD_Type=SD_TYPE_V1;
                retry=0XFFFE;
                do //等待退出IDLE模式
                {
                    send_cmd(CMD55,0,0X01);   //发送CMD55
                    r1 = send_cmd(CMD41,0,0X01);//发送CMD41
                }while(r1&&retry--);
            }else//MMC卡不支持CMD55+CMD41识别
            {
                SD_Type=SD_TYPE_MMC;//MMC V3
                retry=0XFFFE;
                do //等待退出IDLE模式
                {
                    r1 = send_cmd(CMD1,0,0X01);//发送CMD1
                }while(r1&&retry--);
            }
            if(retry==0||send_cmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//错误的卡
        }
    }
    dis_select();//取消片选
    if(SD_Type)return 0;
    else if(r1)return r1;
    return 0xaa;//其他错误
}

//========================================================================
// 描述: 读SD卡.
// 参数: buf:数据缓存区
//       sector:扇区
//       cnt:扇区数
// 返回: 0,ok;其他,失败.
//========================================================================
uint8_t SDCARD::read_disk(uint8_t *buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
    if(cnt == 1)
    {
        r1 = send_cmd(CMD17,sector,0X01);//读命令
        if(r1==0)//指令发送成功
        {
            r1 = recv_data(buf,512);//接收512个字节
        }
    }else
    {
        r1 = send_cmd(CMD18,sector,0X01);//连续读命令
        do
        {
            r1 = recv_data(buf,512);//接收512个字节
            buf+=512;
        }while(--cnt && r1==0);
        send_cmd(CMD12,0,0X01);   //发送停止命令
    }
    dis_select();//取消片选
    return r1;//
}

//========================================================================
// 描述: 写SD卡.
// 参数: buf:数据缓存区
//       sector:起始扇区
//       cnt:扇区数
// 返回: 0,ok;其他,失败.
//========================================================================
uint8_t SDCARD::write_disk(uint8_t *buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址
    if(cnt == 1)
    {
        r1 = send_cmd(CMD24,sector,0X01);//写命令
        if(r1 == 0)//指令发送成功
        {
            r1 = send_block(buf,0xFE);//写512个字节，0xFE表示开始写
        }
    }else
    {
        if(SD_Type!=SD_TYPE_MMC)
        {
            send_cmd(CMD55,0,0X01);
            send_cmd(CMD23,cnt,0X01);//发送指令
        }
        r1 = send_cmd(CMD25,sector,0X01);//连续写命令
        if(r1==0)
        {
            do
            {
                r1 = send_block(buf,0xFC);//0xFC表示多块数据写入开始
                buf+=512;
            }while(--cnt && r1==0);
            r1 = send_block(0,0xFD);//0xFD表示多块数据写入结束
        }
    }
    dis_select();//取消片选
    return r1;
}

SDCARD sd(PC12);

#endif
