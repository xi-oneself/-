#ifndef _BSP_AGS10_H_
#define _BSP_AGS10_H_

#include "gd32f4xx.h"


#define RCU_AGS10_SCL       RCU_GPIOD
#define PORT_AGS10_SCL      GPIOD
#define GPIO_AGS10_SCL      GPIO_PIN_4

#define RCU_AGS10_SDA       RCU_GPIOD
#define PORT_AGS10_SDA      GPIOD
#define GPIO_AGS10_SDA      GPIO_PIN_5


#define AGS10_SDA_IN()  {gpio_mode_set(PORT_AGS10_SDA, GPIO_MODE_INPUT,  GPIO_PUPD_PULLUP, GPIO_AGS10_SDA);}  //SDA输入模式
#define AGS10_SDA_OUT() {gpio_mode_set(PORT_AGS10_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AGS10_SDA);}  //SDA输出模式

#define AGS10_SCL(BIT)  gpio_bit_write( PORT_AGS10_SCL, GPIO_AGS10_SCL, BIT?SET:RESET)
#define AGS10_SDA(BIT)  gpio_bit_write( PORT_AGS10_SDA, GPIO_AGS10_SDA, BIT?SET:RESET)
#define AGS10_GETSDA()  gpio_input_bit_get( PORT_AGS10_SDA, GPIO_AGS10_SDA)

void ags10_gpio_init(void);
uint32_t ags10_read(void);
uint8_t tvoc_measured(void);
uint8_t get_tvoc_measured_accomplish_flag(void);
void set_tvoc_measured_accomplish_flag(uint8_t flag);
#endif
