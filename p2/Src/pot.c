
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

// Step 1a: Define a Register-Matching Struct for ADC3 peripheral
// See processor Reference Manual:
//    Table 98 in Section 15.13.18, "ADC Register Map"

typedef struct
{
    const uint32_t AWD   : 1;
          uint32_t EOC   : 1;
    const uint32_t JEOC  : 1;
    const uint32_t JSTRT : 1;
    const uint32_t STRT  : 1;
    const uint32_t OVR   : 1;
} adc_SR_t;


typedef struct
{
    const uint32_t AWDCH   : 5;
    const uint32_t EOCIE   : 1;
    const uint32_t AWDIE   : 1;
    const uint32_t JEOCIE  : 1;
          uint32_t SCAN    : 1;
    const uint32_t AWD_SGL : 1;
    const uint32_t JAUTO   : 1;
          uint32_t DISCEN  : 1;
    const uint32_t JDISCEN : 1;
    const uint32_t NUM     : 3;
    const uint32_t DISC    : 1;
    const uint32_t _unused : 6;
    const uint32_t JAWDEN  : 1;
    const uint32_t AWDEN   : 1;
          uint32_t RES     : 2;
    const uint32_t OVRIE   : 1;
} adc_CR1_t;

typedef struct
{
          uint32_t ADON     : 1;
          uint32_t CONT     : 1;
    const uint32_t _unused0 : 6;
    const uint32_t DMA      : 1;
          uint32_t DDS      : 1;
    const uint32_t EOCS     : 1;
          uint32_t ALIGN    : 1;
    const uint32_t _unused1 : 4;
    const uint32_t JEXTSEL  : 4;
    const uint32_t JEXTEN   : 2;
    const uint32_t JSWSTART : 1;
    const uint32_t _unused2 : 1;
    const uint32_t EXTSEL   : 4;
          uint32_t EXTEN    : 2;
          uint32_t SWSTART  : 1;
    const uint32_t _unused3 : 1;

} adc_CR2_t;

typedef struct
{
    const uint32_t SQ13     : 5;
    const uint32_t SQ14     : 5;
    const uint32_t SQ15     : 5;
    const uint32_t SQ16     : 5;
          uint32_t L        : 4;
    const uint32_t _unused0 : 8;
} adc_SQR1_t;


typedef struct
{
    uint32_t SQ1      : 5;
    uint32_t SQ2      : 5;
    uint32_t SQ3      : 5;
    uint32_t SQ4      : 5;
    uint32_t SQ5      : 5;
    uint32_t SQ6      : 5;
    uint32_t _unused0 : 2;
} adc_SQR3_t;

typedef struct
{
          uint32_t SMP0     : 3;
    const uint32_t SMP1     : 3;
    const uint32_t SMP2     : 3;
    const uint32_t SMP3     : 3;
    const uint32_t SMP4     : 3;
    const uint32_t SMP6     : 3;
    const uint32_t SMP7     : 3;
          uint32_t SMP8     : 3;
    const uint32_t SMP9     : 3;
    const uint32_t _unused0 : 2;
} adc_SMPR2_t;

// typedef struct __attribute((packed))
// {
//      unsigned _unused : 32;
// } ADC_CSR_bit_t;


typedef struct
{
          uint32_t DATA    : 16;
    const uint32_t _unused : 16;
} adc_DR_t;

typedef struct
{
          adc_SR_t    SR;     // offset: 0x00
          adc_CR1_t   CR1;    // offset: 0x04
          adc_CR2_t   CR2;    // offset: 0x08
    const uint32_t    SMPR1;  // offset: 0x0C
          adc_SMPR2_t SMPR2;  // offset: 0x10
    const uint32_t    JOFR1;  // offset: 0x14
    const uint32_t    JOFR2;  // offset: 0x18
    const uint32_t    JOFR3;  // offset: 0x1C
    const uint32_t    JOFR4;  // offset: 0x20
    const uint32_t    HTR;    // offset: 0x24
    const uint32_t    LTR;    // offset: 0x28
          adc_SQR1_t  SQR1;   // offset: 0x2C
    const uint32_t    SQR2;   // offset: 0x30
          // adc_SQR3_t  SQR3;   // offset: 0x34
          uint32_t    SQR3;   // offset: 0x34
    const uint32_t    JSQR;   // offset: 0x38
    const uint32_t    JDR1;   // offset: 0x3C
    const uint32_t    JDR2;   // offset: 0x40
    const uint32_t    JDR3;   // offset: 0x44
    const uint32_t    JDR4;   // offset: 0x48
          adc_DR_t    DR;     // offset: 0x4C
} ADC_T;


typedef struct
{
    const uint32_t MULTI    : 5;
    const uint32_t _unused2 : 3;
    const uint32_t DELAY    : 4;
    const uint32_t _unused1 : 1;
          uint32_t DDS      : 1;
    const uint32_t DMA      : 2;
          uint32_t ADCPRE   : 2;
    const uint32_t _unused0 : 4;
    const uint32_t VBATE    : 1;
    const uint32_t TSVREFE  : 1;
} CCR_T;

// Step 1b: Define a Register-Matching Struct for ADC Common registers.
// See processor Reference Manual:
//    Table 99 ("Common ADC Registers) in Section 15.13.18
//    Offset - ADC base + 0x300
typedef struct
{
  uint32_t CSR;    // Offset 0x00
  CCR_T CCR;       // Offset 0x04
  uint32_t CDR;    // Offset 0x08
} ADC_COMMON_T;

