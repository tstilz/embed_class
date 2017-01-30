
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
#include "pot.h"

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
  
  // Config ADC channel for potentiometer (12 bit reading, returned as 10 bits)
  pot_init();

  /* Infinite loop */
  while (1)
  {
    uint32_t delay;
    delay = pot_read();  // Reading is returned as 10 bits (0-1023)
    delay >>= 2UL;       // Convert 10-bit reading to 8 bits (0-255)
    busywait_ms(50UL + delay);
    BSP_LED_Toggle(LED2);
  }

  // We will never reach this point of the program
  // return 0;
}


