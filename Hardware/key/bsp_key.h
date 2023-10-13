#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "gd32f4xx.h"
#include "LVGL.h"
#include "gui_guider.h"

#define     RCU_SET             RCU_GPIOA
#define     PORT_SET            GPIOA
#define     GPIO_SET            GPIO_PIN_12

#define     RCU_LEFT            RCU_GPIOA
#define     PORT_LEFT           GPIOA
#define     GPIO_LEFT           GPIO_PIN_11

#define     RCU_RIGHT           RCU_GPIOA
#define     PORT_RIGHT          GPIOA
#define     GPIO_RIGHT          GPIO_PIN_8

#define     RCU_RETURN          RCU_GPIOG
#define     PORT_RETURN         GPIOG
#define     GPIO_RETURN         GPIO_PIN_7

//��������
#define KEY_DOWN    1

//�����ɿ�
#define KEY_RELEASE 0


//����ģʽ
#define SET_MODE 	1
//������ģʽ
#define MAIN_MODE 	0

//���ش�
#define SWITCH_OPEN 	1
//���عر�
#define SWITCH_CLOSE	0	


void key_gpio_init(void);

//���ü�
void set_key_scan(void);
uint8_t get_set_button_state(void);
void set_set_button_state(uint8_t state);

//���
void left_key_scan(void);
uint8_t get_left_button_state(void);
void set_left_button_state(uint8_t state);

//�Ҽ�
void right_key_scan(void);
uint8_t get_right_button_state(void);
void set_right_button_state(uint8_t state);

//���ؼ�
void return_key_scan(void);
uint8_t get_return_button_state(void);
void set_return_button_state(uint8_t state);


//��������
void all_key_scan(lv_ui *gui_ui);

//��ȡ��ǰģʽ
char get_mode_select(void);
//���õ�ǰģʽ
void set_mode_select(char flag);

//��ȡ�ϴ��ƶ˿��ص�״̬
char get_switch_server(void);

//�����ϴ��ƶ˿��ص�״̬
void set_switch_server(char flag);

//��ȡ�ӻ���ʾ���ص�״̬
char get_switch_slave(void);

//���ôӻ���ʾ���ص�״̬
void set_switch_slave(char flag);

//���ý�����ʾ��ʼ��
void set_interface_disp_init(lv_ui *gui_ui);
#endif
