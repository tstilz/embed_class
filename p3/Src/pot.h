
/** @file pot.h
*
* @brief Memory-Mapped Potentiometer Driver
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group.
* All rights reserved.
*/

#ifndef __POT_H
#define __POT_H

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint16_t curr_pot_read;
// ****  Function prototypes for exported / public functions
extern void pot_init(void);
extern uint16_t pot_read(void);
// extern void BG_ADC_IRQHandler(void);
// extern void BG_ADC_MspInit(void);
// void ADC_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __POT_H */

