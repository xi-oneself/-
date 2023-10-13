#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

lv_meter_indicator_t *screen_meter_tempandhumi_scale_1_arc_0;
lv_meter_indicator_t *screen_meter_tempandhumi_scale_1_scaleline_0;
lv_meter_indicator_t *screen_meter_pressure_scale_1_scaleline_0;
lv_meter_indicator_t *screen_meter_tvoc_scale_1_scaleline_0;

void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	static lv_style_t style_screen_main_main_default;
	if (style_screen_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_main_main_default);
	else
		lv_style_init(&style_screen_main_main_default);
	lv_style_set_bg_color(&style_screen_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_main_main_default, 0);
	lv_obj_add_style(ui->screen, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_3, -2, 0);
	lv_obj_set_size(ui->screen_label_3, 91, 240);
	
	lv_obj_set_scrollbar_mode(ui->screen_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_3, "");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_3_main_main_default
	static lv_style_t style_screen_label_3_main_main_default;
	if (style_screen_label_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_3_main_main_default);
	else
		lv_style_init(&style_screen_label_3_main_main_default);
	lv_style_set_radius(&style_screen_label_3_main_main_default, 0);
	//
	//lv_style_set_bg_color(&style_screen_label_3_main_main_default, lv_color_make(0x59, 0xb0, 0xf7));
	//lv_style_set_bg_grad_color(&style_screen_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_color(&style_screen_label_3_main_main_default, lv_color_make(0xcc, 0xcc, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_3_main_main_default, lv_color_make(0x00, 0x00, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_3_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_3_main_main_default, 255);
	lv_style_set_text_color(&style_screen_label_3_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_letter_space(&style_screen_label_3_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_3_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_3, &style_screen_label_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	


	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_4, 89, 0);
	lv_obj_set_size(ui->screen_label_4, 99, 240);
	lv_obj_set_scrollbar_mode(ui->screen_label_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_4, "");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_4_main_main_default
	static lv_style_t style_screen_label_4_main_main_default;
	if (style_screen_label_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_4_main_main_default);
	else
		lv_style_init(&style_screen_label_4_main_main_default);
	lv_style_set_radius(&style_screen_label_4_main_main_default, 0);
	//
	//lv_style_set_bg_color(&style_screen_label_4_main_main_default, lv_color_make(0xed, 0x8a, 0xf4));
	//lv_style_set_bg_grad_color(&style_screen_label_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	
	lv_style_set_bg_color(&style_screen_label_4_main_main_default, lv_color_make(0xcc, 0xff, 0xcc));
	lv_style_set_bg_grad_color(&style_screen_label_4_main_main_default, lv_color_make(0x00, 0xff, 0x00));
	
	lv_style_set_bg_grad_dir(&style_screen_label_4_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_4_main_main_default, 255);
	lv_style_set_text_color(&style_screen_label_4_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_letter_space(&style_screen_label_4_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_4_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_4_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_4_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_4, &style_screen_label_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_5, 186, 0);
	lv_obj_set_size(ui->screen_label_5, 98, 240);
	lv_obj_set_scrollbar_mode(ui->screen_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_5, "");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_5_main_main_default
	static lv_style_t style_screen_label_5_main_main_default;
	if (style_screen_label_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_5_main_main_default);
	else
		lv_style_init(&style_screen_label_5_main_main_default);
	lv_style_set_radius(&style_screen_label_5_main_main_default, 0);
	//
	//lv_style_set_bg_color(&style_screen_label_5_main_main_default, lv_color_make(0xa8, 0xf7, 0x59));
	//lv_style_set_bg_grad_color(&style_screen_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	
	lv_style_set_bg_color(&style_screen_label_5_main_main_default, lv_color_make(0xd3, 0xd3, 0xd3)); // 设置浅紫色背景
  lv_style_set_bg_grad_color(&style_screen_label_5_main_main_default, lv_color_make(0x80 , 0x80, 0x80)); // 设置深紫色渐变背景
	
	lv_style_set_bg_grad_dir(&style_screen_label_5_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_5_main_main_default, 255);
	lv_style_set_text_color(&style_screen_label_5_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_letter_space(&style_screen_label_5_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_5_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_5_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_5_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_5, &style_screen_label_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_meter_tempandhumi
	ui->screen_meter_tempandhumi = lv_meter_create(ui->screen);
	lv_obj_set_pos(ui->screen_meter_tempandhumi, 5, 72);
	lv_obj_set_size(ui->screen_meter_tempandhumi, 85, 85);
	lv_obj_set_scrollbar_mode(ui->screen_meter_tempandhumi, LV_SCROLLBAR_MODE_OFF);

	//add scale screen_meter_tempandhumi_scale_1
	lv_meter_scale_t *screen_meter_tempandhumi_scale_1 = lv_meter_add_scale(ui->screen_meter_tempandhumi);
	lv_meter_set_scale_ticks(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1, 100, 5, 8, lv_color_make(0x00, 0x00, 0x00));
	lv_meter_set_scale_range(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1, 0, 100, 320, 90);

	//add arc for screen_meter_tempandhumi_scale_1
	
	screen_meter_tempandhumi_scale_1_arc_0 = lv_meter_add_arc(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1, 7, lv_color_make(0x00, 0xff, 0xff), 8);
	lv_meter_set_indicator_start_value(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_arc_0, 0);
	lv_meter_set_indicator_end_value(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_arc_0, 20);

	//add scale line for screen_meter_tempandhumi_scale_1
	screen_meter_tempandhumi_scale_1_scaleline_0 = lv_meter_add_scale_lines(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1, lv_color_make(0xe4, 0x3f, 0x79), lv_color_make(0xff, 0x00, 0x4c), true, 0);
	lv_meter_set_indicator_start_value(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_scaleline_0, 0);
	lv_meter_set_indicator_end_value(ui->screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_scaleline_0, 80);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_tempandhumi_main_main_default
	static lv_style_t style_screen_meter_tempandhumi_main_main_default;
	if (style_screen_meter_tempandhumi_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_tempandhumi_main_main_default);
	else
		lv_style_init(&style_screen_meter_tempandhumi_main_main_default);
	lv_style_set_radius(&style_screen_meter_tempandhumi_main_main_default, 200);
	lv_style_set_bg_color(&style_screen_meter_tempandhumi_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_meter_tempandhumi_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_meter_tempandhumi_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_meter_tempandhumi_main_main_default, 0);
	lv_style_set_border_color(&style_screen_meter_tempandhumi_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_meter_tempandhumi_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_meter_tempandhumi_main_main_default, 255);
	lv_obj_add_style(ui->screen_meter_tempandhumi, &style_screen_meter_tempandhumi_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_tempandhumi_main_ticks_default
	static lv_style_t style_screen_meter_tempandhumi_main_ticks_default;
	if (style_screen_meter_tempandhumi_main_ticks_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_tempandhumi_main_ticks_default);
	else
		lv_style_init(&style_screen_meter_tempandhumi_main_ticks_default);
	lv_style_set_text_color(&style_screen_meter_tempandhumi_main_ticks_default, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_meter_tempandhumi_main_ticks_default, &lv_font_simsun_12);
	lv_obj_add_style(ui->screen_meter_tempandhumi, &style_screen_meter_tempandhumi_main_ticks_default, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_meter_pressure
	ui->screen_meter_pressure = lv_meter_create(ui->screen);
	lv_obj_set_pos(ui->screen_meter_pressure, 93, 72);
	lv_obj_set_size(ui->screen_meter_pressure, 91, 91);
	lv_obj_set_scrollbar_mode(ui->screen_meter_pressure, LV_SCROLLBAR_MODE_OFF);

	//add scale screen_meter_pressure_scale_1
	lv_meter_scale_t *screen_meter_pressure_scale_1 = lv_meter_add_scale(ui->screen_meter_pressure);
	lv_meter_set_scale_ticks(ui->screen_meter_pressure, screen_meter_pressure_scale_1, 100, 5, 8, lv_color_make(0x00, 0x00, 0x00));
	lv_meter_set_scale_range(ui->screen_meter_pressure, screen_meter_pressure_scale_1, 0, 999999, 320, 90);

	//add scale line for screen_meter_pressure_scale_1
	screen_meter_pressure_scale_1_scaleline_0 = lv_meter_add_scale_lines(ui->screen_meter_pressure, screen_meter_pressure_scale_1, lv_color_make(0x02, 0x12, 0xf2), lv_color_make(0x62, 0x50, 0xed), true, 0);
	lv_meter_set_indicator_start_value(ui->screen_meter_pressure, screen_meter_pressure_scale_1_scaleline_0, 0);
	lv_meter_set_indicator_end_value(ui->screen_meter_pressure, screen_meter_pressure_scale_1_scaleline_0, 20);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_pressure_main_main_default
	static lv_style_t style_screen_meter_pressure_main_main_default;
	if (style_screen_meter_pressure_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_pressure_main_main_default);
	else
		lv_style_init(&style_screen_meter_pressure_main_main_default);
	lv_style_set_radius(&style_screen_meter_pressure_main_main_default, 200);
	lv_style_set_bg_color(&style_screen_meter_pressure_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_meter_pressure_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_meter_pressure_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_meter_pressure_main_main_default, 0);
	lv_style_set_border_color(&style_screen_meter_pressure_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_meter_pressure_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_meter_pressure_main_main_default, 255);
	lv_obj_add_style(ui->screen_meter_pressure, &style_screen_meter_pressure_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_pressure_main_ticks_default
	static lv_style_t style_screen_meter_pressure_main_ticks_default;
	if (style_screen_meter_pressure_main_ticks_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_pressure_main_ticks_default);
	else
		lv_style_init(&style_screen_meter_pressure_main_ticks_default);
	lv_style_set_text_color(&style_screen_meter_pressure_main_ticks_default, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_meter_pressure_main_ticks_default, &lv_font_simsun_12);
	lv_obj_add_style(ui->screen_meter_pressure, &style_screen_meter_pressure_main_ticks_default, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_meter_tvoc
	ui->screen_meter_tvoc = lv_meter_create(ui->screen);
	lv_obj_set_pos(ui->screen_meter_tvoc, 189, 72);
	lv_obj_set_size(ui->screen_meter_tvoc, 91, 91);
	lv_obj_set_scrollbar_mode(ui->screen_meter_tvoc, LV_SCROLLBAR_MODE_OFF);

	//add scale screen_meter_tvoc_scale_1
	lv_meter_scale_t *screen_meter_tvoc_scale_1 = lv_meter_add_scale(ui->screen_meter_tvoc);
	lv_meter_set_scale_ticks(ui->screen_meter_tvoc, screen_meter_tvoc_scale_1, 100, 5, 8, lv_color_make(0x00, 0x00, 0x00));
	lv_meter_set_scale_range(ui->screen_meter_tvoc, screen_meter_tvoc_scale_1, 0, 99999, 320, 90);

	//add scale line for screen_meter_tvoc_scale_1
	
	screen_meter_tvoc_scale_1_scaleline_0 = lv_meter_add_scale_lines(ui->screen_meter_tvoc, screen_meter_tvoc_scale_1, lv_color_make(0xe1, 0xff, 0x00), lv_color_make(0xc8, 0xf2, 0x54), true, 0);
	lv_meter_set_indicator_start_value(ui->screen_meter_tvoc, screen_meter_tvoc_scale_1_scaleline_0, 0);
	lv_meter_set_indicator_end_value(ui->screen_meter_tvoc, screen_meter_tvoc_scale_1_scaleline_0, 20);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_tvoc_main_main_default
	static lv_style_t style_screen_meter_tvoc_main_main_default;
	if (style_screen_meter_tvoc_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_tvoc_main_main_default);
	else
		lv_style_init(&style_screen_meter_tvoc_main_main_default);
	lv_style_set_radius(&style_screen_meter_tvoc_main_main_default, 200);
	lv_style_set_bg_color(&style_screen_meter_tvoc_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_meter_tvoc_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_meter_tvoc_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_meter_tvoc_main_main_default, 0);
	lv_style_set_border_color(&style_screen_meter_tvoc_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_meter_tvoc_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_meter_tvoc_main_main_default, 255);
	lv_obj_add_style(ui->screen_meter_tvoc, &style_screen_meter_tvoc_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_tvoc_main_ticks_default
	static lv_style_t style_screen_meter_tvoc_main_ticks_default;
	if (style_screen_meter_tvoc_main_ticks_default.prop_cnt > 1)
		lv_style_reset(&style_screen_meter_tvoc_main_ticks_default);
	else
		lv_style_init(&style_screen_meter_tvoc_main_ticks_default);
	lv_style_set_text_color(&style_screen_meter_tvoc_main_ticks_default, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_meter_tvoc_main_ticks_default, &lv_font_simsun_12);
	lv_obj_add_style(ui->screen_meter_tvoc, &style_screen_meter_tvoc_main_ticks_default, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_spangroup_1
	ui->screen_spangroup_1 = lv_spangroup_create(ui->screen);
	lv_obj_set_pos(ui->screen_spangroup_1, 19-5, 54);
	lv_obj_set_size(ui->screen_spangroup_1, 75, 25);
	lv_obj_set_scrollbar_mode(ui->screen_spangroup_1, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_spangroup_1, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_spangroup_1, LV_SPAN_MODE_BREAK);

	//Write style state: LV_STATE_DEFAULT for style_screen_spangroup_1_main_main_default
	static lv_style_t style_screen_spangroup_1_main_main_default;
	if (style_screen_spangroup_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_spangroup_1_main_main_default);
	else
		lv_style_init(&style_screen_spangroup_1_main_main_default);
	lv_style_set_radius(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_spangroup_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_spangroup_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_spangroup_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_border_color(&style_screen_spangroup_1_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_spangroup_1_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_spangroup_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_spangroup_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_spangroup_1, &style_screen_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//create spans
	lv_span_t *screen_spangroup_1_span;

	//create a new span
	screen_spangroup_1_span = lv_spangroup_new_span(ui->screen_spangroup_1);
	lv_span_set_text(screen_spangroup_1_span, "  温湿度  ");
	lv_style_set_text_color(&screen_spangroup_1_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_spangroup_1_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_spangroup_1);

	//Write codes screen_spangroup_2
	ui->screen_spangroup_2 = lv_spangroup_create(ui->screen);
	lv_obj_set_pos(ui->screen_spangroup_2, 111, 55);
	lv_obj_set_size(ui->screen_spangroup_2, 50, 24);
	lv_obj_set_scrollbar_mode(ui->screen_spangroup_2, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_spangroup_2, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_spangroup_2, LV_SPAN_MODE_BREAK);

	//Write style state: LV_STATE_DEFAULT for style_screen_spangroup_2_main_main_default
	static lv_style_t style_screen_spangroup_2_main_main_default;
	if (style_screen_spangroup_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_spangroup_2_main_main_default);
	else
		lv_style_init(&style_screen_spangroup_2_main_main_default);
	lv_style_set_radius(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_spangroup_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_spangroup_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_spangroup_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_border_color(&style_screen_spangroup_2_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_spangroup_2_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_spangroup_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_spangroup_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_spangroup_2, &style_screen_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//create spans
	lv_span_t *screen_spangroup_2_span;

	//create a new span
	screen_spangroup_2_span = lv_spangroup_new_span(ui->screen_spangroup_2);
	lv_span_set_text(screen_spangroup_2_span, "气压");
	lv_style_set_text_color(&screen_spangroup_2_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_spangroup_2_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_spangroup_2_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_spangroup_2);

	//Write codes screen_spangroup_3
	ui->screen_spangroup_3 = lv_spangroup_create(ui->screen);
	lv_obj_set_pos(ui->screen_spangroup_3, 186, 55);
	lv_obj_set_size(ui->screen_spangroup_3, 101, 24);
	lv_obj_set_scrollbar_mode(ui->screen_spangroup_3, LV_SCROLLBAR_MODE_OFF);
	lv_spangroup_set_align(ui->screen_spangroup_3, LV_TEXT_ALIGN_LEFT);
	lv_spangroup_set_overflow(ui->screen_spangroup_3, LV_SPAN_OVERFLOW_CLIP);
	lv_spangroup_set_mode(ui->screen_spangroup_3, LV_SPAN_MODE_BREAK);

	//Write style state: LV_STATE_DEFAULT for style_screen_spangroup_3_main_main_default
	static lv_style_t style_screen_spangroup_3_main_main_default;
	if (style_screen_spangroup_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_spangroup_3_main_main_default);
	else
		lv_style_init(&style_screen_spangroup_3_main_main_default);
	lv_style_set_radius(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_spangroup_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_spangroup_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_spangroup_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_border_color(&style_screen_spangroup_3_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_spangroup_3_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_spangroup_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_spangroup_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_spangroup_3, &style_screen_spangroup_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//create spans
	lv_span_t *screen_spangroup_3_span;

	//create a new span
	screen_spangroup_3_span = lv_spangroup_new_span(ui->screen_spangroup_3);
	lv_span_set_text(screen_spangroup_3_span, "有害气体");
	lv_style_set_text_color(&screen_spangroup_3_span->style, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_decor(&screen_spangroup_3_span->style, LV_TEXT_DECOR_NONE);
	lv_style_set_text_font(&screen_spangroup_3_span->style, &lv_font_simsun_24);
	lv_spangroup_refr_mode(ui->screen_spangroup_3);

	//Write codes screen_label_temp_val——温度值
	ui->screen_label_temp_val = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_temp_val, 9, 168);
	lv_obj_set_size(ui->screen_label_temp_val, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_temp_val, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_temp_val, "0.00C");
	lv_label_set_long_mode(ui->screen_label_temp_val, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_temp_val_main_main_default
	static lv_style_t style_screen_label_temp_val_main_main_default;
	if (style_screen_label_temp_val_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_temp_val_main_main_default);
	else
		lv_style_init(&style_screen_label_temp_val_main_main_default);
	lv_style_set_radius(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_temp_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_temp_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_temp_val_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_temp_val_main_main_default, lv_color_make(0xec, 0x4b, 0x4b));
	lv_style_set_text_font(&style_screen_label_temp_val_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_temp_val_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_temp_val_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_temp_val_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_temp_val_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_temp_val, &style_screen_label_temp_val_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_pressure_val——气压值
	ui->screen_label_pressure_val = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_pressure_val, 100, 168);
	lv_obj_set_size(ui->screen_label_pressure_val, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_pressure_val, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_pressure_val, "0");
	lv_label_set_long_mode(ui->screen_label_pressure_val, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_pressure_val_main_main_default
	static lv_style_t style_screen_label_pressure_val_main_main_default;
	if (style_screen_label_pressure_val_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_pressure_val_main_main_default);
	else
		lv_style_init(&style_screen_label_pressure_val_main_main_default);
	lv_style_set_radius(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_pressure_val_main_main_default, lv_color_make(0x35, 0x28, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_pressure_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_pressure_val_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_pressure_val_main_main_default, lv_color_make(0x33, 0x36, 0xff));
	lv_style_set_text_font(&style_screen_label_pressure_val_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_pressure_val_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_pressure_val_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_pressure_val_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_pressure_val_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_pressure_val, &style_screen_label_pressure_val_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	
	
	
	//Write codes screen_label_vtoc_val——有害气体值
	ui->screen_label_vtoc_val = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_vtoc_val, 195, 168);
	lv_obj_set_size(ui->screen_label_vtoc_val, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_vtoc_val, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_vtoc_val, "0");
	lv_label_set_long_mode(ui->screen_label_vtoc_val, LV_LABEL_LONG_WRAP);





	//Write style state: LV_STATE_DEFAULT for style_screen_label_vtoc_val_main_main_default
	static lv_style_t style_screen_label_vtoc_val_main_main_default;
	if (style_screen_label_vtoc_val_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_vtoc_val_main_main_default);
	else
		lv_style_init(&style_screen_label_vtoc_val_main_main_default);
	lv_style_set_radius(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_vtoc_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_vtoc_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_vtoc_val_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_vtoc_val_main_main_default, lv_color_make(0x6b, 0x7f, 0x0b));
	lv_style_set_text_font(&style_screen_label_vtoc_val_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_vtoc_val_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_vtoc_val_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_vtoc_val_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_vtoc_val, &style_screen_label_vtoc_val_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1——气压单位
	ui->screen_label_1 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_1, 99, 196);
	lv_obj_set_size(ui->screen_label_1, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_1, "PA");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_1_main_main_default
	static lv_style_t style_screen_label_1_main_main_default;
	if (style_screen_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_1_main_main_default);
	else
		lv_style_init(&style_screen_label_1_main_main_default);
	lv_style_set_radius(&style_screen_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_1_main_main_default, lv_color_make(0x33, 0x36, 0xff));
	lv_style_set_text_font(&style_screen_label_1_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_1_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_1, &style_screen_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2——有害气体单位
	ui->screen_label_2 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_2, 195, 196);
	lv_obj_set_size(ui->screen_label_2, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_2, "PPB");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_2_main_main_default
	static lv_style_t style_screen_label_2_main_main_default;
	if (style_screen_label_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_2_main_main_default);
	else
		lv_style_init(&style_screen_label_2_main_main_default);
	lv_style_set_radius(&style_screen_label_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_2_main_main_default, lv_color_make(0x6b, 0x7f, 0x0b));
	lv_style_set_text_font(&style_screen_label_2_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_2_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_2_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_2, &style_screen_label_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_humi_val——湿度单位
	ui->screen_label_humi_val = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_humi_val, 9, 196);
	lv_obj_set_size(ui->screen_label_humi_val, 80, 20);
	lv_obj_set_scrollbar_mode(ui->screen_label_humi_val, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_humi_val, "0%RH");
	lv_label_set_long_mode(ui->screen_label_humi_val, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_humi_val_main_main_default
	static lv_style_t style_screen_label_humi_val_main_main_default;
	if (style_screen_label_humi_val_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_humi_val_main_main_default);
	else
		lv_style_init(&style_screen_label_humi_val_main_main_default);
	lv_style_set_radius(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_humi_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_humi_val_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_humi_val_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_humi_val_main_main_default, lv_color_make(0x20, 0xc9, 0xf3));
	lv_style_set_text_font(&style_screen_label_humi_val_main_main_default, &lv_font_montserratMedium_16);
	lv_style_set_text_letter_space(&style_screen_label_humi_val_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_humi_val_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_humi_val_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_humi_val_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_humi_val, &style_screen_label_humi_val_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//电量图标
	ui->label_power = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->label_power, 220, 10);
	lv_label_set_text(ui->label_power, LV_SYMBOL_BATTERY_FULL " ");

	//设置图标
	ui->label_set = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->label_set, 30, 10);
	//向右移动，向下移动
	lv_label_set_text(ui->label_set, LV_SYMBOL_SETTINGS " ");

}
