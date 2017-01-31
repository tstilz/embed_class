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
#define TASK_STARTUP_PRIO   (20UL)
#define TASK_LED_PRIO       (14UL)

// Task stack size(s)
#define TASK_LED_STK_SIZE     (192UL)
#define TASK_STARTUP_STK_SIZE (192UL)


// *****************************************************************
// Define storage for each Task Control Block (TCB) and task stacks
// *****************************************************************
static  OS_TCB   AppTaskGUI_TCB;
static  CPU_STK  AppTaskGUI_Stk[APP_CFG_TASK_GUI_STK_SIZE];

static  OS_TCB   StartupTask_TCB;
static  CPU_STK  StartupTask_Stk[TASK_LED_STK_SIZE];

static  OS_TCB   TaskLED_TCB[3];
static  CPU_STK  TaskLED_Stk[3][TASK_LED_STK_SIZE];
static  OS_MUTEX Led_Mutex;

// static  OS_ERR   err;

typedef struct
{
    uint8_t    LED;
    uint16_t   period_ms;
    OS_MUTEX * p_Led_Mutex

} LED_Task_arg;

// Flash LED1 at 1 Hz (500ms on / 500ms off)
// Flash LED1 at 5 Hz (100ms on / 100ms off)
// *****************************************************************
static void led_task(void * p_arg)
{
    LED_Task_arg * p_LED_Task_arg = (LED_Task_arg *) p_arg;
    OS_ERR  err;
    CPU_TS  ts;
    for (;;)
    {

        OSMutexPend((OS_MUTEX *) p_LED_Task_arg->p_Led_Mutex,
                    (OS_TICK   ) 0,
                    (OS_OPT    ) OS_OPT_PEND_BLOCKING,
                    (CPU_TS   *) &ts,
                    (OS_ERR   *) &err);
        my_assert(OS_ERR_NONE == err);

        BSP_LED_Toggle(p_LED_Task_arg->LED);

        OSMutexPost((OS_MUTEX *) p_LED_Task_arg->p_Led_Mutex,
                    (OS_OPT    ) OS_OPT_POST_NONE,
                    (OS_ERR   *) &err);
        my_assert(OS_ERR_NONE == err);

        OSTimeDlyHMSM(0,0,0, p_LED_Task_arg->period_ms, OS_OPT_TASK_NONE, &err);
        my_assert(OS_ERR_NONE == err);
    }
}

// Flash LED1 at 1 Hz (500ms on / 500ms off)
// *****************************************************************
static void startup_task(void * p_arg)
{
    OS_ERR  err;
    BSP_Init();

    LED_Task_arg LED_Task_arg[3];

    LED_Task_arg[0].LED         = LED1;
    LED_Task_arg[0].period_ms   = 500;
    LED_Task_arg[0].p_Led_Mutex = &Led_Mutex;

    LED_Task_arg[1].LED         = LED2;
    LED_Task_arg[1].period_ms   = 100;
    LED_Task_arg[1].p_Led_Mutex = &Led_Mutex;

    OSMutexCreate (&Led_Mutex,
                   "LED_MUTEX",
                   &err);
    my_assert(OS_ERR_NONE == err);

    // Create the BLINKY task
    for(int i=0; i<2; i++)
    {
        OSTaskCreate(&TaskLED_TCB[i],
                     "Blinky Task",
                     (OS_TASK_PTR ) led_task,
                     &LED_Task_arg[i],
                     TASK_LED_PRIO + i,
                     &TaskLED_Stk[i][0],
                     (TASK_LED_STK_SIZE / 10u),
                     TASK_LED_STK_SIZE,
                     0u,
                     0u,
                     0,
                     (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     &err);
        my_assert(OS_ERR_NONE == err);
    }

    for (;;)
    {
        // TODO: Toggle LED1
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

    BSP_LED_Toggle(LED1);

    OSInit(&err);
    my_assert(OS_ERR_NONE == err);

    // Create the GUI task
    OSTaskCreate(&AppTaskGUI_TCB,
                 "uC/GUI Task",
                 (OS_TASK_PTR ) GUI_DemoTask,
                 0,
                 APP_CFG_TASK_GUI_PRIO,
                 &AppTaskGUI_Stk[0],
                 (APP_CFG_TASK_GUI_STK_SIZE / 10u),
                 APP_CFG_TASK_GUI_STK_SIZE,
                 0u,
                 0u,
                 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err
            );
    my_assert(OS_ERR_NONE == err);

    // TODO: Create task to blink LED1
    // Create the BLINKY task
    OSTaskCreate(&StartupTask_TCB,
                 "Startup",
                 (OS_TASK_PTR ) startup_task,
                 0,
                 TASK_STARTUP_PRIO,
                 StartupTask_Stk,
                 (TASK_STARTUP_STK_SIZE / 10u),
                 TASK_STARTUP_STK_SIZE,
                 0u,
                 0u,
                 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);
    my_assert(OS_ERR_NONE == err);

    // TODO: Start multitasking (i.e. give control to uC/OS-III)
    OSStart(&err);
    my_assert(OS_ERR_NONE == err);
}

