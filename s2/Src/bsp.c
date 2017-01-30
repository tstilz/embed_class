
/** \file bsp.c
*
* @brief Basic Board Support Functionality
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/


#include "project.h"
#include "hw.h"
#include "bsp.h"

// ENABLE CLOCK TO GPIO PORT
// Dummy read after register write is required because we need a
// delay after enabling RCC peripheral clock
#define GPIO_CLK_ENABLE(VAL) \
                                       do { \
                                        volatile uint32_t tmpreg; \
                                        SET_BIT((REG32(0x40023830)), (VAL));\
                                        tmpreg = READ_BIT((REG32(0x40023830)), (VAL));\
                                        (void)(tmpreg); \
                                      } while(0)


// Macros to enable clock for specific GPIO ports
#define GPIOA_CLK_ENABLE()   GPIO_CLK_ENABLE(1UL << 0UL)
#define GPIOF_CLK_ENABLE()   GPIO_CLK_ENABLE(1UL << 5UL)
#define GPIOI_CLK_ENABLE()   GPIO_CLK_ENABLE(1UL << 8UL)

// LED 1 - GPIO PI.1
#define LED1_GPIO_PORT_CLK_ENABLE()      GPIOI_CLK_ENABLE()
#define LED_1_PIN   (1UL)   // PI.1

// LED 2 - GPIO PA.0
#define LED2_GPIO_PORT_CLK_ENABLE()      GPIOA_CLK_ENABLE()
#define LED_2_PIN   (0UL)   // PA.0

// Test Outputs - PF.6 through PF.9
#define TESTOUT_GPIO_PORT_CLK_ENABLE()   GPIOF_CLK_ENABLE()
#define TEST_OUT_PIN6         (6UL)   // PF.6
#define TEST_OUT_PIN7         (7UL)   // PF.7
#define TEST_OUT_PIN8         (8UL)   // PF.8
#define TEST_OUT_PIN9         (9UL)   // PF.9
#define TEST_OUT_PIN_FIRST    (TEST_OUT_PIN6)
#define TEST_OUT_PIN_LAST     (TEST_OUT_PIN9)
#define TEST_OUT_PORT         (GPIO_PORT_F_BASE_ADDR)   // Port F

// Base addresses of memory-mapped GPIO port peripherals
#define  GPIO_PORT_A_BASE_ADDR    (0x40020000UL)
#define  GPIO_PORT_F_BASE_ADDR    (0x40021400UL)
#define  GPIO_PORT_I_BASE_ADDR    (0x40022000UL)

// GPIO port register offsets
#define GPIO_MODER_OFFSET  (0x00UL)
#define GPIO_OTYPER_OFFSET (0x04UL)
#define GPIO_SPEEDR_OFFSET (0x08UL)
#define GPIO_PUPDR_OFFSET  (0x0CUL)
#define GPIO_ODR_OFFSET    (0x14UL)
#define GPIO_BSRR_OFFSET   (0x18UL)

// GPIO port register addresses
#define GPIO_MODER(Port)   ((Port) + GPIO_MODER_OFFSET)
#define GPIO_OTYPER(Port)  ((Port) + GPIO_OTYPER_OFFSET)
#define GPIO_SPEEDR(Port)  ((Port) + GPIO_SPEEDR_OFFSET)
#define GPIO_PUPDR(Port)   ((Port) + GPIO_PUPDR_OFFSET)
#define GPIO_ODR(Port)     ((Port) + GPIO_ODR_OFFSET)
#define GPIO_BSRR(Port)    ((Port) + GPIO_BSRR_OFFSET)

// Function prototypes for local (hidden) functions
static void BSP_LED_Init(BOARD_LED_ID Led);
static void BSP_Test_Outputs_Init(void);

// **********************************************************
/*!
* @brief Initialize the BSP (LEDs, test outputs, etc.)
*/
void BSP_Init(void)
{
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_Test_Outputs_Init();
}

// **********************************************************
/*!
* @brief Set a GPIO port's pin to 0 or 1
* @param[in] PortBaseAddr  GPIO port base address
* @param[in] Pin  The pin number (0-15) to change
*/
static void BG_GPIO_WritePin(uint32_t PortBaseAddr, uint32_t Pin, uint32_t Val)
{
  my_assert(PortBaseAddr >= 0x40020000UL);
  my_assert(PortBaseAddr < 0x40023000UL);
  my_assert(Pin <= 15);
  Pin += (Val ? 0 : 16UL);  // Reset bits are upper 16 bits of register
  REG32(GPIO_BSRR(PortBaseAddr)) = (1UL << Pin); // Set or reset pin!
}

