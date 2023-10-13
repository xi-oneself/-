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

//按键按下
#define KEY_DOWN    1

//按键松开
#define KEY_RELEASE 0


//设置模式
#define SET_MODE 	1
//主界面模式
#define MAIN_MODE 	0

//开关打开
#define SWITCH_OPEN 	1
//开关关闭
#define SWITCH_CLOSE	0	


void key_gpio_init(void);

//设置键
void set_key_scan(void);
uint8_t get_set_button_state(void);
void set_set_button_state(uint8_t state);

//左键
void left_key_scan(void);
uint8_t get_left_button_state(void);
void set_left_button_state(uint8_t state);

//右键
void right_key_scan(void);
uint8_t get_right_button_state(void);
void set_right_button_state(uint8_t state);

//返回键
void return_key_scan(void);
uint8_t get_return_button_state(void);
void set_return_button_state(uint8_t state);


//按键任务
void all_key_scan(lv_ui *gui_ui);

//获取当前模式
char get_mode_select(void);
//设置当前模式
void set_mode_select(char flag);

//获取上传云端开关的状态
char get_switch_server(void);

//设置上传云端开关的状态
void set_switch_server(char flag);

//获取从机显示开关的状态
char get_switch_slave(void);

//设置从机显示开关的状态
void set_switch_slave(char flag);

//设置界面显示初始化
void set_interface_disp_init(lv_ui *gui_ui);
#endif
