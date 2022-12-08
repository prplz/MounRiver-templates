/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

#ifndef __SYS_MSWRAPPER_CUS_OS_ATOMIC_H__
#define __SYS_MSWRAPPER_CUS_OS_ATOMIC_H__

/*------------------------------------------------------------------------------
    Include Files
-------------------------------------------------------------------------------*/

#include "sys_MsWrapper_cus_os_type.h"

typedef struct
{
    volatile s32 nCounter;
} MsAtomic_t;

//=============================================================================
// Description:
//      Read atomic variable.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicRead(MsAtomic_t *ptAtomic);

//=============================================================================
// Description:
//      Set atomic variable.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Required value.
// Return:
//      N/A
//=============================================================================
void MsAtomicSet(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      Add to the atomic variable and return value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to add.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicAddReturn(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      Subtract the atomic variable and return value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to subtract.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicSubReturn(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      Subtract value from variable and test result.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to subtract.
// Return:
//      Returns true if the result is zero, or false for all other cases.
//=============================================================================
s32 MsAtomicSubAndTest(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      Increment atomic variable and return value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicIncReturn(MsAtomic_t *ptAtomic);

//=============================================================================
// Description:
//      decrement atomic variable and return value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicDecReturn(MsAtomic_t *ptAtomic);

//=============================================================================
// Description:
//      Increment and test result.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
// Return:
//      Returns true if the result is zero, or false for all other cases.
//=============================================================================
s32 MsAtomicIncAndTest(MsAtomic_t *ptAtomic);

//=============================================================================
// Description:
//      Decrement and test result.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
// Return:
//      Returns true if the result is zero, or false for all other cases.
//=============================================================================
s32 MsAtomicDecAndTest(MsAtomic_t *ptAtomic);

//=============================================================================
// Description:
//      Add to the atomic variable and test if negative.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to subtract.
// Return:
//      Returns true if the result is negative, or false when result is greater
//      than or equal to zero.
//=============================================================================
s32 MsAtomicAddNegative(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      Read the 32-bit value (referred to as nOldValue) stored at location pointed by ptAtomic.
//      Compute (nOldValue == cmp) ? val : nOldValue and store result at location pointed by ptAtomic. The function returns nOldValue
// Parameters:
//      [in]  ptr: Pointer of type void
//      [in]  nOldValue: old value.
//      [in]  nNewValue : new value

// Return:
//      Returns true if the val is changed into new value
//=============================================================================
s32 MsAtomicCompareAndSwap(MsAtomic_t *ptAtomic, s32 nOldValue, s32 nNewValue);

//=============================================================================
// Description:
//      AND operation with the atomic variable and return the new value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to AND.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicAndFetch(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      AND operation with the atomic variable and returns the value that had
//      previously been in memory.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to AND.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicFetchAnd(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      NAND operation with the atomic variable and return the new value.
//      GCC 4.4 and later implement NAND as "~(ptAtomic & nValue)" instead of
//      "~ptAtomic & nValue".
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to NAND.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicNandFetch(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      NAND operation with the atomic variable and returns the value that had
//      previously been in memory. GCC 4.4 and later implement NAND as
//      "~(ptAtomic & nValue)" instead of "~ptAtomic & nValue".
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to NAND.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicFetchNand(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      OR operation with the atomic variable and return the new value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to OR.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicOrFetch(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      OR operation with the atomic variable and returns the value that had
//      previously been in memory.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to OR.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicFetchOr(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      XOR operation with the atomic variable and return the new value.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to XOR.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicXorFetch(MsAtomic_t *ptAtomic, s32 nValue);

//=============================================================================
// Description:
//      XOR operation with the atomic variable and returns the value that had
//      previously been in memory.
// Parameters:
//      [in]  ptAtomic: Pointer of type MsAtomic_t.
//      [in]  nValue: Integer value to XOR.
// Return:
//      The value of ptAtomic.
//=============================================================================
s32 MsAtomicFetchXor(MsAtomic_t *ptAtomic, s32 nValue);

#endif /* __SYS_MSWRAPPER_CUS_OS_ATOMIC_H__ */
