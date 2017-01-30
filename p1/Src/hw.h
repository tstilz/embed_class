/** @file hw.h
* 
* @brief Hardware / register / bit manipulation definitions
*
* @par       
* COPYRIGHT NOTICE: (c) 2017 Barr Group.  
* All rights reserved.
*/ 

#ifndef __HW_H
#define __HW_H

#ifdef __cplusplus
extern "C" {
#endif

#define SET_BIT(REG, BIT)   ((REG) |= (BIT))
#define SET_BITn(REG, n)    ((REG) |= (1UL << (n)))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define CLEAR_BITn(REG, n)  ((REG) &= ~(1UL << (n)))
#define READ_BIT(REG, BIT)  ((REG) & (BIT))
#define READ_BITn(REG, n)   ((REG) & (1UL << (n))

// Macro for quick-and-dirty manipulation of registers
#define REG32(reg_addr)  ( * ( ( volatile uint32_t * ) (reg_addr) ) )
#define REG16(reg_addr)  ( * ( ( volatile uint16_t * ) (reg_addr) ) )
#define REG8(reg_addr)   ( * ( ( volatile  uint8_t * ) (reg_addr) ) )

#ifdef __cplusplus
}
#endif

#endif /* __HW_H */


