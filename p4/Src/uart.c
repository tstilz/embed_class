
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



void uart_hw_config(void);

typedef union {
    uint32_t bytes;
    struct
    {
              uint32_t UE        : 1;
        const uint32_t _unused0  : 1;
              uint32_t RE        : 1;
              uint32_t TE        : 1;
        const uint32_t IDLEIE    : 1;
              uint32_t RXNEIE    : 1;
        const uint32_t TCIE      : 1;
        const uint32_t TXEIE     : 1;
        const uint32_t PEIE      : 1;
        const uint32_t PS        : 1;
              uint32_t PCE       : 1;
        const uint32_t WAKE      : 1;
        const uint32_t M0        : 1;
        const uint32_t MME       : 1;
        const uint32_t CMIE      : 1;
              uint32_t OVER8     : 1;
        const uint32_t DEDT      : 5;
        const uint32_t DEAT      : 5;
        const uint32_t RTOIE     : 1;
        const uint32_t EOBIE     : 1;
              uint32_t M1        : 1;
        const uint32_t _unused1  : 3;
    } bits;
} usart_CR1_t;

typedef union {
    uint32_t bytes;
    struct
    {
        const uint32_t _unused0 : 4;
        const uint32_t ADDM7    : 1;
        const uint32_t LBDL     : 1;
        const uint32_t LBDIE    : 1;
        const uint32_t _unused1 : 1;
        const uint32_t LBCL     : 1;
        const uint32_t CPHA     : 1;
        const uint32_t CPOL     : 1;
        const uint32_t CLKEN    : 1;
        const uint32_t STOP     : 2;
        const uint32_t LINEN    : 1;
        const uint32_t SWAP     : 1;
        const uint32_t RXINV    : 1;
        const uint32_t TXINV    : 1;
        const uint32_t DATAINV  : 1;
        const uint32_t MSBFIRST : 1;
              uint32_t ABREN    : 1;
        const uint32_t ABRMOD0  : 1;
        const uint32_t ABRMOD1  : 1;
        const uint32_t RTOEN    : 1;
        const uint32_t ADD      : 8;
    } bits;
} usart_CR2_t;

typedef union {
    uint32_t bytes;
    struct
    {
        const uint32_t EIE      : 1;
        const uint32_t IREN     : 1;
        const uint32_t IRLP     : 1;
        const uint32_t HDSEL    : 1;
        const uint32_t NACK     : 1;
        const uint32_t SCEN     : 1;
        const uint32_t DMAR     : 1;
        const uint32_t DMAT     : 1;
        const uint32_t RTSE     : 1;
        const uint32_t CTSE     : 1;
        const uint32_t CTSIE    : 1;
        const uint32_t ONEBIT   : 1;
        const uint32_t OVRDIS   : 1;
        const uint32_t DDRE     : 1;
        const uint32_t DEM      : 1;
        const uint32_t DEP      : 1;
        const uint32_t _unused0 : 1;
        const uint32_t SCARCNT  : 3;
        const uint32_t _unused1 : 12;
    } bits;
} usart_CR3_t;

typedef union {
    uint32_t bytes;
    struct
    {
        const uint32_t PE        : 1;
        const uint32_t FE        : 1;
        const uint32_t NF        : 1;
              uint32_t ORE       : 1;
        const uint32_t IDLE      : 1;
              uint32_t RXNE      : 1;
        const uint32_t TC        : 1;
              uint32_t TXE       : 1;
        const uint32_t LBDF      : 1;
        const uint32_t CTSIF     : 1;
        const uint32_t CTS       : 1;
        const uint32_t RTOF      : 1;
        const uint32_t EOBF      : 1;
        const uint32_t _unused0  : 1;
        const uint32_t ABRE      : 1;
        const uint32_t ABRF      : 1;
        const uint32_t BUSY      : 1;
        const uint32_t CMF       : 1;
        const uint32_t SBKF      : 1;
        const uint32_t RWU       : 1;
        const uint32_t _unused1  : 1;
        const uint32_t TEACK     : 1;
        const uint32_t _unused2  : 10;
    } bits;
} usart_ISR_t;


