/**********************************************************
 * 该库为CH32V103的内部flash库.
 * *******************************************************/
#ifndef _CH32V_FLASH_H
#define _CH32V_FLASH_H

#include "CH32V103.h"
#include "string.h"

/* Global define */
#define FLASH_SIZE             (128*512)
#define FLASH_PAGE_SIZE        128
#define FLASH_START_ADDR       ((uint32_t)0x08000000)
#define FLASH_END_ADDR         ((uint32_t)0x08010000)

#define FLASH_PAGES_TO_BE_PROTECTED  FLASH_WRProt_Pages60to63

class INTERNAL_FLASH {
public:
	INTERNAL_FLASH(void);
	~INTERNAL_FLASH(void);
		
	FLASH_Status erase_page(uint32_t page);
	FLASH_Status FLASH_erase_page_all(void);  //擦除flash全部数据

	int8_t read(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes);
	int8_t readChar(uint32_t data_addr);
	uint8_t readUChar(uint32_t data_addr);
	int16_t readShort(uint32_t data_addr);
	uint16_t readUShort(uint32_t data_addr);
	int32_t readInt(uint32_t data_addr);
	uint32_t readUInt(uint32_t data_addr);
	int64_t readLong64(uint32_t data_addr);
	uint64_t readULong64(uint32_t data_addr);
	float readFloat(uint32_t data_addr);
	double readDouble(uint32_t data_addr);
	template <class T> T readALL (uint32_t address, T &);

	void write(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes);
	FLASH_Status writeChar(uint32_t data_addr,char *Data,uint32_t count);
	FLASH_Status writeUChar(uint32_t data_addr,uint8_t *Data,uint32_t count);
	FLASH_Status writeShort(uint32_t data_addr,int16_t *Data,uint32_t count);
	FLASH_Status writeUShort(uint32_t data_addr,uint16_t *Data,uint32_t count);
	FLASH_Status writeInt(uint32_t data_addr,int32_t *Data,uint32_t count);
	FLASH_Status writeUInt(uint32_t data_addr,uint32_t *Data,uint32_t count);
	FLASH_Status writeLong64(uint32_t data_addr,int64_t *Data,uint32_t count);
	FLASH_Status writeULong64(uint32_t data_addr,uint64_t *Data,uint32_t count);
	FLASH_Status writeFloat(uint32_t data_addr,float *Data,uint32_t count);
	FLASH_Status writeDouble(uint32_t data_addr,double *Data,uint32_t count);

	FLASH_Status write_half_word(uint32_t data_addr, uint16_t buf);
	void write_128bytes(uint32_t data_addr, uint8_t *dat);
protected:		
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
};

INTERNAL_FLASH::INTERNAL_FLASH(void)
{
}

INTERNAL_FLASH::~INTERNAL_FLASH() 
{
}

//========================================================================
// 描述: flash整片擦除.
// 参数: none
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::FLASH_erase_page_all(void)
{
	uint32_t WRPR_Value;
    FLASH_Unlock();
    WRPR_Value = FLASH_GetWriteProtectionOptionByte();    //写保护
    if ( (WRPR_Value & FLASH_PAGES_TO_BE_PROTECTED) != 0x00){
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
        FLASHStatus = FLASH_EraseAllPages();  //整片擦除
    }
    FLASH_Lock();
    return FLASHStatus;
}

