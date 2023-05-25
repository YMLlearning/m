#ifndef __USART3_H
#define __USART3_H

#include "sys.h" 
#include <stdio.h>

void uart3_init(u32 bound);
void Serial_SendByte(uint8_t Byte);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void UsartPrintf(USART_TypeDef * USARTx,char * fmt ,...);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Usart_SendHalfWord( uint16_t data);

void USART3_IRQHandler(void);


#endif
