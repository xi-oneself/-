/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_label_3;//背景板-1
	lv_obj_t *screen_label_4;//背景板-2
	lv_obj_t *screen_label_5;//背景板-3
	lv_obj_t *screen_meter_tempandhumi;//温湿度图
	lv_obj_t *screen_meter_pressure;//气压图
	lv_obj_t *screen_meter_tvoc;//有害气体图
	lv_obj_t *screen_spangroup_1;//温湿度（字）
	lv_obj_t *screen_spangroup_2;//气压（字）
	lv_obj_t *screen_spangroup_3;//有害气体（字）
	lv_obj_t *screen_label_temp_val;//温湿度值
	lv_obj_t *screen_label_pressure_val;//气压值
	lv_obj_t *screen_label_vtoc_val;//有害气体值
	lv_obj_t *screen_label_1;//气压单位
	lv_obj_t *screen_label_2;//有害气体单位
	lv_obj_t *screen_label_humi_val;//湿度单位
//	lv_obj_t *screen_main;
//	bool screen_main_del;
//	lv_obj_t *screen_main_tileview_1;
//	lv_obj_t *tileview_1_name_TempAndHumi;
//	lv_obj_t *screen_main_spangroup_humi_txt;
//	lv_obj_t *screen_main_spangroup_temp_txt;
//	lv_obj_t *screen_main_img_TempAndHumi;
//	lv_obj_t *tileview_1_name_pressure_tileview;
//	lv_obj_t *screen_main_spangroup_pressure_txt;
//	lv_obj_t *screen_main_img_pressure;
//	lv_obj_t *tileview_1_name_tvoc_tileview;
//	lv_obj_t *screen_main_label_tvoc_value;
//	lv_obj_t *screen_main_spangroup_tvoc_txt;
//	lv_obj_t *screen_main_img_7;
	lv_obj_t *screen_set;
	bool screen_set_del;
	lv_obj_t *screen_set_canvas_light;
	lv_obj_t *screen_set_canvas_server;
	lv_obj_t *screen_set_label_1;
	lv_obj_t *screen_set_bar_light;//亮度的进度条
	lv_obj_t *screen_set_label_2;
	lv_obj_t *screen_set_label_light;//百分比字
	lv_obj_t *screen_set_canvas_slave;
	lv_obj_t *screen_set_spangroup_1;//开启上传云端功能字
	lv_obj_t *screen_set_spangroup_6;//亮度字
	lv_obj_t *screen_set_spangroup_7;//设置字
	lv_obj_t *screen_set_canvas_About;
	lv_obj_t *screen_set_spangroup_2;//开启从机显示功能字
	lv_obj_t *screen_set_sw_server;//云端按键
	lv_obj_t *screen_set_sw_slave;//从机按键
	lv_obj_t *screen_set_spangroup_3;
	lv_obj_t *screen_set_spangroup_4;
	lv_obj_t *screen_set_spangroup_5;
	
	
	
	lv_obj_t * label_power;//电量图标
	lv_obj_t * label_set;	//设置图标
}lv_ui;

void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_set(lv_ui *ui);
LV_IMG_DECLARE(_humiture_80x83);
LV_IMG_DECLARE(_pressure_80x83);
LV_IMG_DECLARE(_TVOC_80x83);


//extern lv_span_t *screen_main_spangroup_temp_txt_span;
//extern lv_span_t *screen_main_spangroup_humi_txt_span;
//extern lv_span_t *screen_main_spangroup_pressure_txt_span;

extern lv_meter_indicator_t *screen_meter_tempandhumi_scale_1_arc_0;
extern lv_meter_indicator_t *screen_meter_tempandhumi_scale_1_scaleline_0;
extern lv_meter_indicator_t *screen_meter_pressure_scale_1_scaleline_0;
extern lv_meter_indicator_t *screen_meter_tvoc_scale_1_scaleline_0;

#ifdef __cplusplus
}
#endif
#endif