typedef struct
{
          uint32_t BRR     : 16;
    const uint32_t _unused : 16;
} usart_BRR_t;

typedef struct
{
          uint32_t RDR     : 16;
    const uint32_t _unused : 16;
} usart_RDR_t;

typedef struct
{
          uint32_t TDR     : 16;
    const uint32_t _unused : 16;
} usart_TDR_t;

typedef struct
{
          usart_CR1_t   CR1;         // offset: 0x00
          usart_CR2_t   CR2;         // offset: 0x04
          usart_CR3_t   CR3;         // offset: 0x08
          usart_BRR_t   BRR;         // offset: 0x0C
    const uint32_t      _unused0[3]; // offset: 0x00
          usart_ISR_t   ISR;         // offset: 0x1C
    const uint32_t      _unused1[1]; // offset: 0x00
          usart_RDR_t   RDR;         // offset: 0x24
          usart_TDR_t   TDR;         // offset: 0x28
} USART_T;


// Step 2a: Declare a pointer to the ADC3 Register Struct
// See processor Reference Manual:
//    Section 2.2.2, "Memory Map and Register Boundary Addresses"
//    Section 15.13.18, Table 97, "ADC Global Register Map"
#define USART_BASE    (0x40011000UL)

#define USART1_OFFSET (0x000UL)
#define USART1_ADDR   ((USART_BASE) + (USART1_OFFSET))
//static  USART_T volatile * const p_USART1 = (USART_T *) (USART1_ADDR);
#define USART1        (*p_USART1)

#define USART6_OFFSET (0x400UL)
#define USART6_ADDR   ((USART_BASE) + (USART6_OFFSET))
static  USART_T volatile * const p_USART6 = (USART_T *) (USART6_ADDR);
#define USART6        (*p_USART6)
#define USART6_IRQ   0x47

#define UART    USART6


// Step 2b: Declare a pointer to the USART Common Registers Struct
// See processor Reference Manual:
//    Section 15.13.18, Table 97, "USART Global Register Map"

// CR1, CR2, CR2, and BRR for control and configuration;
// ISR for status
// RDR and TDR for reading/writing USART data



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
  UART.CR1.bytes = 0x00000000;   // Also resets ISR
  UART.CR2.bytes = 0x00000000;   // Also resets ISR
  UART.CR3.bytes = 0x00000000;   // Also resets ISR

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
  uart_reset();
  uart_hw_config();

  // TODO: Fill in rest (configure / enable USART6)
  UART.CR1.bits.UE     = 0x0;  // Disable USART

  UART.CR1.bits.M1     = 0x00; // M[1:0] = 00: 1 Start bit, 8 data bits, n stop bits
  UART.CR1.bits.PCE    = 0x00; // Disable Parity
  UART.CR1.bits.OVER8  = 0x00; // Oversampling 16
  UART.CR1.bits.RXNEIE  = 0x01; // RX interrupts

  UART.CR2.bits.ABREN  = 0x00; // Auto baud rate disabled

  UART.BRR.BRR         = 0x57E4; // 9600 Baud with 8 bit oversampling  for 215Mhz

  UART.CR1.bits.RE = 0x01;  // Enable USART RX
  UART.CR1.bits.TE = 0x01;  // Enable USART TX
  UART.CR1.bits.UE = 0x01;  // Enable USART

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
    //BSP_LED_Toggle(LED2);
    if (UART.ISR.bits.RXNE == 0x1)
    {
        int out = port_get_char();
        rx_byte_read = (uint8_t) (out & 0x00FF);
        BSP_LED_Toggle(LED2);
    }
}


// *******************************************************************
/*!
 * @brief Write a character to serial port, space permitting.
 */
int port_put_char(char c)
{
  // TODO
  UART.TDR.TDR     = c;     // Write 1 byte to UART TX register
  return 0;
}

// *******************************************************************
/*!
 * @brief Write a string to serial port, space permitting.
 */
int port_put_string(char const *p_str)
{
  // TODO
  return 0;
}

// *******************************************************************
/*!
 * @brief Get a char from serial port
 */
int port_get_char(void)
{
  // TODO
  uint8_t output = 0;

  output = UART.RDR.RDR;     // Write 1 byte to UART TX register
  return output;
}

