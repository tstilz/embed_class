
/** \file pot.c
*
* @brief Memory-Mapped Potentiometer Driver
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/

#include "project.h"
#include "hw.h"
#include "bsp.h"
#include "pot.h"

// *******************************************************************

/*!
* @brief Configure hardware for potentiometer ADC channel
*/
static void ADC_Hardware_Config(void)
{
  volatile uint32_t temp;

  // -1- Enable clocks to ADC3 peripheral and GPIO Port for its pin
  // Be sure to perform a dummy read to delay after enabling clock to peripheral.

  // ADC3 clock enable
  REG32(0x40023844UL) |= (1UL << 10UL);
  temp = REG32(0x40023844UL) & (1UL << 10UL);  // dummy read

  // GPIO Port F clock enable
  REG32(0x40023830UL) |= (1UL << 5UL);
  temp = REG32(0x40023830UL) & (1UL << 5UL);  // dummy read

  // -2- Configure GPIO pin PF.10 for ADC3 peripheral function
  BSP_AdcPin_init();

}

// *******************************************************************
static void pot_init_pt1(void)
{
  // Configure ADC pin hardware and interrupts
  ADC_Hardware_Config();

  // TODO: ADC CCR
  // ADC Clock prescaler - PCLK2 divided by 4

  // TODO: ADC Scan Mode - disabled

  // TODO: ADC Resolution - 12 bits

  // TODO: ADC Data Alignment  - right-aligned

  // TODO: External trigger conversion start - disabled

  // TODO: External trigger polarity setting - N/A (just clear it)

  // TODO: Disable ADC Continuous Conversion Mode 

  // TODO: Disable ADC regular discontinuous mode

  // TODO: Set number of conversions (1)

  // TODO: DMA Continuous requests - disabled

  // TODO: ADC End of Conversion (EOC) selection (clear)

}

// *******************************************************************
static void pot_init_pt2(void)
{
  // TODO: Set ADC sample time to 3 cycles

  // TODO: Set ADC_SQR3 bits for Rank 1
}

// *******************************************************************
static void pot_init_pt3(void)
{
  volatile uint32_t counter = 0;
  const    uint32_t sysclock_speed = SystemCoreClock;

  // TODO:
  // Enable ADC peripheral

  // After the ADC is enabled, we must wait a short period
  // for the ADC peripheral to stabilize.
  // tSTAB = 1usec per device datasheet, let's give it 3uS.
  counter = (3UL * (sysclock_speed / (1000UL * 1000UL)));
  while (counter--)
  {
    // just decrement and kill some time
  }

}

// *******************************************************************
/*!
* @brief Initialize potentiometer's ADC channel
*/
void pot_init(void)
{
  // -1- Configure the ADC peripheral
  pot_init_pt1();

  // -2- Configure specific ADC channel
  pot_init_pt2();

  //  -3- Power up the ADC and allow it to stabilize
  pot_init_pt3();
}

