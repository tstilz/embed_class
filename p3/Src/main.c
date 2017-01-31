
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

volatile uint16_t curr_pot_read ;

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

  // Init ADC3

  pot_init();
  __asm(" CPSIE i"); // enable interrupt for ARM
  
  // Set the output pattern - 1Hz (500ms on / 500ms off).
  uint16_t delay = 500UL;
  BSP_LED_Off(LED2);
  
  //__asm(" CPSID i"); // enable interrupt for ARM
  //busywait_ms(5UL);
  
  for (;;)
  {
    /* Disable interrupts */
    __asm(" CPSID i"); // enable interrupt for ARM

    delay = curr_pot_read;

    /* Enable interrupts back */
    __asm(" CPSIE i"); // enable interrupt for ARM

    busywait_ms(5UL + delay);
    BSP_LED_Toggle(LED2);
  }

  // We will never reach this point of the program
  // return 0;
}


