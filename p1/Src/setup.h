/** @file setup.h
* 
* @brief Various setup / config / initialization / utility stuff
*
* @par       
* COPYRIGHT NOTICE: (c) 2017 Barr Group.  
* All rights reserved.
*/ 

#ifndef __SETUP_H
#define __SETUP_H

#ifdef __cplusplus
extern "C" {
#endif

// Priority of SYSTICK interrupt; 0 = highest, 15 = lowest
#define  SYSTICK_INT_PRIORITY            ((uint32_t) 15)

// Public API function prototypes
void LowLevelInit(void);
void busywait_ms(uint32_t ms);
void assert_failed(void);
void Systick_IncTick(void);

#ifdef __cplusplus
}
#endif

#endif /* __SETUP_H */

