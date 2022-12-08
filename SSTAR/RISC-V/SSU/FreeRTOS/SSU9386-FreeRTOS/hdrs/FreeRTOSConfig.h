/*
 * FreeRTOS Kernel V10.4.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/*
 * The FreeRTOS Cortex-A port implements a full interrupt nesting model.
 *
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * The Renesas RZ implements 32 unique interrupt priorities.  For the purpose of
 * setting configMAX_API_CALL_INTERRUPT_PRIORITY 31 represents the lowest
 * priority.
 */

//////////////////////////////////////////////////////////////////////////////////

#define configMTIME_BASE_ADDRESS                ( 0x0001000CUL )
#define configMTIMECMP_BASE_ADDRESS             ( 0x00010004UL )

#ifndef CONFIG_FPGA_VERIFY
#define configRTC_CLOCK_HZ                      (12000000UL)
#else
//RTC clock
#define configRTC_CLOCK_HZ                      (32000UL)
#endif

#define configSUPPORT_STATIC_ALLOCATION         1

/* Tick definitions. */
#ifdef CONFIG_KERNEL_HZ
#define configTICK_RATE_HZ                      ((portTickType) CONFIG_KERNEL_HZ)
#else
#define configTICK_RATE_HZ                      ((portTickType) 1000)
#endif
#define configPERIPHERAL_CLOCK_HZ               (12000000UL)
#define configUSE_16_BIT_TICKS                  0

/* Task definitions. */
#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_TIME_SLICING                  1
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     1
#if (configUSE_PORT_OPTIMISED_TASK_SELECTION)
#define configMAX_PRIORITIES                    32
#define configRSVD_PRIORITIES                   2
#else
#define configMAX_PRIORITIES                    128//256
#define configRSVD_PRIORITIES                   28//156
#endif
#define configMINIMAL_STACK_SIZE                512 // Unit:Word
#define configMAX_TASK_NAME_LEN                 16
#define configCHECK_FOR_STACK_OVERFLOW          2
#if defined(CONFIG_FREERTOS_POSIX_SUPPORT)
#define configUSE_APPLICATION_TASK_TAG          1
#define configUSE_POSIX_ERRNO                   1
#else
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_POSIX_ERRNO                   0
#endif
#define configUSE_TASK_NOTIFICATIONS            0
#if defined(CONFIG_FREERTOS_PLUS_FAT_SUPPORT)
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
#else
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 2
#endif
#define configRECORD_STACK_HIGH_ADDRESS         1

#if defined(CONFIG_VFP_SUPPORT)
#if defined(CONFIG_VFP_LAZY_SWITCH_SUPPORT)
#define configUSE_TASK_FPU_SUPPORT              1
#else
#define configUSE_TASK_FPU_SUPPORT              2
#endif
#endif

#define configRUN_TIME_COUNTER_TYPE             uint64_t

#define configUSE_NEWLIB_REENTRANT              0

// Sigma Unique Config
#define CONFIG_LOG_TASK_ENTRY_ADDR_SUPPORT
//#define CONFIG_LOG_TASK_EXEC_CNT_SUPPORT
//#define CONFIG_LOG_SPINLOCK_TIME_SUPPORT
//#define CONFIG_LOG_TASK_MAX_STACK_USAGE_SUPPORT (0 && configRECORD_STACK_HIGH_ADDRESS)

/* Memory management definitions. */
#define configUSE_MALLOC_FAILED_HOOK            1
#define configTOTAL_HEAP_SIZE                   ((size_t) 0) // NOT USED in heap_SS.c

/* Signal definitions. */
#define configUSE_MUTEXES                       1
#if defined(CONFIG_FREERTOS_POSIX_SUPPORT) || defined(CONFIG_FREERTOS_PLUS_FAT_SUPPORT)
#define configUSE_RECURSIVE_MUTEXES             1
#else
#define configUSE_RECURSIVE_MUTEXES             0
#endif
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_QUEUE_SETS                    0
#define configQUEUE_REGISTRY_SIZE               0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         2

/* Software timer definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            (configMINIMAL_STACK_SIZE)

#if defined(CONFIG_SMP_SUPPORT)
#if (CONFIG_CORE_NUM > 1)
#define configUSE_MINIMAL_IDLE_HOOK             1
#define configIDLE_SHOULD_YIELD                 0
#else
#define configUSE_MINIMAL_IDLE_HOOK             0
#define configIDLE_SHOULD_YIELD                 0
#endif
#define configNUM_CORES                         (CONFIG_CORE_NUM)
#define configRUN_MULTIPLE_PRIORITIES           1
#define configUSE_TASK_PREEMPTION_DISABLE       1
#define configUSE_CORE_AFFINITY                 1
#else // defined(CONFIG_SMP_SUPPORT)
#define configUSE_MINIMAL_IDLE_HOOK             0
#define configIDLE_SHOULD_YIELD                 0
#define configNUM_CORES                         1
#define configRUN_MULTIPLE_PRIORITIES           0
#define configUSE_TASK_PREEMPTION_DISABLE       0
#define configUSE_CORE_AFFINITY                 0
#endif // defined(CONFIG_SMP_SUPPORT)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#if defined(CONFIG_FREERTOS_FULLY_API)
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskGetIdleTaskHandle          1
#else
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskCleanUpResources           0
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 0
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xSemaphoreGetMutexHolder        0
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#endif

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#if defined(CONFIG_FREERTOS_TASK_TRACE)
#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_TRACE_FACILITY                1
#else
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#endif
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

//////////////////////////////////////////////////////////////////////////////////

/* Run time stats gathering definitions. */
void vInitialiseRunTimeStats(void);
configRUN_TIME_COUNTER_TYPE ulGetRunTimeCounterValue(void);

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vInitialiseRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()        ulGetRunTimeCounterValue()

/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function. FreeRTOS_Tick_Handler() must
 * be installed as the peripheral's interrupt handler.
 */
void vConfigureTickInterrupt(void);

#define configSETUP_TICK_INTERRUPT()            vConfigureTickInterrupt()

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
void vAssertCalled(const char * pcFile, unsigned long ulLine);

#define configASSERT( x )                       if ((x) == 0) vAssertCalled(__FILE__, __LINE__);

#endif /* FREERTOS_CONFIG_H */
