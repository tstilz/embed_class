
/** \file setup.c
*
* @brief Hardware / platform set up functionality
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/

#include "project.h"
#include "hw.h"
#include "setup.h"


// *****************************************************************************************
// ** Private (local) functions **
// *****************************************************************************************


static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
static void Platform_Init(void);

static void mpu_enable(void);
static void mpu_disable(void);
static void mpu_config(void);
static void enable_instruction_cache(void);
static void enable_data_cache(void);


// Global variable representing the number of 1ms ticks since power on.
// This will last approximately 4 million seconds, which is fine for all
// exercises in the course without worrying about overflow / wrap-around.
static volatile uint32_t tick_count = 0;

// *****************************************************************************************
/*!
 * @brief Perform low-level processor initialization
 */
void LowLevelInit(void)
{
  // Configure the Memory Protection Unit (MPU) attributes
  MPU_Config();

  // Enable the STM32F7 Caches (instruction and data)
  CPU_CACHE_Enable();

  // Low-level platform init
  Platform_Init();

  // Boost the system clock to 216 MHz
  SystemClock_Config();
}

// *****************************************************************************************
/*!
 * @brief Called from ISR to update variable that tracks elapsed 1ms ticks
 */
void Systick_IncTick(void)
{
  ++tick_count;
}

// *****************************************************************************************
/*!
 * @brief Returns variable that tracks 1ms ticks
 */
static inline uint32_t Systick_GetTick(void)
{
  return tick_count;
}

// *****************************************************************************************
/*!
 * @brief Delay for some number of milliseconds, by wasting CPU cycles.
 */
void busywait_ms (uint32_t delay_in_ms)
{
  uint32_t start_time = Systick_GetTick();
  while((Systick_GetTick() - start_time) < delay_in_ms)
  {
  }
}

// *****************************************************************************************
/*!
 * @brief Configure the MPU and enable it
 */
static void MPU_Config(void)
{
  
  // Disable MPU
  mpu_disable();

  // Configure MPU attributes - Write-through for SRAM
  mpu_config();

  // Enable MPU
  mpu_enable();
}

// *****************************************************************************************
/*!
 * @brief Enable CPU L1 I & D caches
 */
static void CPU_CACHE_Enable(void)
{
  // Enable I-Cache
  enable_instruction_cache();

  // Enable D-Cache
  enable_data_cache();
}

// *****************************************************************************************
/*!
 * @brief Disable MPU
 */
static void mpu_disable(void)
{
  // Flush / sync
  __DMB();

  // Disable fault exceptions at MPU
  // SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
  REG32(0xE000ED24) &= (1u << 16);
  
  // Disable MPU
  // MPU->CTRL = 0;
  REG32(0xE000ED94) = 0;
}

// *****************************************************************************************
/*!
 * @brief Enable MPU
 */
static void mpu_enable(void)
{
  // Enable MPU
  // MPU->CTRL = MPU_Control | MPU_CTRL_ENABLE_Msk;
  REG32(0xE000ED94) = 5;

  // Enable fault exceptions at MPU
  // SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
  REG32(0xE000ED24) |= (1u << 16);
  
  // Flush / sync
  __DMB();
  __ISB();

}

/*!
 * @brief Program/load the MPU settings
 */
static void mpu_config(void)
{
  // Set MPU region number
  // MPU->RNR = 0
  REG32(0xE000ED98) = 0;

  // Base address - SRAM
  // MPU->RBAR = <base>
  REG32(0xE000ED9C) = 0x20010000;
  
  // Region attributes
  // MPU->RASR = <attributes / config>
  REG32(0xE000EDA0) = 0x03020023;
}

// *****************************************************************************************
/*!
 * @brief Enable I-cache
 */
static void enable_instruction_cache(void)
{
    __DSB();
    __ISB();

    // Invalidate I-cache
    // SCB->ICIALLU = 0UL;
  REG32(0xE000EF50) = 0;

    // Enable I-cache
    // SCB->CCR |=  (uint32_t)SCB_CCR_IC_Msk;
  REG32(0xE000ED14) |= (1u << 17);

    __DSB();
    __ISB();
}

// *****************************************************************************************
/*!
 * @brief Enable data cache.
 */
static void enable_data_cache(void)
{
  // Level 1 cache
  REG32(0xE000ED84) = 0;
  __DSB();

  // ccsidr = SCB->CCSIDR = 0xF003E019
  uint32_t ccsidr = REG32(0xE000ED80);
  uint32_t sets = 0x1F;
  do
  {
    uint32_t ways = 3;
    do
    {
      REG32(0xE000EF60) = (((sets <<  5UL) & (0x1FFUL <<  5UL)) |
                           ((ways << 30UL) & (    3UL << 30UL)));
    } while (ways--);
  } while (sets--);

  __DSB();

  // ENABLE D-CACHE
  // SCB->CCR |=  (uint32_t)SCB_CCR_DC_Msk;
  REG32(0xE000ED14) |= (1UL << 16);

  // FLUSH / SYNC
  __DSB();
  __ISB();

}


