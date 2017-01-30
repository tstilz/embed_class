
/** \file pot.c
 *
 * @brief Memory-Mapped UART Driver
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
 * All rights reserved.
 */

#include "project.h"
#include "hw.h"
#include "bsp.h"
#include "uart.h"

// *******************************************************************
/*!
 * @brief Configure USART hardware
 */
void uart_hw_config(void)
{
  volatile uint32_t temp;

  // RCC: Reset USART6, then bring it out of reset
  REG32(0x40023824UL) |= (1UL << 5UL);
  REG32(0x40023824UL) &= ~(1UL << 5UL);

  // Disable USART6 interrupt at NVIC
  REG32(0xE000E180UL + ((USART6_IRQ >> 5UL) << 2UL)) |= (1UL << (USART6_IRQ & 0x1FUL));

  // Configure the NVIC for USART6 interrupts.
  // See Table 43, p. 288 of reference manual for vector slot
  const uint32_t USART6_Int_Slot = USART6_IRQ;
  // Set priority of USART6 interrupt
  REG8(0xE000E400UL + USART6_Int_Slot) = 0;

  // **********************************************************************************
  // TODO: Move this (enable interrupts) to your code
  // Enable USART6 interrupt at NVIC.
  // All interrupts within the peripheral (per CR1/2/3) are still off.
  REG32(0xE000E100UL + ((USART6_Int_Slot >> 5UL) << 2)) |= (1UL << (USART6_Int_Slot & 0x1FUL));
  // **********************************************************************************

  // -1- Enable clocks
  // Be sure to perform a dummy read to delay after enabling clock to peripheral.

  // USART6 TX:  GPIO-C.6
  // USART6 RX:  GPIO-C.7
  // GPIO Port C clock enable
  REG32(0x40023830UL) |= (1UL << 2UL);
  temp = REG32(0x40023830UL) & (1UL << 2UL);  // dummy read

  // USART6 clock enable
  REG32(0x40023844UL) |= (1UL << 5UL);
  temp = REG32(0x40023844UL) & (1UL << 5UL);  // dummy read

  // -2- Configure GPIO pins for USART6 TX/RX function
  BSP_UartPin_init();

}

// *******************************************************************
/*!
 * @brief Reset the USART
 */
static void uart_reset(void)
{
  UART.CR1 = 0;   // Also resets ISR
  UART.CR2 = 0;
  UART.CR3 = 0;

  // RCC: Reset USART6, then bring it out of reset
  REG32(0x40023824UL) |= (1UL << 5UL);
  REG32(0x40023824UL) &= ~(1UL << 5UL);

  // Disable USART6 interrupt at NVIC
  REG32(0xE000E180UL + ((USART6_IRQ >> 5UL) << 2UL)) |= (1UL << (USART6_IRQ & 0x1FUL));

  // Configure the NVIC for USART6 interrupts.
  // See Table 43, p. 288 of reference manual for vector slot
  const uint32_t USART6_Int_Slot = USART6_IRQ;
  // Set priority of USART6 interrupt
  REG8(0xE000E400UL + USART6_Int_Slot) = 0;

  // Enable USART6 interrupt at NVIC.
  // All interrupts within the peripheral (per CR1/2/3) are still off.
  REG32(0xE000E100UL + ((USART6_Int_Slot >> 5UL) << 2)) |= (1UL << (USART6_Int_Slot & 0x1FUL));

}


// *******************************************************************
/*!
 * @brief Initialize USART
 */
void port_init(void)
{
  // Configure ports / pins / clocks / interrupt controller...
  uart_hw_config();

  // TODO: Fill in rest (configure / enable USART6)

}


// *******************************************************************
/*!
 * @brief USART6 Interrupt Service Routine (ISR)
 *               Installed directly in vector table.
 *               Overrides weak definition of USART6_IRQHandler() in startup_stm32f746xx.s in EWARM folder.
 */
extern void USART6_IRQHandler(void);
void USART6_IRQHandler(void)
{
  // TODO: Write interrupt handler here
}


// *******************************************************************
/*!
 * @brief Write a character to serial port, space permitting.
 */
int port_put_char(char c)
{
  // TODO
}

// *******************************************************************
/*!
 * @brief Write a string to serial port, space permitting.
 */
int port_put_string(char const *p_str)
{
  // TODO
}

// *******************************************************************
/*!
 * @brief Get a char from serial port
 */
int port_get_char(void)
{
  // TODO
}

