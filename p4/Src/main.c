
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
#include "uart.h"

volatile uint16_t curr_pot_read ;
volatile uint8_t  rx_byte_read ;

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

  // Init UART
  port_init();

  // Set the output pattern - 1Hz (500ms on / 500ms off).
  uint16_t delay = 500UL;
  BSP_LED_Off(LED1);
  BSP_LED_Off(LED2);
  for (;;)
  {
    /* Disable interrupts */
    __asm(" CPSID i"); // enable interrupt for ARM
    delay = curr_pot_read;
    /* Enable interrupts back */
    __asm(" CPSIE i"); // enable interrupt for ARM

    busywait_ms(50UL + delay);
    BSP_LED_Toggle(LED1);
    
    port_put_char( curr_pot_read >> 3);
    //printf('HI');
  }

  // We will never reach this point of the program
  // return 0;
}