// **********************************************************
/*!
* @brief Toggle a GPIO output pin
* @param[in] PortBaseAddr  GPIO port base address
* @param[in] Pin  The pin number (0-15) to toggle
*/
static void BG_GPIO_TogglePin(uint32_t PortBaseAddr, uint32_t Pin)
{
  my_assert(PortBaseAddr >= 0x40020000UL);
  my_assert(PortBaseAddr < 0x40023000UL);
  my_assert(Pin <= 15);
  REG32(GPIO_ODR(PortBaseAddr)) ^= (1UL << Pin); // Toggle Pin x
}

// **********************************************************
/*!
* @brief Initialize a GPIO port's pin as a test output.
* @param[in] PortBaseAddr  GPIO port base address
* @param[in] Pin  The pin number (0-15) to configure
*/
static void BG_GPIO_Init_Output(uint32_t PortBaseAddr, uint32_t Pin)
{
  volatile uint32_t temp;

  // Param checking
  my_assert(PortBaseAddr >= 0x40020000UL);
  my_assert(PortBaseAddr < 0x40023000UL);
  my_assert(Pin <= 15);

  const uint32_t ThePin = Pin;

  // Configure GPIO as Output Pin
  temp  = REG32(GPIO_MODER(PortBaseAddr));
  temp &= ~(0x03UL << (ThePin * 2UL));
  temp |= (0x01UL << (ThePin * 2UL));  // output
  REG32(GPIO_MODER(PortBaseAddr)) = temp;

  // Output speed
  temp  = REG32(GPIO_SPEEDR(PortBaseAddr));
  temp &= ~(0x03UL << (ThePin * 2UL));
  temp |= (0x02UL << (ThePin * 2UL));  // High Speed
  REG32(GPIO_SPEEDR(PortBaseAddr)) = temp;

  // IO Output type - Push Pull (not open drain)
  temp  = REG32(GPIO_OTYPER(PortBaseAddr));
  temp &= ~(0x01UL << ThePin);
  temp |=  (0x00UL << ThePin);  // Push Pull
  REG32(GPIO_OTYPER(PortBaseAddr)) = temp;

  // Pull Up / Pull Down 
  temp  = REG32(GPIO_PUPDR(PortBaseAddr));
  temp &= ~(0x03UL << (ThePin * 2UL));
  temp |= (0x00UL << (ThePin * 2UL));  // No Pull Up or Pull Down
  REG32(GPIO_PUPDR(PortBaseAddr)) = temp;

}

// **********************************************************
/*!
* @brief Configure GPIO PF.10 as ADC input channel for potentiometer .
*/
void BSP_AdcPin_init(void)
{
  volatile uint32_t temp;
  const uint32_t PortBaseAddr = GPIO_PORT_F_BASE_ADDR;
  const uint32_t Pin = 10UL;

  const uint32_t ThePin = Pin;

  // Configure GPIO as Analog Pin
  temp  = REG32(GPIO_MODER(PortBaseAddr));
  temp &= ~(0x03UL << (ThePin * 2UL));
  temp |= (0x03UL << (ThePin * 2UL));  // output
  REG32(GPIO_MODER(PortBaseAddr)) = temp;

  // Pull Up / Pull Down 
  temp  = REG32(GPIO_PUPDR(PortBaseAddr));
  temp &= ~(0x03UL << (ThePin * 2UL));
  temp |= (0x00UL << (ThePin * 2UL));  // No Pull Up or Pull Down
  REG32(GPIO_PUPDR(PortBaseAddr)) = temp;

}

