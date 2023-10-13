#ifndef _BSP_BLUETOOTH_H_
#define _BSP_BLUETOOTH_H_
 
#include "gd32f4xx.h" 
#include "string.h"  
#include "systick.h"

//�Ƿ�������0����     1��ʼ  0�ر�
#define     DEBUG   1

#define  BLERX_LEN_MAX  200 

//������TXD��Ҫ�ӵ���
#define BLUETOOTH_TXD_RCU		  RCU_GPIOF
#define BLUETOOTH_TXD_PORT		GPIOF
#define BLUETOOTH_TXD_GPIO		GPIO_PIN_6

//������RXD��Ҫ�ӵ���
#define BLUETOOTH_RXD_RCU		  RCU_GPIOF
#define BLUETOOTH_RXD_PORT		GPIOF
#define BLUETOOTH_RXD_GPIO		GPIO_PIN_7

//��������
#define BLE_USART_RCU           RCU_UART6
#define BLE_USART 				      UART6      	               	
#define BLE_USART_AF 			      GPIO_AF_8  
#define BLE_USART_IRQ     		  UART6_IRQn 							
#define BLE_USART_IRQHandler    UART6_IRQHandler						

//
//���ӳɹ�ָʾ����
#define BLUETOOTH_LINK_RCU		RCU_GPIOC
#define BLUETOOTH_LINK_PORT		GPIOC
#define BLUETOOTH_LINK_GPIO		GPIO_PIN_2

#define BLUETOOTH_LINK  gpio_input_bit_get(BLUETOOTH_LINK_PORT, BLUETOOTH_LINK_GPIO)	

#define CONNECT 		1       //�������ӳɹ�
#define DISCONNECT  	0		//�������ӶϿ�


void Bluetooth_Init(void);
unsigned char Get_Bluetooth_ConnectFlag(void);
void Bluetooth_Mode(void);
//void Receive_Bluetooth_Data(void);
char* Receive_Bluetooth_Data(void);

void Receive_Bluetooth_Data_jieshou(void);

void Send_Bluetooth_Data(char *dat);
void Clear_BLERX_BUFF(void);
#endif

