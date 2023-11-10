/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32l103_crc.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/07/08
 * Description        : This file contains all the functions prototypes for the
 *                      CRC firmware library.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32L103_CRC_H
#define __CH32L103_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32l103.h"

void     CRC_ResetDR(void);
uint32_t CRC_CalcCRC(uint32_t Data);
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
uint32_t CRC_GetCRC(void);
void     CRC_SetIDRegister(uint8_t IDValue);
uint8_t  CRC_GetIDRegister(void);

#ifdef __cplusplus
}
#endif

#endif