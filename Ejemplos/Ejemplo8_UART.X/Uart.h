/* 
 * File:   Uart.h
 * Author: gssan
 *
 * Created on 7 de febrero de 2023, 08:51 PM
 */

#ifndef UART_H
#define	UART_H



#include <stdint.h>
//Serial.begin(9600)
void UART_dsPIC_Init(uint32_t Baudios);
void UART_dsPIC_putC(char c);
void UART_dsPIC_putS(char *s);


#endif	/* UART_H */