// *****************************************************************************************
/*!
 * @brief Enable flash acceleration.
 */
static void flash_enable_accel(void)
{
  REG32(0x40023C00) |= (1UL << 9);
}

// *****************************************************************************************
/*!
 * @brief Configure NVIC priority Group
 * @param[in] prio NVIC Priority Group
 * @return int8_t
 */
static void nvic_set_priority_grouping(uint32_t prio)
{
  prio &= 0x07UL;
  volatile uint32_t temp = REG32(0xE000ED0CUL);
  temp &= 0xF8FFUL;
  temp = ((temp) | (0x5FAUL << 16UL) | (prio << 8UL));
  REG32(0xE000ED0CUL) = temp;
}

// *****************************************************************************************
/*!
 * @brief  Configure Cortex-M SYSTICK timer
 * @param  param[in] ticks number of sysclock ticks to count each cycle
 */
static void systicker_config(uint32_t ticks)
{
  // SYSTICK limit is 24 bits
  if (ticks > ((1UL << 24) - 1UL))
  {
    ticks = ((1UL << 24) - 1UL);
  }

  REG32(0xE000E014UL) = (ticks - 1);  // SYSTICK LOAD (period)

  // Set SYSTICK interrupt prio to lowest (15) (can always be changed later at runtime)
  // SCB->SHPR[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  // REG8((0xE000ED27UL - 4UL)) = (0x0FUL << 4UL);
  REG8(0xE000ED23UL) = (0x0FUL << 4UL);   // Bits 24-31 of SHPR[3]

  // Clear systick count / value
  REG32(0xE000E018UL) = 0;

  // Start systick
  REG32(0xE000E010UL) = 0x07UL;
}

// *****************************************************************************************
/*!
 * @brief Configure Cortex-M SYSTICK interrupt priority
 * @param[in] prio The SYSTICK interrupt priority
 */
static void systicker_set_prio(uint32_t prio)
{
  uint32_t prio_group = (((REG32(0xE000ED0CUL)) & (0x07UL << 8UL)) >> 8UL);

  uint32_t ppb        = 7UL - prio_group;
  // assert(ppb == 4);

  uint32_t spb        = ((ppb + 3) - 7UL);
  // assert(spb == 0);

  uint32_t enc        = ((15UL & ((1UL << ppb) - 1UL) << spb) |
                         ( 0UL & ((1UL << spb) - 1UL)));
  // assert(enc == 15);

  REG8(0xE000ED23UL)  = (enc << 4UL);   // Bits 24-31 of SHPR[3]
}

// *****************************************************************************************
/*!
* @brief Initialize Cortex-M SYSTICK timer
* @param[in] prio  The SYSTICK interrupt priority
* @param[in] num2  The System Clock Frequency
*/
static void ticker_init(uint32_t prio, uint32_t SysClockFreq)
{
  systicker_config(SysClockFreq / 1000UL);
  systicker_set_prio(prio);
}

// *****************************************************************************************
/*!
* @brief Initialize hardware platform
*/
static void Platform_Init(void)
{
  // Enable flash acceleration
  flash_enable_accel();

  // Set Interrupt Group Priority
  nvic_set_priority_grouping(3);

  // Use systick as source for our time base.
  // Configure 1ms tick (otherwise, default clock after Reset is HSI)
  ticker_init(SYSTICK_INT_PRIORITY, (16UL * 1000UL * 1000UL));
  
}

// *****************************************************************************************
/*!
 * @brief  Configure RCC Oscillators
 */
static void RCC_osc_config(void)
{
 
    volatile uint32_t temp;

    // **************************
    // HSE Configuration
    // **************************
  
    // Program the new HSE configuration, and then 
    // wait for HSE to become ready.
    REG32(0x40023800UL) |= (1UL << 16);
    while((REG32(0x40023800UL) & (1UL << 17UL)) == 0)
    {
    }

    // **************************
    // PLL Configuration
    // **************************

    // Confirm that the PLL is not used as the system clock
    my_assert((REG32(0x40023808UL) & 0x0CUL) != 0x08UL);

    // Disable the main PLL and wait for it to become reset
    REG32(0x40023800UL) &= ~(1UL << 24UL);
    while((REG32(0x40023800UL) & (1UL << 25UL)) != 0)
    {
    }

    // Configure the main PLL clock source, multiplication and division factors.
    REG32(0x40023804UL) = 0x29406C19UL;

    // Enable the main PLL
    REG32(0x40023800UL) |= (1UL << 24UL);

    // Wait for the PLL to become ready
    while((REG32(0x40023800UL) & (1UL << 25UL)) == 0)
    {
    }
}

