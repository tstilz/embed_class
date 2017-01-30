
/** @file uart.h
* 
* @brief Memory-Mapped UART Driver
*
* @par       
* COPYRIGHT NOTICE: (c) 2017 Barr Group.  
* All rights reserved.
*/ 

#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

// ****  Function prototypes for exported / public functions
// TODO: implement all of these
extern void port_init(void);
extern  int port_put_char(char c);
extern  int port_put_string(char const *p_str);
extern  int port_get_char(void);
extern void BG_UART_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

