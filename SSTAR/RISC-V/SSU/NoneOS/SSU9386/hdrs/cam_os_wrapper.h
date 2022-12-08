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


///////////////////////////////////////////////////////////////////////////////
/// @file      cam_os_wrapper.h
/// @brief     Cam OS Wrapper Header File for
///            1. RTK OS
///            2. Linux User Space
///            3. Linux Kernel Space
///////////////////////////////////////////////////////////////////////////////

#ifndef __CAM_OS_WRAPPER_H__
#define __CAM_OS_WRAPPER_H__

#define CAM_OS_WRAPPER_VERSION "v1.0.44"

#define CAM_OS_BITS_PER_LONG 32

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char       u8;
typedef signed   char       s8;
typedef unsigned short      u16;
typedef signed   short      s16;
typedef unsigned int        u32;
typedef signed   int        s32;
typedef unsigned long long  u64;
typedef signed   long long  s64;

typedef u64 CamOsPhysAddr_t;

#include "cam_os_util.h"
#include "cam_os_util_list.h"
#include "cam_os_util_hash.h"
#include "cam_os_util_string.h"
#include "cam_os_struct.h"

typedef enum
{
    CAM_OS_OK               = 0,
    CAM_OS_FAIL             = -1,
    CAM_OS_PARAM_ERR        = -2,
    CAM_OS_ALLOCMEM_FAIL    = -3,
    CAM_OS_TIMEOUT          = -4,
    CAM_OS_RESOURCE_BUSY    = -5,
    CAM_OS_INTERRUPTED      = -6,
} CamOsRet_e;

typedef enum
{
    CAM_OS_TIME_DIFF_SEC    = 0,
    CAM_OS_TIME_DIFF_MS     = 1,
    CAM_OS_TIME_DIFF_US     = 2,
    CAM_OS_TIME_DIFF_NS     = 3,
} CamOsTimeDiffUnit_e;

typedef struct
{
    u32 nPriv[CAM_OS_MUTEX_SIZE];
} CamOsMutex_t;

typedef struct
{
    u32 nPriv[CAM_OS_TSEM_SIZE];
} CamOsTsem_t;

typedef struct
{
    u32 nPriv[CAM_OS_RWTSEM_SIZE];
} CamOsRwsem_t;

typedef struct
{
    u32 nPriv[CAM_OS_SPINLOCK_SIZE];
}CamOsSpinlock_t;

typedef struct
{
    u32 nSec;
    u32 nNanoSec;
} CamOsTimespec_t;

typedef struct
{
    u32 nPriority;      /* From 1(lowest) to 99(highest), use OS default priority if set 0 */
    u32 nStackSize;     /* If nStackSize is zero, use OS default value */
    char *szName;
    struct {
        u32 nRuntime;
        u32 nDeadline;
    } Sched;
} CamOsThreadAttrb_t, *pCamOsThreadAttrb;

typedef struct
{
    u32 nPriv[CAM_OS_TIMER_SIZE];
} CamOsTimer_t;

typedef struct
{
    u32 nPriv[CAM_OS_MEMCACHE_SIZE];
} CamOsMemCache_t;

typedef struct
{
    volatile s32 nCounter;
} CamOsAtomic_t;

typedef struct
{
    u32 nPriv[CAM_OS_IDR_SIZE];
} CamOsIdr_t;

typedef struct
{
    u64 nBytes;
    u16 nType;
    u16 nBusWidth;
} CamOsDramInfo_t;

typedef struct CamOsCpuMask
{
    u32 nPriv[CAM_OS_CPUMASK_SIZE];
} CamOsCpuMask_t;

typedef void * CamOsThread;

typedef void (*CamOsIrqHandler)(u32 nIrq, void *pDevId);

typedef void * CamOsWorkQueue;

typedef void * CamOsMsgQueue;

typedef void (*CamOsTimerCallback)(unsigned long pData);

//=============================================================================
// Description:
//      Writes the C string pointed by format to the standard output.
// Parameters:
//      [in]  szFmt: C string that contains the text to be written, it can
//                   optionally contain embedded format specifiers.
// Return:
//      N/A
//=============================================================================
void CamOsPrintf(const char *szFmt, ...);

//=============================================================================
// Description:
//      Reads data from stdin and stores them according to the parameter format
//      into the locations pointed by the additional arguments.
// Parameters:
//      [in]  szBuf: Pointer to a buffer where the resulting C-string is stored.
//                   The buffer should have a size of at least nSize characters.
//      [in]  nSize: Maximum number of bytes to be used in the buffer.
//                   The generated string has a length of at most nSize-1,
//                   leaving space for the additional terminating null character.
//      [in]  szFmt: C string that contains a format string, it can optionally
//                   contain embedded format specifiers.
// Return:
//      The number of items of the argument list successfully filled.
//=============================================================================
s32 CamOsSnprintf(char *szBuf, u32 nSize, const char *szFmt, ...);

