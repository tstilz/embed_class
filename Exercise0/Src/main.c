
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
  
  // Set the output pattern - 1Hz (500ms on / 500ms off).
  uint32_t delay = 500UL;
  BSP_LED_On(LED1);
  BSP_LED_Off(LED2);
  for (;;)
  {
    // Blink LEDs and then delay
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    busywait_ms(delay);
  }

  // We will never reach this point of the program
  // return 0;
}


