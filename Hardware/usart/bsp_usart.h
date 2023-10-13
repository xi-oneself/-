#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_USART_TX_RCU  RCU_GPIOA   // ����TX�Ķ˿�ʱ��
#define BSP_USART_RX_RCU  RCU_GPIOA   // ����RX�Ķ˿�ʱ��
#define BSP_USART_RCU     RCU_USART5  // ����0��ʱ��

#define BSP_USART_TX_PORT GPIOA				// ����TX�Ķ˿�
#define BSP_USART_RX_PORT GPIOA				// ����RX�Ķ˿�
#define BSP_USART_AF 			GPIO_AF_8   // ����0�ĸ��ù���
#define BSP_USART_TX_PIN  GPIO_PIN_11  // ����TX������
#define BSP_USART_RX_PIN  GPIO_PIN_12 // ����RX������

#define BSP_USART 						USART5      								// ����0
#define BSP_USART_IRQ     		USART5_IRQn 								// ����0�ж�
#define BSP_USART_IRQHandler  USART5_IRQHandler						// ����0�жϷ�����

/* ���ڻ����������ݳ��� */
#define USART_RECEIVE_LENGTH  4096

extern uint8_t  g_recv_buff[USART_RECEIVE_LENGTH]; // ���ջ�����
extern uint16_t g_recv_length;										 // �������ݳ���
extern uint8_t  g_recv_complete_flag; 						 // ������ɱ�־λ

void usart_gpio_config(uint32_t band_rate);  			 // ���ô���
void usart_send_data(uint8_t ucch);          			 // ����һ���ַ�
void usart_send_string(uint8_t *ucstr);      			 // ����һ���ַ���
void  usart_receive_clear(void);

#endif