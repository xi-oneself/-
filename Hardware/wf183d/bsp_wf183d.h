#ifndef _BSP_WF183D_H
#define _BSP_WF183D_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_WF183D_USART_TX_RCU     RCU_GPIOA   // 串口TX的端口时钟
#define BSP_WF183D_USART_RX_RCU     RCU_GPIOA   // 串口RX的端口时钟
#define BSP_WF183D_USART_RCU        RCU_USART1  // 串口1的时钟

#define BSP_WF183D_USART_TX_PORT    GPIOA				// 串口TX的端口
#define BSP_WF183D_USART_RX_PORT    GPIOA				// 串口RX的端口
#define BSP_WF183D_USART_AF 	    GPIO_AF_7   // 串口1的复用功能
#define BSP_WF183D_USART_TX_PIN     GPIO_PIN_2  // 串口TX的引脚
#define BSP_WF183D_USART_RX_PIN     GPIO_PIN_3 // 串口RX的引脚

#define BSP_WF183D_USART 			USART1      								// 串口1
#define BSP_WF183D_USART_IRQ     	USART1_IRQn 								// 串口1中断



void wf183d_gpio_config(void);
uint32_t GetAirPressureValue(void);

#endif