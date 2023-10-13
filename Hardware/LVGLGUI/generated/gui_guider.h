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
	lv_obj_t *screen_label_3;//������-1
	lv_obj_t *screen_label_4;//������-2
	lv_obj_t *screen_label_5;//������-3
	lv_obj_t *screen_meter_tempandhumi;//��ʪ��ͼ
	lv_obj_t *screen_meter_pressure;//��ѹͼ
	lv_obj_t *screen_meter_tvoc;//�к�����ͼ
	lv_obj_t *screen_spangroup_1;//��ʪ�ȣ��֣�
	lv_obj_t *screen_spangroup_2;//��ѹ���֣�
	lv_obj_t *screen_spangroup_3;//�к����壨�֣�
	lv_obj_t *screen_label_temp_val;//��ʪ��ֵ
	lv_obj_t *screen_label_pressure_val;//��ѹֵ
	lv_obj_t *screen_label_vtoc_val;//�к�����ֵ
	lv_obj_t *screen_label_1;//��ѹ��λ
	lv_obj_t *screen_label_2;//�к����嵥λ
	lv_obj_t *screen_label_humi_val;//ʪ�ȵ�λ
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
	lv_obj_t *screen_set_bar_light;//���ȵĽ�����
	lv_obj_t *screen_set_label_2;
	lv_obj_t *screen_set_label_light;//�ٷֱ���
	lv_obj_t *screen_set_canvas_slave;
	lv_obj_t *screen_set_spangroup_1;//�����ϴ��ƶ˹�����
	lv_obj_t *screen_set_spangroup_6;//������
	lv_obj_t *screen_set_spangroup_7;//������
	lv_obj_t *screen_set_canvas_About;
	lv_obj_t *screen_set_spangroup_2;//�����ӻ���ʾ������
	lv_obj_t *screen_set_sw_server;//�ƶ˰���
	lv_obj_t *screen_set_sw_slave;//�ӻ�����
	lv_obj_t *screen_set_spangroup_3;
	lv_obj_t *screen_set_spangroup_4;
	lv_obj_t *screen_set_spangroup_5;
	
	
	
	lv_obj_t * label_power;//����ͼ��
	lv_obj_t * label_set;	//����ͼ��
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