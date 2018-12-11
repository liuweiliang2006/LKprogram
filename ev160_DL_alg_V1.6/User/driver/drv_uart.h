
#include "stm32f10x.h"

#ifndef DRV_URAT_H
#define DRV_URAT_H

void USART1_Config(void);
void USART3_Config(void);
void USART4_Config(void);
void USART5_Config(void);
void usart1_rev_irq_process(u8 data);

bool is_uart_rev_finished(void);
void uart_rev_en(void);
u8 * get_buf(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );

#endif

/*******************************************END OF FILE******************************************/
