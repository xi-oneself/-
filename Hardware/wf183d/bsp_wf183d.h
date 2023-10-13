#ifndef _BSP_WF183D_H
#define _BSP_WF183D_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_WF183D_USART_TX_RCU     RCU_GPIOA   // ����TX�Ķ˿�ʱ��
#define BSP_WF183D_USART_RX_RCU     RCU_GPIOA   // ����RX�Ķ˿�ʱ��
#define BSP_WF183D_USART_RCU        RCU_USART1  // ����1��ʱ��

#define BSP_WF183D_USART_TX_PORT    GPIOA				// ����TX�Ķ˿�
#define BSP_WF183D_USART_RX_PORT    GPIOA				// ����RX�Ķ˿�
#define BSP_WF183D_USART_AF 	    GPIO_AF_7   // ����1�ĸ��ù���
#define BSP_WF183D_USART_TX_PIN     GPIO_PIN_2  // ����TX������
#define BSP_WF183D_USART_RX_PIN     GPIO_PIN_3 // ����RX������

#define BSP_WF183D_USART 			USART1      								// ����1
#define BSP_WF183D_USART_IRQ     	USART1_IRQn 								// ����1�ж�



void wf183d_gpio_config(void);
uint32_t GetAirPressureValue(void);

#endif