//=============================================================================
// Description:
//      Initializes the semaphore at a given value.
// Parameters:
//      [in]  ptTsem: The semaphore to initialize.
//      [in]  nVal: the initial value of the semaphore.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns CamOsRet_e.
//=============================================================================
CamOsRet_e CamOsTsemInit(CamOsTsem_t *ptTsem, u32 nVal);

//=============================================================================
// Description:
//      Destroy the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to destroy.
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns CamOsRet_e.
//=============================================================================
CamOsRet_e CamOsTsemDeinit(CamOsTsem_t *ptTsem);

//=============================================================================
// Description:
//      Increases the value of the semaphore.
// Parameters:
//      [in]  ptTsem: The semaphore to increase.
// Return:
//      N/A
//=============================================================================
void CamOsTsemUp(CamOsTsem_t *ptTsem);

//=============================================================================
// Description:
//      Decreases the value of the semaphore. Blocks if the semaphore value is
//      zero.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease.
// Return:
//      N/A
//=============================================================================
void CamOsTsemDown(CamOsTsem_t *ptTsem);

//=============================================================================
// Description:
//      Decreases the value of the semaphore. Blocks if the semaphore value is
//      zero. This function is interruptible in Linux kernel.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease.
// Return:
//      N/A
//=============================================================================
CamOsRet_e CamOsTsemDownInterruptible(CamOsTsem_t *ptTsem);

//=============================================================================
// Description:
//      Decreases the value of the semaphore. Blocks if the semaphore value is
//      zero.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease.
//      [in]  nMsec: The value of delay for the timeout.
// Return:
//      If the timeout is reached the function exits with error CAM_OS_TIMEOUT.
//      CAM_OS_OK is returned if down successful; otherwise, returns CamOsRet_e.
//=============================================================================
CamOsRet_e CamOsTsemTimedDown(CamOsTsem_t *ptTsem, u32 nMsec);

//=============================================================================
// Description:
//      Always return as non-blocking mode. Decreases the value of the semaphore
//      if it is bigger than zero. If the semaphore value is less than or equal
//      to zero, return directly.
// Parameters:
//      [in]  ptTsem: The semaphore to decrease.
// Return:
//      CAM_OS_OK is returned if down successful; otherwise, returns CamOsRet_e.
//=============================================================================
CamOsRet_e CamOsTsemTryDown(CamOsTsem_t *ptTsem);

//=============================================================================
// Description:
//      Create a memory cache(memory pool) and allocate with specified size
//      to ignore internal fragmentation.
// Parameters:
//      [out] ptMemCache: Get memory cache information if create successfully.
//      [in]  szName: A string which is used in /proc/slabinfo to identify
//                    this cache(It's significant only in linux kernel).
//      [in]  nSize: Object size in this cache.
//      [in]  bHwCacheAlign: Align objs on cache lines(Only for Linux)
// Return:
//      CAM_OS_OK is returned if successful; otherwise, returns CamOsRet_e.
//=============================================================================
CamOsRet_e CamOsMemCacheCreate(CamOsMemCache_t *ptMemCache, char *szName, u32 nSize, u8 bHwCacheAlign);

//=============================================================================
// Description:
//      Destroy the memory cache.
// Parameters:
//      [in]  ptMemCache: The cache to destroy.
// Return:
//      N/A
//=============================================================================
void CamOsMemCacheDestroy(CamOsMemCache_t *ptMemCache);

//=============================================================================
// Description:
//      Allocate a memory block(object) from this memory cache.
// Parameters:
//      [in]  ptMemCache: The cache to be allocated.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void *CamOsMemCacheAlloc(CamOsMemCache_t *ptMemCache);

//=============================================================================
// Description:
//      Allocate a memory block(object) from this memory cache without sleep.
// Parameters:
//      [in]  ptMemCache: The cache to be allocated.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void *CamOsMemCacheAllocAtomic(CamOsMemCache_t *ptMemCache);

//=============================================================================
// Description:
//      Release a memory block(object) to this memory cache.
// Parameters:
//      [in]  ptMemCache: The cache to be released to.
//      [in]  pObjPtr: Pointer to a memory block(object) previously allocated by
//                     CamOsMemCacheAlloc.
// Return:
//      N/A
//=============================================================================
void CamOsMemCacheFree(CamOsMemCache_t *ptMemCache, void *pObjPtr);

//=============================================================================
// Description:
//      Unsigned 64bit divide with Unsigned 64bit divisor with remainder.
// Parameters:
//      [in]  nDividend: Dividend.
//      [in]  nDivisor: Divisor.
//      [out] pRemainder: Pointer to the remainder. This parameter can also be
//                        a null pointer, in which case it is not used.
// Return:
//      Quotient of division.
//=============================================================================
u64 CamOsMathDivU64(u64 nDividend, u64 nDivisor, u64 *pRemainder);