// *****************************************************************************************
/*!
 * @brief  Turn on CPU overdrive
 */
static void pwrex_enable_overDrive(void)
{
  volatile uint32_t temp;
  //__HAL_RCC_PWR_CLK_ENABLE();
  REG32(0x40023840UL) |= (1UL << 28UL);
  temp = (REG32(0x40023840) & (1UL << 28));

  // Enable overdrive to switch clock to 216MHz
  //__HAL_PWR_OVERDRIVE_ENABLE();
  REG32(0x40007000UL) |= (1UL << 16);

  // Wait for ready
  while (!REG32(0x40007004UL) & (1UL << 16))
  {
  }

  // __HAL_PWR_OVERDRIVESWITCHING_ENABLE();
  REG32(0x40007000UL) |= (1UL << 17);

  // Wait for ready
  while (!REG32(0x40007004UL) & (1UL << 17))
  {
  }

}

// *****************************************************************************************
/*!
 * @brief  Configure RCC clocks
 */
static void RCC_clock_config(void)
{
  // Considering increased CPU frequency, we must program a larger
  // number of wait states to FLASH_ACR[LATENCY]
  // __HAL_FLASH_SET_LATENCY(FLatency);
  volatile uint32_t temp;
  temp = REG32(0x40023C00UL);
  temp &= ~(0x0FUL);
  temp |= (0x07UL);
  REG32(0x40023C00UL) = temp;
  my_assert((REG32(0x40023C00UL) & 0x0FUL) == 0x07UL);

  // HCLK Configuration
  temp = REG32(0x40023808UL);
  temp &= ~(0xF0UL);
  temp |=  (0x00UL);
  REG32(0x40023808UL) = temp;
  
  // SYSCLK Configuration
  temp = REG32(0x40023808UL);
  temp >>= 2;
  temp <<= 2;
  temp  |= 0x02UL;
  REG32(0x40023808UL) = temp;

  // Wait for SYSCLK source (PLL)
  while ((REG32(0x40023808UL) & 0x0CUL) != 0x08UL)
  {
  }

  // PCLK1 Configuration
  temp = REG32(0x40023808UL);
  temp &= ~(0x1C00UL);
  temp |=  (0x1400UL);
  REG32(0x40023808UL) = temp;
  
  // PCLK2 Configuration
  temp = REG32(0x40023808UL);
  temp &= ~(0xE000UL);
  temp |=  (0x01UL << 15UL);
  REG32(0x40023808UL) = temp;

  // Update the SystemCoreClock global variable
extern uint32_t SystemCoreClock;
  SystemCoreClock = (216UL * 1000UL * 1000UL);
  
  // Re-configure SYSTICK in light of new system clocks settings
  ticker_init(SYSTICK_INT_PRIORITY, SystemCoreClock);
  
}

// *****************************************************************************************
/*!
 * @brief  Configure the system (CPU) clock
 */
static void SystemClock_Config(void)
{
  // *****************************************************************
  // *         The system Clock is configured as follows : 
  // *            System Clock source            = PLL (HSE)
  // *            SYSCLK(Hz)                     = 216000000
  // *            HCLK(Hz)                       = 216000000
  // *            AHB Prescaler                  = 1
  // *            APB1 Prescaler                 = 4
  // *            APB2 Prescaler                 = 2
  // *            HSE Frequency(Hz)              = 25000000
  // *            PLL_M                          = 25
  // *            PLL_N                          = 432
  // *            PLL_P                          = 2
  // *            PLL_Q                          = 9
  // *            VDD(V)                         = 3.3
  // *            Main regulator output voltage  = Scale1 mode
  // *            Flash Latency(WS)              = 7

  // Part 1 - RCC oscillator
  RCC_osc_config();

  // Part 2 - Enable Clock overdrive
  pwrex_enable_overDrive();

  // Part 3 - RCC Clock
  RCC_clock_config();
}


/*!
 * @brief Called when a home-made assertion (my_assert()) fails.
 */
// *****************************************************************
void assert_failed(void)
{ 
  // DEBUG: Set breakpoint here; call stack shows history/trace
  // RELEASE: Log, then reset
  while (1)
  {
    //busywait_ms(250UL);
    //BSP_LED_Toggle(LED1);
    //BSP_LED_Toggle(LED2);
  }
}