//========================================================================
// 描述: flash页擦除.
// 参数: page：擦除哪一页（CH32V103R8T6一共512页(0-511)，每页128byte）
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::erase_page(uint32_t page)
{
	if(page < 64)return FLASH_ERROR_PG;
    FLASH_Unlock_Fast();
    FLASH_ErasePage_Fast(FLASH_START_ADDR + (FLASH_PAGE_SIZE * page));  //擦除页
    FLASH_Lock_Fast();
    return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash从指定地址读出指定长度的数据.
// 参数: data_addr:地址; 
// 返回: none.
//========================================================================
int8_t INTERNAL_FLASH::read(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes)
{
	// memcpy((void*)buf,(const void*)(data_addr),(size_t)num_bytes);
	for(uint32_t i = 0; i < num_bytes;i++)
	{
		buf[i] = (*(__IO uint8_t*)(data_addr+i));
	}
	return 1;
}

//========================================================================
// 描述: flash读取有符号8位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
int8_t INTERNAL_FLASH::readChar(uint32_t data_addr)
{
	int8_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取无符号8位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
uint8_t INTERNAL_FLASH::readUChar(uint32_t data_addr)
{
	uint8_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取有符号16位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
int16_t INTERNAL_FLASH::readShort(uint32_t data_addr)
{
	int16_t value = 0;
	return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取无符号16位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
uint16_t INTERNAL_FLASH::readUShort(uint32_t data_addr)
{
    uint16_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取有符号32位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
int32_t INTERNAL_FLASH::readInt(uint32_t data_addr)
{
    int32_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取无符号32位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
uint32_t INTERNAL_FLASH::readUInt(uint32_t data_addr)
{
    uint32_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取有符号64位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
int64_t INTERNAL_FLASH::readLong64(uint32_t data_addr)
{
    int64_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取无符号64位数据.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
uint64_t INTERNAL_FLASH::readULong64(uint32_t data_addr)
{
    uint64_t value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取float类型小数.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
float INTERNAL_FLASH::readFloat(uint32_t data_addr)
{
    float value=0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取double类型小数.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
double INTERNAL_FLASH::readDouble(uint32_t data_addr)
{
    double value = 0;
    return readALL(data_addr,value);
}

//========================================================================
// 描述: flash读取模板.
// 参数: data_addr:地址
// 返回: none.
//========================================================================
template <class T> T INTERNAL_FLASH::readALL(uint32_t data_addr,T &value)
{
    if( (data_addr-FLASH_START_ADDR + sizeof(T)) > FLASH_SIZE )
        return value;
    memcpy((uint8_t*) &value,(uint8_t*)(data_addr),sizeof(T));
    return value;
}

//========================================================================
// 描述: flash写入一个半字数据.
// 参数: data_addr:地址
//       dat:半字数据
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::write_half_word(uint32_t data_addr, uint16_t buf)
{
    FLASH_Unlock();
    FLASHStatus = FLASH_ProgramHalfWord(data_addr, buf);
    FLASH_Lock();
    return FLASHStatus;
}

//========================================================================
// 描述: flash快速写入128字节数据.
// 参数: data_addr:地址
//       buf:数组指针（128字节）
// 返回: none.
//========================================================================
void INTERNAL_FLASH::write_128bytes(uint32_t data_addr, uint8_t *buf)
{
	uint32_t page_buf[32];
	uint32_t data_32 = 0;
	for (int i = 0; i < 32; i++)
  	{
		data_32 = buf[i * 4 + 3];
		data_32 <<= 8;
		data_32 |= buf[i * 4 + 2];
		data_32 <<= 8;
		data_32 |= buf[i * 4 + 1];
		data_32 <<= 8;
		data_32 |= buf[i * 4];
		page_buf[i] = data_32;
  	}
    FLASH_Unlock_Fast();
    FLASH_BufReset();
	FLASH_BufLoad(data_addr, page_buf[0], page_buf[1], page_buf[2], page_buf[3]);
	FLASH_BufLoad(data_addr + 0x10, page_buf[4], page_buf[5], page_buf[6], page_buf[7]);
	FLASH_BufLoad(data_addr + 0x20, page_buf[8], page_buf[9], page_buf[10], page_buf[11]);
	FLASH_BufLoad(data_addr + 0x30, page_buf[12], page_buf[13], page_buf[14], page_buf[15]);
	FLASH_BufLoad(data_addr + 0x40, page_buf[16], page_buf[17], page_buf[18], page_buf[19]);
	FLASH_BufLoad(data_addr + 0x50, page_buf[20], page_buf[21], page_buf[22], page_buf[23]);
	FLASH_BufLoad(data_addr + 0x60, page_buf[24], page_buf[25], page_buf[26], page_buf[27]);
	FLASH_BufLoad(data_addr + 0x70, page_buf[28], page_buf[29], page_buf[30], page_buf[31]);
    FLASH_ProgramPage_Fast(data_addr);
    FLASH_Lock_Fast();
}

//========================================================================
// 描述: flash在指定地址写入指定长度的数据.
// 参数: data_addr:地址
//       buf:数据缓冲区
//		 num_bytes：要写入的字节数
// 返回: none.
//========================================================================
void INTERNAL_FLASH::write(uint32_t data_addr, uint8_t *buf, uint16_t num_bytes)
{
	uint32_t secpos;	//页地址
	uint16_t secoff;  //在页地址的偏移
	uint16_t secremain;	   
 	uint16_t i;
	uint8_t flash_buf[FLASH_PAGE_SIZE];   
	data_addr =  data_addr - FLASH_START_ADDR;
	secpos = data_addr/FLASH_PAGE_SIZE;  //页地址
	secoff = data_addr%FLASH_PAGE_SIZE;  //在页地址内的偏移
	secremain = FLASH_PAGE_SIZE-secoff;//该页的剩余空间大小   
	if(num_bytes <= secremain)	
	{
		secremain = num_bytes;
	}
	while(1)
	{
		read(FLASH_START_ADDR+secpos*FLASH_PAGE_SIZE, flash_buf, FLASH_PAGE_SIZE);	//读出整个扇区的内容
		for(i=0;i<secremain;i++)
		{
			if(flash_buf[secoff+i]!=0XFF){
				erase_page(secpos);	//擦除整页
				break;
			}
		}
		for(i=0;i<secremain;i++)	    //复制
		{
			flash_buf[secoff+i] = buf[i];	  
		}
		write_128bytes(FLASH_START_ADDR+secpos*FLASH_PAGE_SIZE, flash_buf);
		if(num_bytes == secremain)break;	//全部写入完成
		else{
			secpos++;	//页地址增1
			secoff = 0;	//偏移清零
		   	buf+=secremain;  //要写入的数据地址偏移
			data_addr+=secremain;//flash地址偏移	  
			num_bytes-=secremain;	//要写入的字节数递减
			if(num_bytes>128)secremain = 128;	//下一页还是写不完
			else secremain = num_bytes;	  //下一个扇区可以写完了
		}				
	}
}

//========================================================================
// 描述: flash写入有符号8位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeChar(uint32_t data_addr,char *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint8_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count);
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入无符号8位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUChar(uint32_t data_addr,uint8_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint8_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count);
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入有符号16位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeShort(uint32_t data_addr,int16_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int16_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int16_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入无符号16位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUShort(uint32_t data_addr,uint16_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint16_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint16_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入有符号32位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeInt(uint32_t data_addr,int32_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int32_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int32_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入无符号32位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeUInt(uint32_t data_addr,uint32_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint32_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint32_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入有符号64位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeLong64(uint32_t data_addr,int64_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(int64_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(int64_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入无符号64位数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeULong64(uint32_t data_addr,uint64_t *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(uint64_t) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(uint64_t));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入float类型数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeFloat(uint32_t data_addr,float *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(float) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(float));
	return FLASH_COMPLETE;
}

//========================================================================
// 描述: flash写入double类型数据.
// 参数: data_addr:要写入的flash地址
//       Data:要写入的数据指针
//       count:要写入的数据长度  
// 返回: none.
//========================================================================
FLASH_Status INTERNAL_FLASH::writeDouble(uint32_t data_addr,double *Data,uint32_t count)
{
	if(data_addr < 0x08002000) return FLASH_ERROR_PG;
    if (data_addr + count*sizeof(double) > FLASH_END_ADDR) 	//超出flash范围
	{	
		return FLASH_ERROR_PG;
    }
	write(data_addr, (uint8_t *)Data,  count*sizeof(double));
	return FLASH_COMPLETE;
}

#endif









