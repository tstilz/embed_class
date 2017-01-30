/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename      : app_main.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "stdarg.h"
#include  "stdio.h"
#include  "stm32f7xx_hal.h"

#include  "cpu.h"
#include  "lib_math.h"
#include  "lib_mem.h"
#include  "os.h"
#include  "os_app_hooks.h"

#include  "app_cfg.h"
#include  "bsp.h"
#include  "bsp_led.h"
#include  "bsp_clock.h"
#include  "bsp_pb.h"
#include  "bsp_test.h"
#include  "GUI.h"


// Task priorities
#define TASK_LED1_PRIO       (14UL)

// Task stack size(s)
#define TASK_LED_STK_SIZE    (192UL)


// *****************************************************************
// Define storage for each Task Control Block (TCB) and task stacks
// *****************************************************************
static  OS_TCB   AppTaskGUI_TCB;
static  CPU_STK  AppTaskGUI_Stk[APP_CFG_TASK_GUI_STK_SIZE];

static  OS_TCB   TaskLED1_TCB;
static  CPU_STK  TaskLED1_Stk[TASK_LED_STK_SIZE];

// Flash LED1 at 1 Hz (500ms on / 500ms off)
// *****************************************************************
static void led1_task(void * p_arg)
{

#if OS_CFG_STAT_TASK_EN > 0u
    // Compute CPU capacity with no other task running
    OSStatTaskCPUUsageInit(&err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    for (;;)
    {
        // TODO: Toggle LED1

        // TODO: Sleep for 500ms
    }
}

// *****************************************************************
int main(void)
{
    OS_ERR   err;

    HAL_Init();
    BSP_SystemClkCfg();   // Init. system clock frequency to 200MHz
    CPU_Init();           // Initialize the uC/CPU services
    Mem_Init();           // Initialize Memory Managment Module
    Math_Init();          // Initialize Mathematical Module
    CPU_IntDis();         // Disable all Interrupts.

    // TODO: Init uC/OS-III.

    // Create the GUI task
    OSTaskCreate(&AppTaskGUI_TCB, "uC/GUI Task", (OS_TASK_PTR ) GUI_DemoTask,
                 0, APP_CFG_TASK_GUI_PRIO,
                 &AppTaskGUI_Stk[0], (APP_CFG_TASK_GUI_STK_SIZE / 10u),
                  APP_CFG_TASK_GUI_STK_SIZE, 0u, 0u, 0,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    my_assert(OS_ERR_NONE == err);
 
    // TODO: Create task to blink LED1

    // TODO: Start multitasking (i.e. give control to uC/OS-III)

}

