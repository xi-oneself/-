#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

lv_style_t style_screen_set_sw_slave_main_indicator_checked;

void setup_scr_screen_set(lv_ui *ui){

	//Write codes screen_set
	ui->screen_set = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_set, LV_SCROLLBAR_MODE_OFF);

	//Write codes screen_set_spangroup_7—设置字
	ui->screen_set_spangroup_7 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_7, 4, 10);
	lv_obj_set_size(ui->screen_set_spangroup_7, 273, 23);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_7, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_7, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_7, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_7, LV_SPAN_MODE_BREAK);

	//create spans
	lv_span_t *screen_set_spangroup_7_span;

	//create a new span
	screen_set_spangroup_7_span = lv_spangroup_new_span(ui->screen_set_spangroup_7);
	lv_span_set_text(screen_set_spangroup_7_span, "设置");
	/* 设置文本居中对齐 */ 
	lv_obj_set_style_text_align(ui->screen_set_spangroup_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
	lv_style_set_text_color(&screen_set_spangroup_7_span->style, lv_color_make(0x4d, 0x00, 0x82));
	lv_style_set_text_decor(&screen_set_spangroup_7_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_7_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_7);



	//Write codes screen_set_bar_light-亮度的进度条
	ui->screen_set_bar_light = lv_bar_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_bar_light, 64, 69);
	lv_obj_set_size(ui->screen_set_bar_light, 152, 18);
	lv_obj_set_scrollbar_mode(ui->screen_set_bar_light, LV_SCROLLBAR_MODE_OFF);

	lv_bar_set_value(ui->screen_set_bar_light, 50, LV_ANIM_ON);


	//Write codes screen_set_spangroup_6-亮度字	
	ui->screen_set_spangroup_6 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_6, 5, 64);
	lv_obj_set_size(ui->screen_set_spangroup_6, 54, 28);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_6, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_6, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_6, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_6, LV_SPAN_MODE_BREAK);
			
	//create spans
	lv_span_t *screen_set_spangroup_6_span;

	//create a new span
	screen_set_spangroup_6_span = lv_spangroup_new_span(ui->screen_set_spangroup_6);
	lv_span_set_text(screen_set_spangroup_6_span, "亮度");
	lv_style_set_text_color(&screen_set_spangroup_6_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_set_spangroup_6_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_6_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_6);


	//Write codes screen_set_label_light-百分比字
	
	ui->screen_set_label_light = lv_label_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_label_light, 220, 64);
	lv_obj_set_size(ui->screen_set_label_light, 64, 24);
	lv_obj_set_scrollbar_mode(ui->screen_set_label_light, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_set_label_light, "50%");
	lv_label_set_long_mode(ui->screen_set_label_light, LV_LABEL_LONG_WRAP);


	//Write codes screen_set_spangroup_1
	ui->screen_set_spangroup_1 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_1, 5, 102);
	lv_obj_set_size(ui->screen_set_spangroup_1, 200, 25);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_1, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_1, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_1, LV_SPAN_MODE_BREAK);


	//create spans
	lv_span_t *screen_set_spangroup_1_span;

	//create a new span

	screen_set_spangroup_1_span = lv_spangroup_new_span(ui->screen_set_spangroup_1);
	lv_span_set_text(screen_set_spangroup_1_span, "ESP8266");
	lv_style_set_text_color(&screen_set_spangroup_1_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_set_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_1_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_1);

	//Write codes screen_set_spangroup_2
	ui->screen_set_spangroup_2 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_2, 5, 137);
	lv_obj_set_size(ui->screen_set_spangroup_2, 200, 25);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_2, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_2, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_2, LV_SPAN_MODE_BREAK);


	//create spans
	lv_span_t *screen_set_spangroup_2_span;

	//create a new span
	screen_set_spangroup_2_span = lv_spangroup_new_span(ui->screen_set_spangroup_2);
	lv_span_set_text(screen_set_spangroup_2_span, "HC05");
	lv_style_set_text_color(&screen_set_spangroup_2_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_set_spangroup_2_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_2_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_2);

	//Write codes screen_set_sw_server—云端按键
	ui->screen_set_sw_server = lv_switch_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_sw_server, 220, 107);
	lv_obj_set_size(ui->screen_set_sw_server, 40, 20);
	lv_obj_set_scrollbar_mode(ui->screen_set_sw_server, LV_SCROLLBAR_MODE_OFF);



	//Write codes screen_set_sw_slave—从机按键
	ui->screen_set_sw_slave = lv_switch_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_sw_slave, 220, 142);
	lv_obj_set_size(ui->screen_set_sw_slave, 40, 20);
	lv_obj_set_scrollbar_mode(ui->screen_set_sw_slave, LV_SCROLLBAR_MODE_OFF);



	//Write style state: LV_STATE_CHECKED for style_screen_set_sw_slave_main_indicator_checked
  //按钮变色的
	if (style_screen_set_sw_slave_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_set_sw_slave_main_indicator_checked);
	else
		lv_style_init(&style_screen_set_sw_slave_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_set_sw_slave_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_set_sw_slave_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_set_sw_slave_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_set_sw_slave_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_set_sw_slave_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_set_sw_slave_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_set_sw_slave_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_set_sw_slave, &style_screen_set_sw_slave_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);


	//Write codes screen_set_spangroup_3
	ui->screen_set_spangroup_3 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_3, 5, 174);
	lv_obj_set_size(ui->screen_set_spangroup_3, 254, 23);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_3, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_3, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_3, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_3, LV_SPAN_MODE_BREAK);


	//create spans
	lv_span_t *screen_set_spangroup_3_span;

	//create a new span
	screen_set_spangroup_3_span = lv_spangroup_new_span(ui->screen_set_spangroup_3);
	lv_span_set_text(screen_set_spangroup_3_span, "(dyt)");
	lv_style_set_text_color(&screen_set_spangroup_3_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_set_spangroup_3_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_3_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_3);

	//create spans
	lv_span_t *screen_set_spangroup_4_span;

	//create a new span


/*
	screen_set_spangroup_4_span = lv_spangroup_new_span(ui->screen_set_spangroup_4);
	lv_span_set_text(screen_set_spangroup_4_span, " ");
	lv_style_set_text_color(&screen_set_spangroup_4_span->style, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_text_decor(&screen_set_spangroup_4_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_set_spangroup_4_span->style, &lv_font_simsun_16);
	lv_spangroup_refr_mode(ui->screen_set_spangroup_4);
*/
	//Write codes screen_set_spangroup_5
	/*
	ui->screen_set_spangroup_5 = lv_spangroup_create(ui->screen_set);
	lv_obj_set_pos(ui->screen_set_spangroup_5, 13, 218);
	lv_obj_set_size(ui->screen_set_spangroup_5, 181, 16);
	lv_obj_set_scrollbar_mode(ui->screen_set_spangroup_5, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_set_spangroup_5, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_set_spangroup_5, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_set_spangroup_5, LV_SPAN_MODE_BREAK);
*/
}

/*

这段代码是设置 `ui->screen_set_spangroup_3` 对象的位置和大小。

`lv_obj_set_pos(ui->screen_set_spangroup_3, 5, 174)` 设置了对象的位置，将其放置在 x 坐标为 5，y 坐标为 174 的位置。

`lv_obj_set_size(ui->screen_set_spangroup_3, 254, 23)` 设置了对象的大小，将其宽度设置为 254，高度设置为 23。

这样设置之后，`ui->screen_set_spangroup_3` 对象就会被放置在屏幕上的指定位置，并且大小为指定的宽度和高度。

*/
