#ifndef _BSP_VOLTAGE_H__
#define	_BSP_VOLTAGE_H__

#include "gd32f4xx.h"
#include "lvgl.h"
#include "gui_guider.h"

void power_voltage_gpio_config(void);
float get_voltage_value(void);
void electric_quantity_display(lv_ui *ui);

#endif