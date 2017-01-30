
/** \file main.c
*
* @brief Application startup / entry
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/

#include "project.h"
#include "bsp.h"
#include "hw.h"
#include "setup.h"

// API for sign extend function
extern int16_t signextend(uint16_t);

// Define the test cases (input / expected output) here
#define NUM_TESTS   6
static const uint16_t test[NUM_TESTS] = {0x0F64U, 0x0FFFU, 0x2CU, 0U, 0xFFFEU, 0xF001U };
static const int16_t  expect[NUM_TESTS] = {-156, -1, 44, 0, -2, 1 };


//#define NUM_TESTS   5
//static const uint16_t test[NUM_TESTS] = {0x0F64U, 0x0FFFU, 0x2CU, 0U, 0xFFFEU};
//static const int16_t  expect[NUM_TESTS] = {-156, -1, 44, 0, -2};

/*!
* @brief Main Program
* @return int - always returns 0
*/
int main(void)
{
  // Perform low-level platform initialization
  LowLevelInit();

  // Initialize the Board Support Package (BSP)
  BSP_Init();
  
  // Test the implementation of signextend().
  int n_errors = 0;

  for (int i = 0; i < NUM_TESTS; ++i)
  {
    if (expect[i] != signextend(test[i]))
    {
      n_errors++;
    }
  }

  // Set the output pattern.
  // Blink LD1 slowly to indicate success.
  // Set LD2 on solid to indicate failure.
  BSP_LED_Off(LED1);
  BSP_LED_Off(LED2);
  //const BOARD_LED_ID the_led = ((0 == n_errors) ? LED1 : LED2);
  const BOARD_LED_ID the_led = LED2;
  const uint32_t the_delay = ((0 == n_errors) ? 500UL : (500UL * 1000UL));
  for (;;)
  {
    // Blink LED and then delay
    BSP_LED_Toggle(the_led);
    busywait_ms(the_delay);
  }

  // We will never reach this point of the program
  // return 0;
}