//=============================================================================
// Description:
//      Signed 64bit divide with signed 64bit divisor with remainder.
// Parameters:
//      [in]  nDividend: Dividend.
//      [in]  nDivisor: Divisor.
//      [out] pRemainder: Pointer to the remainder. This parameter can also be
//                        a null pointer, in which case it is not used.
// Return:
//      Quotient of division.
//=============================================================================
s64 CamOsMathDivS64(s64 nDividend, s64 nDivisor, s64 *pRemainder);

//=============================================================================
// Description:
//      Allocates a block of nSize bytes of memory, returning a pointer to the
//      beginning of the block.
// Parameters:
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void* CamOsMemAlloc(u32 nSize);

//=============================================================================
// Description:
//      Allocates a block of nSize bytes of memory without sleep, returning a
//      pointer to the beginning of the block.
// Parameters:
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void* CamOsMemAllocAtomic(u32 nSize);

//=============================================================================
// Description:
//      Allocates a block of memory for an array of nNum elements, each of them
//      nSize bytes long, and initializes all its bits to zero.
// Parameters:
//      [in]  nNum: Number of elements to allocate.
//      [in]  nSize: Size of each element.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void* CamOsMemCalloc(u32 nNum, u32 nSize);

//=============================================================================
// Description:
//      Allocates a block of memory for an array of nNum elements without sleep,
//      each of them nSize bytes long, and initializes all its bits to zero.
// Parameters:
//      [in]  nNum: Number of elements to allocate.
//      [in]  nSize: Size of each element.
// Return:
//      On success, a pointer to the memory block allocated by the function. If
//      failed to allocate, a null pointer is returned.
//=============================================================================
void* CamOsMemCallocAtomic(u32 nNum, u32 nSize);

//=============================================================================
// Description:
//      Changes the size of the memory block pointed to by pPtr. The function
//      may move the memory block to a new location (whose address is returned
//      by the function).
// Parameters:
//      [in]  pPtr: Pointer to a memory block previously allocated with
//                  CamOsMemAlloc, CamOsMemCalloc or CamOsMemRealloc.
//      [in]  nSize: New size for the memory block, in bytes.
// Return:
//      A pointer to the reallocated memory block, which may be either the same
//      as pPtr or a new location.
//=============================================================================
void* CamOsMemRealloc(void* pPtr, u32 nSize);

//=============================================================================
// Description:
//      Changes the size of the memory block pointed to by pPtr without sleep.
//      The function may move the memory block to a new location (whose address
//      is returned by the function).
// Parameters:
//      [in]  pPtr: Pointer to a memory block previously allocated with
//                  CamOsMemAlloc, CamOsMemCalloc or CamOsMemRealloc.
//      [in]  nSize: New size for the memory block, in bytes.
// Return:
//      A pointer to the reallocated memory block, which may be either the same
//      as pPtr or a new location.
//=============================================================================
void* CamOsMemReallocAtomic(void* pPtr, u32 nSize);

//=============================================================================
// Description:
//      Flush data in cache
// Parameters:
//      [in]  pPtr: Virtual start address
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      N/A
//=============================================================================
void CamOsMemFlush(void* pPtr, u32 nSize);

//=============================================================================
// Description:
//      Flush data in inner and outer cache
// Parameters:
//      [in]  pVa: Virtual start address
//      [in]  pPa: Physical start address
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      N/A
//=============================================================================
void CamOsMemFlushExt(void* pVa, void* pPa, u32 nSize);

//=============================================================================
// Description:
//      Invalidate data in cache
// Parameters:
//      [in]  pPtr: Virtual start address
//      [in]  nSize: Size of the memory block, in bytes.
// Return:
//      N/A
//=============================================================================
void CamOsMemInvalidate(void* pPtr, u32 nSize);

//=============================================================================
// Description:
//      A block of memory previously allocated by a call to CamOsMemAlloc,
//      CamOsMemCalloc or CamOsMemRealloc is deallocated, making it available
//      again for further allocations. If pPtr is a null pointer, the function
//      does nothing.
// Parameters:
//      [in]  pPtr: Pointer to a memory block previously allocated with
//                  CamOsMemAlloc, CamOsMemCalloc or CamOsMemRealloc.
// Return:
//      N/A
//=============================================================================
void CamOsMemRelease(void* pPtr);

//=============================================================================
// Description:
//      Transfer virtual address to physical address.
// Parameters:
//      [in]  pPtr: Virtual address.
// Return:
//      Physical address.
//=============================================================================
CamOsPhysAddr_t CamOsMemVirtToPhys(void* pPtr);