// Step 2a: Declare a pointer to the ADC3 Register Struct
// See processor Reference Manual:
//    Section 2.2.2, "Memory Map and Register Boundary Addresses"
//    Section 15.13.18, Table 97, "ADC Global Register Map"
#define ADC_BASE    (0x40012000UL)
#define ADC3_OFFSET (0x200UL)
#define ADC3_ADDR   ((ADC_BASE) + (ADC3_OFFSET))
static  ADC_T volatile * const p_ADC3 = (ADC_T *) (ADC3_ADDR);
#define ADC3        (*p_ADC3)

// Step 2b: Declare a pointer to the ADC Common Registers Struct
// See processor Reference Manual:
//    Section 15.13.18, Table 97, "ADC Global Register Map"
#define ADC_COMMON_OFFSET (0x300UL)
#define ADC_COMMON_ADDR   ((ADC_BASE) + (ADC_COMMON_OFFSET))
static ADC_COMMON_T volatile * const p_ADC_COMMON = (ADC_COMMON_T *) (ADC_COMMON_ADDR);
#define ADC_COMMON        (*p_ADC_COMMON)


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
  // ADC_COMMON.CCR &= ~(0x30000UL);
  // ADC_COMMON.CCR |=  (0x10000UL);
  ADC_COMMON.CCR.ADCPRE = 0x01;

  // ADC Scan Mode - disabled
  // ADC3.CR1 &= ~(0x100UL);
  ADC3.CR1.SCAN = 0x0U;

  // ADC Resolution - 12 bits
  // ADC3.CR1 &= ~(0x03000000UL);
  ADC3.CR1.SCAN = 0x00U;

  // ADC Data Alignment  - right-aligned
  // ADC3.CR2 &= ~(1UL << 11UL);
  ADC3.CR2.ALIGN = 0x0U;

  // External trigger conversion start - disabled
  ADC3.CR2.EXTEN = 0x0;

  // External trigger polarity setting - N/A
  // ADC3.CR2 &= ~(0x30000000UL);

  // Disable ADC Continuous Conversion Mode
  ADC3.CR2.CONT = 0x0;

  // Disable ADC regular discontinuous mode
  ADC3.CR1.DISCEN = 0x0;

  // Set number of conversions (1)
  ADC3.SQR1.L = 0x1;

  // DMA Continuous requests - disabled
  ADC3.CR2.DDS = 0x0;

  // ADC End of Conversion (EOC) selection
  // EOC bit is set at the end of each sequence of regular conversions.
  ADC3.SR.EOC = 0;

  /*
  // Configure ADC pin hardware and interrupts
  ADC_Hardware_Config();

  uint32_t read_reg = ADCC->ADC_CCR.ADCPRE;
  // TODO: ADC CCR
  // ADC Clock prescaler - PCLK2 divided by 4
  ADCC->ADC_CCR.ADCPRE = 0x01; // Prescaler set to 0x01 page 453
  read_reg = ADCC->ADC_CCR.ADCPRE;

  // TODO: ADC Scan Mode - disabled
  ADC3->ADC_CR1.SCAN = 0x01; // Prescaler set to 0x01 page 453

  // TODO: ADC Resolution - 12 bits
  ADC3->ADC_CR1.RES = 0x00; // Resolution set to 12 bits -  page 441

  // TODO: ADC Data Alignment  - right-aligned
  ADC3->ADC_CR2.ALIGN = 0x00; // right-align is 0x0 - page 444

  // TODO: External trigger conversion start - disabled
  ADC3->ADC_CR2.ALIGN = 0x00; // disabled is 0x0 - page 444

  // TODO: External trigger polarity setting - N/A (just clear it)

  // TODO: Disable ADC Continuous Conversion Mode

  // TODO: Disable ADC regular discontinuous mode

  // TODO: Set number of conversions (1)

  // TODO: DMA Continuous requests - disabled

  // TODO: ADC End of Conversion (EOC) selection (clear)
  */
}

// *******************************************************************
static void pot_init_pt2(void)
{
  // TODO: Set ADC sample time to 3 cycles
    // Set sample time to 3 cycles
  ADC3.SMPR2.SMP8 = 0;

  // TODO: Set ADC_SQR3 bits for Rank 1

  // Set ADC_SQR3 bits for Rank 1
  //ADC3.SQR3.SQ1 = 0x8;
  ADC3.SQR3 &= ~(0x1FUL << 0UL);
  ADC3.SQR3 |=  (0x08UL << 0UL);
}

// *******************************************************************
static void pot_init_pt3(void)
{
  volatile uint32_t counter = 0;
  const    uint32_t sysclock_speed = SystemCoreClock;

  // TODO:
  // Enable ADC peripheral
  ADC3.CR2.ADON = 1;

  // Since ADC was disabled, we must wait a short period
  // for the ADC peripheral to stabilize.
  // tSTAB = 1usec per device datasheet, let's give it 3uS.
  counter = (3UL * (sysclock_speed / (1000UL * 1000UL)));
  while (counter--)
  {
    // just decrement and kill some time
  }

  // Sanity check: ensure ADC is enabled
  my_assert(ADC3.CR2.ADON == 1);
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
  my_assert(0 == (ADC3.SR.EOC));

  // Start the conversion!
   ADC3.CR2.SWSTART = 1;

  // Wait for conversion to complete
  while (0 == (ADC3.SR.EOC))
  {
    // Wait for EOC bit to become set
  }

  // Clear EOC status register flag
  ADC3.SR.EOC = 0;

  // Read ADC channel's value in converted Data Register
  retval = ADC3.DR.DATA;

  // Reduce the range from 12-bit (0 to 4,095) to 10-bit (0 to 1,023).
  return (retval >> 2);
}