// **********************************************************
/*!
* @brief Initialize the specified LED
* @param[in] Led  The LED number to be manipulated.
*/
static void BSP_LED_Init(BOARD_LED_ID Led)
{
  if (Led == LED1)
  {
    // Enable the LED's GPIO clock
    LED1_GPIO_PORT_CLK_ENABLE();

    // Configure the LED's GPIO pin for output
    BG_GPIO_Init_Output(GPIO_PORT_I_BASE_ADDR, LED_1_PIN);
    
    // Init LED1 state to OFF
    BG_GPIO_WritePin(GPIO_PORT_I_BASE_ADDR, LED_1_PIN, 0);
  }
  else if (Led == LED2)
  {
    // Enable the LED's GPIO clock
    LED2_GPIO_PORT_CLK_ENABLE();

    // Configure the LED's GPIO pin for output
    BG_GPIO_Init_Output(GPIO_PORT_A_BASE_ADDR, LED_2_PIN);

    // Init LED2 state to OFF
    BG_GPIO_WritePin(GPIO_PORT_A_BASE_ADDR, LED_2_PIN, 0);
  }
}


// **********************************************************
/*!
* @brief Turn on specified LED
* @param[in] Led  The LED number to be manipulated.
*/
void BSP_LED_On(BOARD_LED_ID Led)
{
  if (Led == LED1)
  {
    BG_GPIO_WritePin(GPIO_PORT_I_BASE_ADDR, LED_1_PIN, 1);  // PI1
  }
  else if (Led == LED2)
  {
    BG_GPIO_WritePin(GPIO_PORT_A_BASE_ADDR, LED_2_PIN, 1);  // PA0
  }
}

// **********************************************************
/*!
* @brief Turn off specified LED
* @param[in] Led  The LED number to be manipulated.
*/
void BSP_LED_Off(BOARD_LED_ID Led)
{
  if (Led == LED1)
  {
    BG_GPIO_WritePin(GPIO_PORT_I_BASE_ADDR, LED_1_PIN, 0);  // PI1
  }
  else if (Led == LED2)
  {
    BG_GPIO_WritePin(GPIO_PORT_A_BASE_ADDR, LED_2_PIN, 0);  // PA0
  }
}

// **********************************************************
/*!
* @brief Toggle specified LED
* @param[in] Led  The LED number to be manipulated.
*/
void BSP_LED_Toggle(BOARD_LED_ID Led)
{
  if (Led == LED1)
  {
    BG_GPIO_TogglePin(GPIO_PORT_I_BASE_ADDR, LED_1_PIN);    // Toggle PI1
  }
  else if (Led == LED2)
  {
    BG_GPIO_TogglePin(GPIO_PORT_A_BASE_ADDR, LED_2_PIN);    // Toggle PA0
  }
}


// **********************************************************
/**
  * @brief  Init a few GPIOs as outputs for probing / testing / debugging
  * @note Assumes PF6 - PF9 unused
  * @retval None
  */
static void BSP_Test_Outputs_Init(void)
{
  // Enable the Test Output Port's GPIO clock
  TESTOUT_GPIO_PORT_CLK_ENABLE();

  for (uint32_t pin = TEST_OUT_PIN_FIRST; pin <= TEST_OUT_PIN_LAST; ++pin)
  {
    // Configure the GPIO pin for output
    BG_GPIO_Init_Output(TEST_OUT_PORT, pin);

    // Init pin to OFF / LOW / RESET
    BG_GPIO_WritePin(TEST_OUT_PORT, pin, 0);
  }
}


// **********************************************************
/**
  * @brief  Drive a test output high
  * @param  id: output to be driven (6-9)
  * @note Assumes PF6 - PF9 used
  * @retval None
  */
void BSP_Test_Output_On(uint32_t id)
{
  my_assert(id >= 6);
  my_assert(id <= 9);
  BG_GPIO_WritePin(TEST_OUT_PORT, id, 1);
}

/**
  * @brief  Drive a test output low
  * @param  id: output to be driven (6-9)
  * @note Assumes PF6 - PF9 used
  * @retval None
  */
void BSP_Test_Output_Off(uint32_t id)
{
  my_assert(id >= 6);
  my_assert(id <= 9);
  BG_GPIO_WritePin(TEST_OUT_PORT, id, 0);
}

/**
  * @brief  Toggle a test output
  * @param  id: output to be driven (6-9)
  * @note Assumes PF6 - PF9 used
  * @retval None
  */
void BSP_Test_Output_Toggle(uint32_t id)
{
  my_assert(id >= 6);
  my_assert(id <= 9);
  BG_GPIO_TogglePin(TEST_OUT_PORT, id);
}