//=============================================================================
// Description:
//      Transfer physical address to MIU address.
// Parameters:
//      [in]  pPtr: Physical address.
// Return:
//      MIU address.
//=============================================================================
void* CamOsMemPhysToMiu(CamOsPhysAddr_t pPtr);

//=============================================================================
// Description:
//      Transfer MIU address to physical address.
// Parameters:
//      [in]  pPtr: MIU address.
// Return:
//      Physical address.
//=============================================================================
CamOsPhysAddr_t CamOsMemMiuToPhys(void* pPtr);

//=============================================================================
// Description:
//      Busy-delay execution for millisecond intervals.
// Parameters:
//      [in]  nMsec: Millisecond to busy-delay.
// Return:
//      N/A
//=============================================================================
void CamOsMsDelay(u32 nMsec);

//=============================================================================
// Description:
//      Busy-delay execution for microsecond intervals.
// Parameters:
//      [in]  nUsec: Microsecond to busy-delay.
// Return:
//      N/A
//=============================================================================
void CamOsUsDelay(u32 nUsec);

//=============================================================================
// Description:
//      Gets the current time in milliseconds.
// Parameters:
//      N/A
// Return:
//      time in milliseconds.
//=============================================================================
u64 CamOsGetTimeInMs(void);

//=============================================================================
// Description:
//      Free an interrupt allocated with request_irq.
// Parameters:
//      [in]  nIrq: Interrupt line to allocate.
//      [in]  pfnHandler: Function to be called when the IRQ occurs.
//      [in]  szName: An ascii name for the claiming device.
//      [in]  pDevId: A cookie passed back to the handler function.
// Return:
//      N/A
//=============================================================================
CamOsRet_e CamOsIrqRequest(u32 nIrq, CamOsIrqHandler pfnHandler, const char *szName, void *pDevId);

//=============================================================================
// Description:
//      Free an interrupt allocated with request_irq.
// Parameters:
//      [in]  nIrq: Interrupt line to free.
//      [in]  pDevId: Device identity to free.
// Return:
//      N/A
//=============================================================================
void CamOsIrqFree(u32 nIrq, void *pDevId);

//=============================================================================
// Description:
//      Enable handling of an irq.
// Parameters:
//      [in]  nIrq: Interrupt to enable.
// Return:
//      N/A
//=============================================================================
void CamOsIrqEnable(u32 nIrq);

//=============================================================================
// Description:
//      Disable an irq and wait for completion.
// Parameters:
//      [in]  nIrq: Interrupt to disable.
// Return:
//      N/A
//=============================================================================
void CamOsIrqDisable(u32 nIrq);

//=============================================================================
// Description:
//      Convert string to long integer with specific base.
// Parameters:
//      [in]  szStr: String beginning with the representation of
//                   an integral number.
//      [in]  szEndptr: Reference to an object of type char*, whose value
//                      is set by the function to the next character in szStr
//                      after the numerical value.
//                      This parameter can also be a null pointer, in which
//                      case it is not used.
//      [in]  nBase: Numerical base (radix) that determines the valid characters
//                   and their interpretation. If this is 0, the base used is
//                   determined by the format in the sequence (see above).
// Return:
//      Converted long integer.
//=============================================================================
long CamOsStrtol(const char *szStr, char** szEndptr, s32 nBase);

//=============================================================================
// Description:
//      Convert string to unsigned long integer with specific base.
// Parameters:
//      [in]  szStr: String beginning with the representation of
//                   an integral number.
//      [in]  szEndptr: Reference to an object of type char*, whose value
//                      is set by the function to the next character in szStr
//                      after the numerical value.
//                      This parameter can also be a null pointer, in which
//                      case it is not used.
//      [in]  nBase: Numerical base (radix) that determines the valid characters
//                   and their interpretation. If this is 0, the base used is
//                   determined by the format in the sequence (see above).
// Return:
//      Converted long integer.
//=============================================================================
unsigned long CamOsStrtoul(const char *szStr, char** szEndptr, s32 nBase);

//=============================================================================
// Description:
//      Convert string to unsigned long long integer with specific base.
// Parameters:
//      [in]  szStr: String beginning with the representation of
//                   an integral number.
//      [in]  szEndptr: Reference to an object of type char*, whose value
//                      is set by the function to the next character in szStr
//                      after the numerical value.
//                      This parameter can also be a null pointer, in which
//                      case it is not used.
//      [in]  nBase: Numerical base (radix) that determines the valid characters
//                   and their interpretation. If this is 0, the base used is
//                   determined by the format in the sequence (see above).
// Return:
//      Converted long integer.
//=============================================================================
unsigned long long CamOsStrtoull(const char *szStr, char** szEndptr, s32 nBase);

#endif /* __CAM_OS_WRAPPER_H__ */
