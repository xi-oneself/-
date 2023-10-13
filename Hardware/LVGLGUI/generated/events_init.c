/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

void events_init(lv_ui *ui)
{
}
static void screen_set_sw_server_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
//		lv_obj_set_width(gui_ui.screen_set_spangroup_7, 50);
	}
		break;
	default:
		break;
	}
}

void events_init_screen_set(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_set_sw_server, screen_set_sw_server_event_handler, LV_EVENT_ALL, NULL);
}
