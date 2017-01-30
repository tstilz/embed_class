
/** \file system_stm32f7xx.c
*
* @brief Low-level, pre-main() initialization
*
* @par
* COPYRIGHT NOTICE: (c) Barr Group, LLC.
* All rights reserved.
*/


#include "project.h"
#include "hw.h"

// Represents the system (core) CPU speed
uint32_t SystemCoreClock = 16000000;

// Base address of : (up to 1 MB) embedded FLASH memory accessible over AXI
#define FLASHAXI_BASE      0x08000000UL
#define FLASH_BASE         FLASHAXI_BASE

// Function prototype to placate compiler
extern void SystemInit(void);

// ***********************************************************************************
// *  Configure / program the MCU's low-level settings.
// *  The majority of this is from the Reset / Clock Control peripheral.
// *  This includes the embedded flash interface, the PLL, disabling interrupts
// *  and possibly updating the SystemCoreClock variable.
// ***********************************************************************************
void SystemInit(void)
{
  // FPU settings
  // CP10 and CP11 : Full Access
  // SCB->CPACR
  REG32(0xE000ED88UL) |= ((3UL << 10*2)|(3UL << 11*2));

  // Reset the RCC clock configuration to the default reset state
  // RCC->CR[HSION]
  REG32(0x40023800UL) |= (uint32_t)0x00000001UL;

  // Reset CFGR register
  // RCC->CFGR
  REG32(0x40023808UL) = 0x00000000UL;

  // Reset HSEON, CSSON and PLLON bits
  // RCC->CR
  REG32(0x40023800UL) &= (uint32_t)0xFEF6FFFFUL;

  // Reset PLLCFGR register
  // RCC->PLLCFGR
  REG32(0x40023804UL) = 0x24003010UL;

  // Reset HSEBYP bit
  // RCC->CR
  REG32(0x40023800UL) &= (uint32_t)0xFFFBFFFFUL;

  // Disable all interrupts
  // RCC->CIR
  REG32(0x4002380CUL) = 0x00000000UL;

  // Configure the Vector Table location add offset address
  // Configure at base of flash for now.
  // SCB->VTOR
  REG32(0xE000ED08UL) = FLASH_BASE;
}



