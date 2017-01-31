
/** @file bsp.h
*
* @brief Board Support Package (BSP) APIs and definitions
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group.
* All rights reserved.
*/

#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif



// Enumeration for LEDs
// LED1 = on-board LED
// LED2 = external LED
typedef enum {
  LED1, LED2
} BOARD_LED_ID;

// Initialize BSP
void      BSP_Init(void);

// LED API
void      BSP_LED_On(BOARD_LED_ID Led);
void      BSP_LED_Off(BOARD_LED_ID Led);
void      BSP_LED_Toggle(BOARD_LED_ID Led);

// Push Button (PB) API
void BSP_PB_Init(void);
uint32_t BSP_PB_Read(void);

// GPIO output API
void      BSP_Test_Output_On(uint32_t id);
void      BSP_Test_Output_Off(uint32_t id);
void      BSP_Test_Output_Toggle(uint32_t id);

// Configure ADC pin for exercise
void      BSP_AdcPin_init(void);

// Configure USART pins for exercise
void      BSP_UartPin_init(void);



#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */


