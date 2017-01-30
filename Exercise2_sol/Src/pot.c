
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

// **************************************************************************
// NOTE: We define the adc_t struct inside the .c file, not the .h file.
//       This is a form of encapsulation; we do not want to give
//       other code access to the register layout of the ADC registers.
// **************************************************************************

// Step 1a: Define a Register-Matching Struct for ADC3 peripheral
// See processor Reference Manual:
//     Table 98 in Section 15.13.18, "ADC Register Map"
typedef struct
{
  uint32_t SR;     // offset: 0x00
  uint32_t CR1;    // offset: 0x04
  uint32_t CR2;    // offset: 0x08
  uint32_t SMPR1;  // offset: 0x0C
  uint32_t SMPR2;  // offset: 0x10
  uint32_t JOFR1;  // offset: 0x14
  uint32_t JOFR2;  // offset: 0x18
  uint32_t JOFR3;  // offset: 0x1C
  uint32_t JOFR4;  // offset: 0x20
  uint32_t HTR;    // offset: 0x24
  uint32_t LTR;    // offset: 0x28
  uint32_t SQR1;   // offset: 0x2C
  uint32_t SQR2;   // offset: 0x30
  uint32_t SQR3;   // offset: 0x34
  uint32_t JSQR;   // offset: 0x38
  uint32_t JDR1;   // offset: 0x3C
  uint32_t JDR2;   // offset: 0x40
  uint32_t JDR3;   // offset: 0x44
  uint32_t JDR4;   // offset: 0x48
  uint32_t DR;     // offset: 0x4C
} ADC_T;

// Step 1b: Define a Register-Matching Struct for ADC Common registers.
// See processor Reference Manual:
//     Table 99 ("Common ADC Registers) in Section 15.13.18
//     Offset - ADC base + 0x300
typedef struct
{
  uint32_t CSR;    // Offset 0x00
  uint32_t CCR;    // Offset 0x04
  uint32_t CDR;    // Offset 0x08
} ADC_COMMON_T;

// Step 2a: Declare a pointer to the ADC3 Register Struct
// See processor Reference Manual:
//     Section 2.2.2, "Memory Map and Register Boundary Addresses"
//     Section 15.13.18, Table 97, "ADC Global Register Map"
#define ADC_BASE     (0x40012000UL)
#define ADC3_OFFSET  (0x200UL)
#define ADC3_ADDR    ((ADC_BASE) + (ADC3_OFFSET))
ADC_T volatile * const p_ADC3 = (ADC_T *) (ADC3_ADDR);
#define ADC3     (*p_ADC3)

// Step 2b: Declare a pointer to the ADC Common Registers Struct
// See processor Reference Manual:
//     Section 15.13.18, Table 97, "ADC Global Register Map"
#define ADC_COMMON_OFFSET    (0x300UL)
#define ADC_COMMON_ADDR ((ADC_BASE) + (ADC_COMMON_OFFSET))
ADC_COMMON_T volatile * const p_ADC_COMMON = (ADC_COMMON_T *) (ADC_COMMON_ADDR);
#define ADC_COMMON     (*p_ADC_COMMON)

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

  // ADC Clock prescaler - PCLK2 divided by 4
  ADC_COMMON.CCR &= ~(0x30000UL);
  ADC_COMMON.CCR |=  (0x10000UL);

  // ADC Scan Mode - disabled
  ADC3.CR1 &= ~(0x100UL);

  // ADC Resolution - 12 bits
  ADC3.CR1 &= ~(0x03000000UL);

  // ADC Data Alignment  - right-aligned
  ADC3.CR2 &= ~(1UL << 11UL);

  // External trigger conversion start - disabled
  ADC3.CR2 &= ~(0x0F000000UL);

  // External trigger polarity setting - N/A
  ADC3.CR2 &= ~(0x30000000UL);

  // Disable ADC Continuous Conversion Mode 
  ADC3.CR2 &= ~(1UL << 1UL);

  // Disable ADC regular discontinuous mode
  ADC3.CR1 &= ~(1UL << 11UL);

  // Set number of conversions (1)
  ADC3.SQR1 &= ~(0x0FUL << 20UL);
  ADC3.SQR1 |=  ((1UL - 1UL) << 20UL);

  // DMA Continuous requests - disabled
  ADC3.CR2 &= ~(1UL << 9UL);

  // ADC End of Conversion (EOC) selection
  // EOC bit is set at the end of each sequence of regular conversions.
  ADC3.CR2 &= ~(1UL << 10UL);

}

// *******************************************************************
static void pot_init_pt2(void)
{
  // Set sample time to 3 cycles
  ADC3.SMPR2 &= ~(7UL << 24UL);

  // Set ADC_SQR3 bits for Rank 1
  ADC3.SQR3 &= ~(0x1FUL << 0UL);
  ADC3.SQR3 |=  (0x08UL << 0UL);

}

// *******************************************************************
static void pot_init_pt3(void)
{
  volatile uint32_t counter = 0;
  const    uint32_t sysclock_speed = SystemCoreClock;

  // Enable ADC peripheral
  ADC3.CR2 |=  (1UL << 0UL);

  // Since ADC was disabled, we must wait a short period
  // for the ADC peripheral to stabilize.
  // tSTAB = 1usec per device datasheet, let's give it 3uS.
  counter = (3UL * (sysclock_speed / (1000UL * 1000UL)));
  while (counter--)
  {
    // just decrement and kill some time
  }

  // Sanity check: ensure ADC is enabled
  my_assert((ADC3.CR2 & (1UL << 0UL)));
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

// *******************************************************************
/*!
* @brief Return reading from potentiometer's ADC channel
* @return 10-bit unsigned ADC reading
*/
uint16_t pot_read(void)
{
  uint16_t retval;

  // Sanity check.  There should be no pending "conversion completed".
  my_assert(0 == (ADC3.SR & (1UL << 1UL)));

  // Start the conversion!
  ADC3.CR2 |=  (1UL << 30UL);

  // Wait for conversion to complete
  while (0 == (ADC3.SR & (1UL << 1UL)))
  {
    // Wait for EOC bit to become set
  }

  // Clear EOC status register flag
  ADC3.SR &= ~(1UL << 1UL);

  // Read ADC channel's value in converted Data Register
  retval = ADC3.DR;

  // Reduce the range from 12-bit (0 to 4,095) to 10-bit (0 to 1,023).
  return (retval >> 2);
}

