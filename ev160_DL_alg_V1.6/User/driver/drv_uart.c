/******************************************************************************
* �ļ���         : drv_uart.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : 2018.07
* ����           : �������򣬴�������
*
*******************************************************************************/

#include "hw_lib.h"

/*******************************************************************************
* ��������       : USART1_Config
* ����           : USART1���ã����ڵ���
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void USART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =  USART_Mode_Tx| USART_Mode_Rx;//����
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
}


/*******************************************************************************
* ��������       : USART3_Config
* ����           : USART3���ã�sim����
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void USART3_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =  USART_Mode_Tx| USART_Mode_Rx;//����
	USART_Init(USART3, &USART_InitStructure); 
	USART_Cmd(USART3, ENABLE);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  								 //����
}

/*******************************************************************************
* ��������       : USART4_Config
* ����           : USART4���ã���������
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void USART4_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =  USART_Mode_Tx| USART_Mode_Rx;//����
	USART_Init(UART4, &USART_InitStructure); 
	
	USART_Cmd(UART4, ENABLE);
	
}

/*******************************************************************************
* ��������       : USART5_Config
* ����           : USART5���ã���������
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void USART5_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =  USART_Mode_Tx| USART_Mode_Rx;//����
	USART_Init(UART5, &USART_InitStructure); 
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5, ENABLE);
	
}
/*******************************************************************************
* ��������       : fputc
* ����           : fputc����
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
int fputc(int ch, FILE *f)
{
//  USART_SendData(USART1, (unsigned char) ch);
//  while (!(USART1->SR & USART_FLAG_TXE));
// 
//  return (ch);
	/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

#if	SERIAL_DEBUG
#define CMD_HEAD							0x5A
#define CMD_CH_DATA						0xA1
//#define CMD_CH_DATA_LEN				512
#define CMD_CH_DATA_LEN				(512 * 12)
volatile u8 rev_state = 0;// 0����ʼ״̬��1���յ�����ͷ��2����ʼ��������
volatile u8 cmd_data_buf[CMD_CH_DATA_LEN] = {0};
volatile u16 cmd_data_len = 0;
bool rev_finished = FALSE;
volatile u16 len_ptr = 0;
volatile u16 len_ptr2 = 0;
volatile u16 len_ptr3 = 0;
void usart1_rev_irq_process(u8 data)
{	
	if(rev_finished == FALSE)
	{
	
		switch(rev_state)
		{
			case 0:
				if(data == CMD_HEAD) rev_state = 1;
				break;
			case 1:
				if(data == CMD_CH_DATA) 
				{
					cmd_data_len = CMD_CH_DATA_LEN;
					len_ptr = 0;
					rev_state = 2;
				}		
				break;
			case 2:
	cmd_data_buf[len_ptr2++] = data;
	if(len_ptr2 == CMD_CH_DATA_LEN) 
	{
		len_ptr2 = 0;
		len_ptr3++;
	}
				*((u8*)(AD_store_final) + len_ptr++) = data;
				if(len_ptr == cmd_data_len)
				{
					rev_state = 0;
					rev_finished = TRUE;
				}
				break;
			default:
				break;	
		}	
	}
}
bool is_uart_rev_finished(void)
{
	return rev_finished;
}
void uart_rev_en(void)
{
	rev_finished = FALSE;
}
u8 * get_buf(void)
{
	return (u8 *)cmd_data_buf;
}


/*****************  ����һ���ַ� **********************/
 void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART5 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

#endif
/*******************************************END OF FILE******************************************/

