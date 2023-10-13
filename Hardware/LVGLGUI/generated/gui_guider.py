# Copyright 2022 NXP
# SPDX-License-Identifier: MIT
# The auto-generated can only be used on NXP devices

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct

lv.init()
SDL.init(w=280,h=240)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(280*10)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 280
disp_drv.ver_res = 240
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init() 
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def ta_event_cb(e,kb):
    code = e.get_code()
    ta = e.get_target()
    if code == lv.EVENT.FOCUSED:
        kb.set_textarea(ta)
        kb.move_foreground()
        kb.clear_flag(lv.obj.FLAG.HIDDEN)

    if code == lv.EVENT.DEFOCUSED:
        kb.set_textarea(None)
        kb.move_background()
        kb.add_flag(lv.obj.FLAG.HIDDEN)


screen_main = lv.obj()
screen_main.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_screen_main_main_main_default
style_screen_main_main_main_default = lv.style_t()
style_screen_main_main_main_default.init()
style_screen_main_main_main_default.set_bg_color(lv.color_make(0x00,0x00,0x00))
style_screen_main_main_main_default.set_bg_opa(0)

# add style for screen_main
screen_main.add_style(style_screen_main_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_tileview_1 = lv.tileview(screen_main)
screen_main_tileview_1.set_pos(int(4),int(29))
screen_main_tileview_1.set_size(272,208)
screen_main_tileview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.ON)
tileview_1_name_TempAndHumi = screen_main_tileview_1.add_tile(0, 0, lv.DIR.RIGHT)
screen_main_spangroup_humi_txt = lv.spangroup(tileview_1_name_TempAndHumi)
screen_main_spangroup_humi_txt.set_pos(int(38),int(121))
screen_main_spangroup_humi_txt.set_size(199,28)
screen_main_spangroup_humi_txt.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_spangroup_humi_txt.set_align(lv.TEXT_ALIGN.LEFT)
screen_main_spangroup_humi_txt.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_main_spangroup_humi_txt.set_mode(lv.SPAN_MODE.BREAK)
screen_main_spangroup_humi_txt_span = screen_main_spangroup_humi_txt.new_span()
screen_main_spangroup_humi_txt_span.set_text("湿度：")
screen_main_spangroup_humi_txt_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_main_spangroup_humi_txt_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_main_spangroup_humi_txt_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_main_spangroup_humi_txt_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_main_spangroup_humi_txt_span.style.set_text_font(lv.font_montserrat_16)
screen_main_spangroup_humi_txt.refr_mode()
# create style style_screen_main_spangroup_humi_txt_main_main_default
style_screen_main_spangroup_humi_txt_main_main_default = lv.style_t()
style_screen_main_spangroup_humi_txt_main_main_default.init()
style_screen_main_spangroup_humi_txt_main_main_default.set_radius(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_humi_txt_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_humi_txt_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_spangroup_humi_txt_main_main_default.set_bg_opa(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_main_spangroup_humi_txt_main_main_default.set_border_width(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_border_opa(255)
style_screen_main_spangroup_humi_txt_main_main_default.set_pad_left(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_pad_right(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_pad_top(0)
style_screen_main_spangroup_humi_txt_main_main_default.set_pad_bottom(0)

# add style for screen_main_spangroup_humi_txt
screen_main_spangroup_humi_txt.add_style(style_screen_main_spangroup_humi_txt_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_spangroup_temp_txt = lv.spangroup(tileview_1_name_TempAndHumi)
screen_main_spangroup_temp_txt.set_pos(int(38),int(152))
screen_main_spangroup_temp_txt.set_size(199,28)
screen_main_spangroup_temp_txt.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_spangroup_temp_txt.set_align(lv.TEXT_ALIGN.LEFT)
screen_main_spangroup_temp_txt.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_main_spangroup_temp_txt.set_mode(lv.SPAN_MODE.BREAK)
screen_main_spangroup_temp_txt_span = screen_main_spangroup_temp_txt.new_span()
screen_main_spangroup_temp_txt_span.set_text("温度：")
screen_main_spangroup_temp_txt_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_main_spangroup_temp_txt_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_main_spangroup_temp_txt_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_main_spangroup_temp_txt_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_main_spangroup_temp_txt_span.style.set_text_font(lv.font_montserrat_16)
screen_main_spangroup_temp_txt.refr_mode()
# create style style_screen_main_spangroup_temp_txt_main_main_default
style_screen_main_spangroup_temp_txt_main_main_default = lv.style_t()
style_screen_main_spangroup_temp_txt_main_main_default.init()
style_screen_main_spangroup_temp_txt_main_main_default.set_radius(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_temp_txt_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_temp_txt_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_spangroup_temp_txt_main_main_default.set_bg_opa(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_main_spangroup_temp_txt_main_main_default.set_border_width(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_border_opa(255)
style_screen_main_spangroup_temp_txt_main_main_default.set_pad_left(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_pad_right(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_pad_top(0)
style_screen_main_spangroup_temp_txt_main_main_default.set_pad_bottom(0)

# add style for screen_main_spangroup_temp_txt
screen_main_spangroup_temp_txt.add_style(style_screen_main_spangroup_temp_txt_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_img_TempAndHumi = lv.img(tileview_1_name_TempAndHumi)
screen_main_img_TempAndHumi.set_pos(int(97),int(29))
screen_main_img_TempAndHumi.set_size(80,83)
screen_main_img_TempAndHumi.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_img_TempAndHumi.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp898886495.png','rb') as f:
        screen_main_img_TempAndHumi_img_data = f.read()
except:
    print('Could not open C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp898886495.png')
    sys.exit()

screen_main_img_TempAndHumi_img = lv.img_dsc_t({
  'data_size': len(screen_main_img_TempAndHumi_img_data),
  'header': {'always_zero': 0, 'w': 80, 'h': 83, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': screen_main_img_TempAndHumi_img_data
})

screen_main_img_TempAndHumi.set_src(screen_main_img_TempAndHumi_img)
screen_main_img_TempAndHumi.set_pivot(0,0)
screen_main_img_TempAndHumi.set_angle(0)
# create style style_screen_main_img_tempandhumi_main_main_default
style_screen_main_img_tempandhumi_main_main_default = lv.style_t()
style_screen_main_img_tempandhumi_main_main_default.init()
style_screen_main_img_tempandhumi_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_main_img_tempandhumi_main_main_default.set_img_recolor_opa(0)
style_screen_main_img_tempandhumi_main_main_default.set_img_opa(255)

# add style for screen_main_img_TempAndHumi
screen_main_img_TempAndHumi.add_style(style_screen_main_img_tempandhumi_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

tileview_1_name_pressure_tileview = screen_main_tileview_1.add_tile(1 , 0, lv.DIR.LEFT | lv.DIR.RIGHT)
screen_main_spangroup_pressure_txt = lv.spangroup(tileview_1_name_pressure_tileview)
screen_main_spangroup_pressure_txt.set_pos(int(30),int(123))
screen_main_spangroup_pressure_txt.set_size(214,32)
screen_main_spangroup_pressure_txt.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_spangroup_pressure_txt.set_align(lv.TEXT_ALIGN.LEFT)
screen_main_spangroup_pressure_txt.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_main_spangroup_pressure_txt.set_mode(lv.SPAN_MODE.BREAK)
screen_main_spangroup_pressure_txt_span = screen_main_spangroup_pressure_txt.new_span()
screen_main_spangroup_pressure_txt_span.set_text("气压：")
screen_main_spangroup_pressure_txt_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_main_spangroup_pressure_txt_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_main_spangroup_pressure_txt_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_main_spangroup_pressure_txt_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_main_spangroup_pressure_txt_span.style.set_text_font(lv.font_montserrat_16)
screen_main_spangroup_pressure_txt.refr_mode()
# create style style_screen_main_spangroup_pressure_txt_main_main_default
style_screen_main_spangroup_pressure_txt_main_main_default = lv.style_t()
style_screen_main_spangroup_pressure_txt_main_main_default.init()
style_screen_main_spangroup_pressure_txt_main_main_default.set_radius(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_pressure_txt_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_pressure_txt_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_spangroup_pressure_txt_main_main_default.set_bg_opa(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_main_spangroup_pressure_txt_main_main_default.set_border_width(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_border_opa(255)
style_screen_main_spangroup_pressure_txt_main_main_default.set_pad_left(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_pad_right(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_pad_top(0)
style_screen_main_spangroup_pressure_txt_main_main_default.set_pad_bottom(0)

# add style for screen_main_spangroup_pressure_txt
screen_main_spangroup_pressure_txt.add_style(style_screen_main_spangroup_pressure_txt_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_img_pressure = lv.img(tileview_1_name_pressure_tileview)
screen_main_img_pressure.set_pos(int(97),int(28))
screen_main_img_pressure.set_size(80,83)
screen_main_img_pressure.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_img_pressure.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp-549184953.png','rb') as f:
        screen_main_img_pressure_img_data = f.read()
except:
    print('Could not open C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp-549184953.png')
    sys.exit()

screen_main_img_pressure_img = lv.img_dsc_t({
  'data_size': len(screen_main_img_pressure_img_data),
  'header': {'always_zero': 0, 'w': 80, 'h': 83, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': screen_main_img_pressure_img_data
})

screen_main_img_pressure.set_src(screen_main_img_pressure_img)
screen_main_img_pressure.set_pivot(0,0)
screen_main_img_pressure.set_angle(0)
# create style style_screen_main_img_pressure_main_main_default
style_screen_main_img_pressure_main_main_default = lv.style_t()
style_screen_main_img_pressure_main_main_default.init()
style_screen_main_img_pressure_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_main_img_pressure_main_main_default.set_img_recolor_opa(0)
style_screen_main_img_pressure_main_main_default.set_img_opa(255)

# add style for screen_main_img_pressure
screen_main_img_pressure.add_style(style_screen_main_img_pressure_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

tileview_1_name_tvoc_tileview = screen_main_tileview_1.add_tile(2 , 0, lv.DIR.LEFT)
screen_main_label_tvoc_value = lv.label(tileview_1_name_tvoc_tileview)
screen_main_label_tvoc_value.set_pos(int(32),int(128))
screen_main_label_tvoc_value.set_size(218,27)
screen_main_label_tvoc_value.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_label_tvoc_value.set_text("0")
screen_main_label_tvoc_value.set_long_mode(lv.label.LONG.WRAP)
# create style style_screen_main_label_tvoc_value_main_main_default
style_screen_main_label_tvoc_value_main_main_default = lv.style_t()
style_screen_main_label_tvoc_value_main_main_default.init()
style_screen_main_label_tvoc_value_main_main_default.set_radius(0)
style_screen_main_label_tvoc_value_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_label_tvoc_value_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_label_tvoc_value_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_label_tvoc_value_main_main_default.set_bg_opa(0)
style_screen_main_label_tvoc_value_main_main_default.set_text_color(lv.color_make(0x00,0x00,0x00))
try:
    style_screen_main_label_tvoc_value_main_main_default.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        style_screen_main_label_tvoc_value_main_main_default.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        style_screen_main_label_tvoc_value_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_main_label_tvoc_value_main_main_default.set_text_letter_space(2)
style_screen_main_label_tvoc_value_main_main_default.set_text_line_space(0)
style_screen_main_label_tvoc_value_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_screen_main_label_tvoc_value_main_main_default.set_pad_left(0)
style_screen_main_label_tvoc_value_main_main_default.set_pad_right(0)
style_screen_main_label_tvoc_value_main_main_default.set_pad_top(0)
style_screen_main_label_tvoc_value_main_main_default.set_pad_bottom(0)

# add style for screen_main_label_tvoc_value
screen_main_label_tvoc_value.add_style(style_screen_main_label_tvoc_value_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_spangroup_tvoc_txt = lv.spangroup(tileview_1_name_tvoc_tileview)
screen_main_spangroup_tvoc_txt.set_pos(int(39),int(166))
screen_main_spangroup_tvoc_txt.set_size(194,24)
screen_main_spangroup_tvoc_txt.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_spangroup_tvoc_txt.set_align(lv.TEXT_ALIGN.LEFT)
screen_main_spangroup_tvoc_txt.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_main_spangroup_tvoc_txt.set_mode(lv.SPAN_MODE.BREAK)
screen_main_spangroup_tvoc_txt_span = screen_main_spangroup_tvoc_txt.new_span()
screen_main_spangroup_tvoc_txt_span.set_text("有害气体浓度(PA)")
screen_main_spangroup_tvoc_txt_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_main_spangroup_tvoc_txt_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_main_spangroup_tvoc_txt_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_main_spangroup_tvoc_txt_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_main_spangroup_tvoc_txt_span.style.set_text_font(lv.font_montserrat_16)
screen_main_spangroup_tvoc_txt.refr_mode()
# create style style_screen_main_spangroup_tvoc_txt_main_main_default
style_screen_main_spangroup_tvoc_txt_main_main_default = lv.style_t()
style_screen_main_spangroup_tvoc_txt_main_main_default.init()
style_screen_main_spangroup_tvoc_txt_main_main_default.set_radius(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_tvoc_txt_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_main_spangroup_tvoc_txt_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_bg_opa(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_main_spangroup_tvoc_txt_main_main_default.set_border_width(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_border_opa(255)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_pad_left(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_pad_right(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_pad_top(0)
style_screen_main_spangroup_tvoc_txt_main_main_default.set_pad_bottom(0)

# add style for screen_main_spangroup_tvoc_txt
screen_main_spangroup_tvoc_txt.add_style(style_screen_main_spangroup_tvoc_txt_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_main_img_7 = lv.img(tileview_1_name_tvoc_tileview)
screen_main_img_7.set_pos(int(97),int(29))
screen_main_img_7.set_size(80,83)
screen_main_img_7.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_main_img_7.add_flag(lv.obj.FLAG.CLICKABLE)
try:
    with open('C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp-1848818600.png','rb') as f:
        screen_main_img_7_img_data = f.read()
except:
    print('Could not open C:\\Users\\win\\Desktop\\LVGL_GUI\\Environmental_detector_add_set\\generated\\mPythonImages\\mp-1848818600.png')
    sys.exit()

screen_main_img_7_img = lv.img_dsc_t({
  'data_size': len(screen_main_img_7_img_data),
  'header': {'always_zero': 0, 'w': 80, 'h': 83, 'cf': lv.img.CF.TRUE_COLOR_ALPHA},
  'data': screen_main_img_7_img_data
})

screen_main_img_7.set_src(screen_main_img_7_img)
screen_main_img_7.set_pivot(0,0)
screen_main_img_7.set_angle(0)
# create style style_screen_main_img_7_main_main_default
style_screen_main_img_7_main_main_default = lv.style_t()
style_screen_main_img_7_main_main_default.init()
style_screen_main_img_7_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_main_img_7_main_main_default.set_img_recolor_opa(0)
style_screen_main_img_7_main_main_default.set_img_opa(255)

# add style for screen_main_img_7
screen_main_img_7.add_style(style_screen_main_img_7_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_main_tileview_1_main_main_default
style_screen_main_tileview_1_main_main_default = lv.style_t()
style_screen_main_tileview_1_main_main_default.init()
style_screen_main_tileview_1_main_main_default.set_radius(0)
style_screen_main_tileview_1_main_main_default.set_bg_color(lv.color_make(0xf6,0xf6,0xf6))
style_screen_main_tileview_1_main_main_default.set_bg_grad_color(lv.color_make(0xf6,0xf6,0xf6))
style_screen_main_tileview_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_main_tileview_1_main_main_default.set_bg_opa(255)

# add style for screen_main_tileview_1
screen_main_tileview_1.add_style(style_screen_main_tileview_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_main_tileview_1_main_scrollbar_default
style_screen_main_tileview_1_main_scrollbar_default = lv.style_t()
style_screen_main_tileview_1_main_scrollbar_default.init()
style_screen_main_tileview_1_main_scrollbar_default.set_radius(0)
style_screen_main_tileview_1_main_scrollbar_default.set_bg_color(lv.color_make(0xea,0xef,0xf3))
style_screen_main_tileview_1_main_scrollbar_default.set_bg_opa(255)

# add style for screen_main_tileview_1
screen_main_tileview_1.add_style(style_screen_main_tileview_1_main_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

screen_set = lv.obj()
screen_set.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_screen_set_main_main_default
style_screen_set_main_main_default = lv.style_t()
style_screen_set_main_main_default.init()
style_screen_set_main_main_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_set_main_main_default.set_bg_opa(0)

# add style for screen_set
screen_set.add_style(style_screen_set_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_canvas_light = lv.canvas(screen_set)
screen_set_canvas_light.set_pos(int(0),int(60))
screen_set_canvas_light.set_size(280,35)
screen_set_canvas_light.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
cbuf_screen_set_canvas_light = bytearray(280 * 35 * 4)
screen_set_canvas_light.set_buffer(cbuf_screen_set_canvas_light, 280, 35, lv.img.CF.TRUE_COLOR_ALPHA)
screen_set_canvas_light.fill_bg(lv.color_make(0xff,0xff,0xff), 255)
# create style style_screen_set_canvas_light_main_main_default
style_screen_set_canvas_light_main_main_default = lv.style_t()
style_screen_set_canvas_light_main_main_default.init()
style_screen_set_canvas_light_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_set_canvas_light_main_main_default.set_img_recolor_opa(255)

# add style for screen_set_canvas_light
screen_set_canvas_light.add_style(style_screen_set_canvas_light_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_canvas_server = lv.canvas(screen_set)
screen_set_canvas_server.set_pos(int(0),int(95))
screen_set_canvas_server.set_size(279,38)
screen_set_canvas_server.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
cbuf_screen_set_canvas_server = bytearray(279 * 38 * 4)
screen_set_canvas_server.set_buffer(cbuf_screen_set_canvas_server, 279, 38, lv.img.CF.TRUE_COLOR_ALPHA)
screen_set_canvas_server.fill_bg(lv.color_make(0xff,0xff,0xff), 255)
# create style style_screen_set_canvas_server_main_main_default
style_screen_set_canvas_server_main_main_default = lv.style_t()
style_screen_set_canvas_server_main_main_default.init()
style_screen_set_canvas_server_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_set_canvas_server_main_main_default.set_img_recolor_opa(255)

# add style for screen_set_canvas_server
screen_set_canvas_server.add_style(style_screen_set_canvas_server_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_label_1 = lv.label(screen_set)
screen_set_label_1.set_pos(int(0),int(28))
screen_set_label_1.set_size(280,26)
screen_set_label_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_label_1.set_text("设置")
screen_set_label_1.set_long_mode(lv.label.LONG.WRAP)
# create style style_screen_set_label_1_main_main_default
style_screen_set_label_1_main_main_default = lv.style_t()
style_screen_set_label_1_main_main_default.init()
style_screen_set_label_1_main_main_default.set_radius(0)
style_screen_set_label_1_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_1_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_label_1_main_main_default.set_bg_opa(187)
style_screen_set_label_1_main_main_default.set_text_color(lv.color_make(0x00,0x00,0x00))
try:
    style_screen_set_label_1_main_main_default.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        style_screen_set_label_1_main_main_default.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        style_screen_set_label_1_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_set_label_1_main_main_default.set_text_letter_space(2)
style_screen_set_label_1_main_main_default.set_text_line_space(0)
style_screen_set_label_1_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_screen_set_label_1_main_main_default.set_pad_left(0)
style_screen_set_label_1_main_main_default.set_pad_right(0)
style_screen_set_label_1_main_main_default.set_pad_top(0)
style_screen_set_label_1_main_main_default.set_pad_bottom(0)

# add style for screen_set_label_1
screen_set_label_1.add_style(style_screen_set_label_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_bar_light = lv.bar(screen_set)
screen_set_bar_light.set_pos(int(64),int(69))
screen_set_bar_light.set_size(152,18)
screen_set_bar_light.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_bar_light.set_style_anim_time(200, 0)
screen_set_bar_light.set_mode(lv.bar.MODE.NORMAL)
screen_set_bar_light.set_value(50, lv.ANIM.ON)
# create style style_screen_set_bar_light_main_main_default
style_screen_set_bar_light_main_main_default = lv.style_t()
style_screen_set_bar_light_main_main_default.init()
style_screen_set_bar_light_main_main_default.set_radius(10)
style_screen_set_bar_light_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_bar_light_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_bar_light_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_bar_light_main_main_default.set_bg_opa(60)

# add style for screen_set_bar_light
screen_set_bar_light.add_style(style_screen_set_bar_light_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_set_bar_light_main_indicator_default
style_screen_set_bar_light_main_indicator_default = lv.style_t()
style_screen_set_bar_light_main_indicator_default.init()
style_screen_set_bar_light_main_indicator_default.set_radius(10)
style_screen_set_bar_light_main_indicator_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_bar_light_main_indicator_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_bar_light_main_indicator_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_bar_light_main_indicator_default.set_bg_opa(255)

# add style for screen_set_bar_light
screen_set_bar_light.add_style(style_screen_set_bar_light_main_indicator_default, lv.PART.INDICATOR|lv.STATE.DEFAULT)

screen_set_label_2 = lv.label(screen_set)
screen_set_label_2.set_pos(int(5),int(64))
screen_set_label_2.set_size(54,28)
screen_set_label_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_label_2.set_text("亮度")
screen_set_label_2.set_long_mode(lv.label.LONG.WRAP)
# create style style_screen_set_label_2_main_main_default
style_screen_set_label_2_main_main_default = lv.style_t()
style_screen_set_label_2_main_main_default.init()
style_screen_set_label_2_main_main_default.set_radius(0)
style_screen_set_label_2_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_2_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_2_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_label_2_main_main_default.set_bg_opa(0)
style_screen_set_label_2_main_main_default.set_text_color(lv.color_make(0x00,0x00,0x00))
try:
    style_screen_set_label_2_main_main_default.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        style_screen_set_label_2_main_main_default.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        style_screen_set_label_2_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_set_label_2_main_main_default.set_text_letter_space(2)
style_screen_set_label_2_main_main_default.set_text_line_space(0)
style_screen_set_label_2_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_screen_set_label_2_main_main_default.set_pad_left(0)
style_screen_set_label_2_main_main_default.set_pad_right(0)
style_screen_set_label_2_main_main_default.set_pad_top(0)
style_screen_set_label_2_main_main_default.set_pad_bottom(0)

# add style for screen_set_label_2
screen_set_label_2.add_style(style_screen_set_label_2_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_label_light = lv.label(screen_set)
screen_set_label_light.set_pos(int(220),int(64))
screen_set_label_light.set_size(64,24)
screen_set_label_light.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_label_light.set_text("50%")
screen_set_label_light.set_long_mode(lv.label.LONG.WRAP)
# create style style_screen_set_label_light_main_main_default
style_screen_set_label_light_main_main_default = lv.style_t()
style_screen_set_label_light_main_main_default.init()
style_screen_set_label_light_main_main_default.set_radius(0)
style_screen_set_label_light_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_light_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_label_light_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_label_light_main_main_default.set_bg_opa(0)
style_screen_set_label_light_main_main_default.set_text_color(lv.color_make(0x00,0x00,0x00))
try:
    style_screen_set_label_light_main_main_default.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        style_screen_set_label_light_main_main_default.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        style_screen_set_label_light_main_main_default.set_text_font(lv.font_montserrat_16)
style_screen_set_label_light_main_main_default.set_text_letter_space(2)
style_screen_set_label_light_main_main_default.set_text_line_space(0)
style_screen_set_label_light_main_main_default.set_text_align(lv.TEXT_ALIGN.CENTER)
style_screen_set_label_light_main_main_default.set_pad_left(0)
style_screen_set_label_light_main_main_default.set_pad_right(0)
style_screen_set_label_light_main_main_default.set_pad_top(0)
style_screen_set_label_light_main_main_default.set_pad_bottom(0)

# add style for screen_set_label_light
screen_set_label_light.add_style(style_screen_set_label_light_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_canvas_slave = lv.canvas(screen_set)
screen_set_canvas_slave.set_pos(int(0),int(132))
screen_set_canvas_slave.set_size(280,35)
screen_set_canvas_slave.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
cbuf_screen_set_canvas_slave = bytearray(280 * 35 * 4)
screen_set_canvas_slave.set_buffer(cbuf_screen_set_canvas_slave, 280, 35, lv.img.CF.TRUE_COLOR_ALPHA)
screen_set_canvas_slave.fill_bg(lv.color_make(0xff,0xff,0xff), 255)
# create style style_screen_set_canvas_slave_main_main_default
style_screen_set_canvas_slave_main_main_default = lv.style_t()
style_screen_set_canvas_slave_main_main_default.init()
style_screen_set_canvas_slave_main_main_default.set_img_recolor(lv.color_make(0xff,0xff,0xff))
style_screen_set_canvas_slave_main_main_default.set_img_recolor_opa(255)

# add style for screen_set_canvas_slave
screen_set_canvas_slave.add_style(style_screen_set_canvas_slave_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_spangroup_1 = lv.spangroup(screen_set)
screen_set_spangroup_1.set_pos(int(5),int(102))
screen_set_spangroup_1.set_size(200,25)
screen_set_spangroup_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_spangroup_1.set_align(lv.TEXT_ALIGN.LEFT)
screen_set_spangroup_1.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_set_spangroup_1.set_mode(lv.SPAN_MODE.BREAK)
screen_set_spangroup_1_span = screen_set_spangroup_1.new_span()
screen_set_spangroup_1_span.set_text("开启上传云端功能")
screen_set_spangroup_1_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_set_spangroup_1_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_set_spangroup_1_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_set_spangroup_1_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_set_spangroup_1_span.style.set_text_font(lv.font_montserrat_16)
screen_set_spangroup_1.refr_mode()
# create style style_screen_set_spangroup_1_main_main_default
style_screen_set_spangroup_1_main_main_default = lv.style_t()
style_screen_set_spangroup_1_main_main_default.init()
style_screen_set_spangroup_1_main_main_default.set_radius(0)
style_screen_set_spangroup_1_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_1_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_1_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_spangroup_1_main_main_default.set_bg_opa(0)
style_screen_set_spangroup_1_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_set_spangroup_1_main_main_default.set_border_width(0)
style_screen_set_spangroup_1_main_main_default.set_border_opa(255)
style_screen_set_spangroup_1_main_main_default.set_pad_left(0)
style_screen_set_spangroup_1_main_main_default.set_pad_right(0)
style_screen_set_spangroup_1_main_main_default.set_pad_top(0)
style_screen_set_spangroup_1_main_main_default.set_pad_bottom(0)

# add style for screen_set_spangroup_1
screen_set_spangroup_1.add_style(style_screen_set_spangroup_1_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_canvas_About = lv.canvas(screen_set)
screen_set_canvas_About.set_pos(int(0),int(168))
screen_set_canvas_About.set_size(280,73)
screen_set_canvas_About.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
cbuf_screen_set_canvas_About = bytearray(280 * 73 * 4)
screen_set_canvas_About.set_buffer(cbuf_screen_set_canvas_About, 280, 73, lv.img.CF.TRUE_COLOR_ALPHA)
screen_set_canvas_About.fill_bg(lv.color_make(0x78,0xbc,0xf7), 255)
# create style style_screen_set_canvas_about_main_main_default
style_screen_set_canvas_about_main_main_default = lv.style_t()
style_screen_set_canvas_about_main_main_default.init()
style_screen_set_canvas_about_main_main_default.set_img_recolor(lv.color_make(0x78,0xbc,0xf7))
style_screen_set_canvas_about_main_main_default.set_img_recolor_opa(255)

# add style for screen_set_canvas_About
screen_set_canvas_About.add_style(style_screen_set_canvas_about_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_spangroup_2 = lv.spangroup(screen_set)
screen_set_spangroup_2.set_pos(int(5),int(137))
screen_set_spangroup_2.set_size(200,25)
screen_set_spangroup_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_spangroup_2.set_align(lv.TEXT_ALIGN.LEFT)
screen_set_spangroup_2.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_set_spangroup_2.set_mode(lv.SPAN_MODE.BREAK)
screen_set_spangroup_2_span = screen_set_spangroup_2.new_span()
screen_set_spangroup_2_span.set_text("开启从机显示功能")
screen_set_spangroup_2_span.style.set_text_color(lv.color_make(0x00,0x00,0x00))
screen_set_spangroup_2_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_set_spangroup_2_span.style.set_text_font(lv.font_simsun_24)
except AttributeError:
    try:
        screen_set_spangroup_2_span.style.set_text_font(lv.font_montserrat_24)
    except AttributeError:
        screen_set_spangroup_2_span.style.set_text_font(lv.font_montserrat_16)
screen_set_spangroup_2.refr_mode()
# create style style_screen_set_spangroup_2_main_main_default
style_screen_set_spangroup_2_main_main_default = lv.style_t()
style_screen_set_spangroup_2_main_main_default.init()
style_screen_set_spangroup_2_main_main_default.set_radius(0)
style_screen_set_spangroup_2_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_2_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_2_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_spangroup_2_main_main_default.set_bg_opa(0)
style_screen_set_spangroup_2_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_set_spangroup_2_main_main_default.set_border_width(0)
style_screen_set_spangroup_2_main_main_default.set_border_opa(255)
style_screen_set_spangroup_2_main_main_default.set_pad_left(0)
style_screen_set_spangroup_2_main_main_default.set_pad_right(0)
style_screen_set_spangroup_2_main_main_default.set_pad_top(0)
style_screen_set_spangroup_2_main_main_default.set_pad_bottom(0)

# add style for screen_set_spangroup_2
screen_set_spangroup_2.add_style(style_screen_set_spangroup_2_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_sw_server = lv.switch(screen_set)
screen_set_sw_server.set_pos(int(220),int(107))
screen_set_sw_server.set_size(40,20)
screen_set_sw_server.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_screen_set_sw_server_main_main_default
style_screen_set_sw_server_main_main_default = lv.style_t()
style_screen_set_sw_server_main_main_default.init()
style_screen_set_sw_server_main_main_default.set_radius(100)
style_screen_set_sw_server_main_main_default.set_bg_color(lv.color_make(0x0d,0x0d,0x0d))
style_screen_set_sw_server_main_main_default.set_bg_grad_color(lv.color_make(0xe6,0xe2,0xe6))
style_screen_set_sw_server_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_server_main_main_default.set_bg_opa(255)
style_screen_set_sw_server_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_server_main_main_default.set_border_width(0)
style_screen_set_sw_server_main_main_default.set_border_opa(255)

# add style for screen_set_sw_server
screen_set_sw_server.add_style(style_screen_set_sw_server_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_set_sw_server_main_indicator_checked
style_screen_set_sw_server_main_indicator_checked = lv.style_t()
style_screen_set_sw_server_main_indicator_checked.init()
style_screen_set_sw_server_main_indicator_checked.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_server_main_indicator_checked.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_server_main_indicator_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_server_main_indicator_checked.set_bg_opa(255)
style_screen_set_sw_server_main_indicator_checked.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_server_main_indicator_checked.set_border_width(0)
style_screen_set_sw_server_main_indicator_checked.set_border_opa(255)

# add style for screen_set_sw_server
screen_set_sw_server.add_style(style_screen_set_sw_server_main_indicator_checked, lv.PART.INDICATOR|lv.STATE.CHECKED)

# create style style_screen_set_sw_server_main_knob_default
style_screen_set_sw_server_main_knob_default = lv.style_t()
style_screen_set_sw_server_main_knob_default.init()
style_screen_set_sw_server_main_knob_default.set_radius(100)
style_screen_set_sw_server_main_knob_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_set_sw_server_main_knob_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_set_sw_server_main_knob_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_server_main_knob_default.set_bg_opa(255)
style_screen_set_sw_server_main_knob_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_server_main_knob_default.set_border_width(0)
style_screen_set_sw_server_main_knob_default.set_border_opa(255)

# add style for screen_set_sw_server
screen_set_sw_server.add_style(style_screen_set_sw_server_main_knob_default, lv.PART.KNOB|lv.STATE.DEFAULT)

screen_set_sw_slave = lv.switch(screen_set)
screen_set_sw_slave.set_pos(int(220),int(142))
screen_set_sw_slave.set_size(40,20)
screen_set_sw_slave.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# create style style_screen_set_sw_slave_main_main_default
style_screen_set_sw_slave_main_main_default = lv.style_t()
style_screen_set_sw_slave_main_main_default.init()
style_screen_set_sw_slave_main_main_default.set_radius(100)
style_screen_set_sw_slave_main_main_default.set_bg_color(lv.color_make(0x0d,0x0d,0x0d))
style_screen_set_sw_slave_main_main_default.set_bg_grad_color(lv.color_make(0xe6,0xe2,0xe6))
style_screen_set_sw_slave_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_slave_main_main_default.set_bg_opa(255)
style_screen_set_sw_slave_main_main_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_slave_main_main_default.set_border_width(0)
style_screen_set_sw_slave_main_main_default.set_border_opa(255)

# add style for screen_set_sw_slave
screen_set_sw_slave.add_style(style_screen_set_sw_slave_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# create style style_screen_set_sw_slave_main_indicator_checked
style_screen_set_sw_slave_main_indicator_checked = lv.style_t()
style_screen_set_sw_slave_main_indicator_checked.init()
style_screen_set_sw_slave_main_indicator_checked.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_slave_main_indicator_checked.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_slave_main_indicator_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_slave_main_indicator_checked.set_bg_opa(255)
style_screen_set_sw_slave_main_indicator_checked.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_slave_main_indicator_checked.set_border_width(0)
style_screen_set_sw_slave_main_indicator_checked.set_border_opa(255)

# add style for screen_set_sw_slave
screen_set_sw_slave.add_style(style_screen_set_sw_slave_main_indicator_checked, lv.PART.INDICATOR|lv.STATE.CHECKED)

# create style style_screen_set_sw_slave_main_knob_default
style_screen_set_sw_slave_main_knob_default = lv.style_t()
style_screen_set_sw_slave_main_knob_default.init()
style_screen_set_sw_slave_main_knob_default.set_radius(100)
style_screen_set_sw_slave_main_knob_default.set_bg_color(lv.color_make(0xff,0xff,0xff))
style_screen_set_sw_slave_main_knob_default.set_bg_grad_color(lv.color_make(0xff,0xff,0xff))
style_screen_set_sw_slave_main_knob_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_sw_slave_main_knob_default.set_bg_opa(255)
style_screen_set_sw_slave_main_knob_default.set_border_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_sw_slave_main_knob_default.set_border_width(0)
style_screen_set_sw_slave_main_knob_default.set_border_opa(255)

# add style for screen_set_sw_slave
screen_set_sw_slave.add_style(style_screen_set_sw_slave_main_knob_default, lv.PART.KNOB|lv.STATE.DEFAULT)

screen_set_spangroup_3 = lv.spangroup(screen_set)
screen_set_spangroup_3.set_pos(int(5),int(174))
screen_set_spangroup_3.set_size(254,23)
screen_set_spangroup_3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_spangroup_3.set_align(lv.TEXT_ALIGN.LEFT)
screen_set_spangroup_3.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_set_spangroup_3.set_mode(lv.SPAN_MODE.BREAK)
screen_set_spangroup_3_span = screen_set_spangroup_3.new_span()
screen_set_spangroup_3_span.set_text("关于:")
screen_set_spangroup_3_span.style.set_text_color(lv.color_make(0xff,0xff,0xff))
screen_set_spangroup_3_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_set_spangroup_3_span.style.set_text_font(lv.font_simsun_16)
except AttributeError:
    try:
        screen_set_spangroup_3_span.style.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        screen_set_spangroup_3_span.style.set_text_font(lv.font_montserrat_16)
screen_set_spangroup_3.refr_mode()
# create style style_screen_set_spangroup_3_main_main_default
style_screen_set_spangroup_3_main_main_default = lv.style_t()
style_screen_set_spangroup_3_main_main_default.init()
style_screen_set_spangroup_3_main_main_default.set_radius(0)
style_screen_set_spangroup_3_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_3_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_3_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_spangroup_3_main_main_default.set_bg_opa(0)
style_screen_set_spangroup_3_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_set_spangroup_3_main_main_default.set_border_width(0)
style_screen_set_spangroup_3_main_main_default.set_border_opa(255)
style_screen_set_spangroup_3_main_main_default.set_pad_left(0)
style_screen_set_spangroup_3_main_main_default.set_pad_right(0)
style_screen_set_spangroup_3_main_main_default.set_pad_top(0)
style_screen_set_spangroup_3_main_main_default.set_pad_bottom(0)

# add style for screen_set_spangroup_3
screen_set_spangroup_3.add_style(style_screen_set_spangroup_3_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_spangroup_4 = lv.spangroup(screen_set)
screen_set_spangroup_4.set_pos(int(13),int(197))
screen_set_spangroup_4.set_size(238,15)
screen_set_spangroup_4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_spangroup_4.set_align(lv.TEXT_ALIGN.LEFT)
screen_set_spangroup_4.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_set_spangroup_4.set_mode(lv.SPAN_MODE.BREAK)
screen_set_spangroup_4_span = screen_set_spangroup_4.new_span()
screen_set_spangroup_4_span.set_text("立创开发板：WWW.LCKFB.COM")
screen_set_spangroup_4_span.style.set_text_color(lv.color_make(0xff,0xff,0xff))
screen_set_spangroup_4_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_set_spangroup_4_span.style.set_text_font(lv.font_simsun_16)
except AttributeError:
    try:
        screen_set_spangroup_4_span.style.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        screen_set_spangroup_4_span.style.set_text_font(lv.font_montserrat_16)
screen_set_spangroup_4.refr_mode()
# create style style_screen_set_spangroup_4_main_main_default
style_screen_set_spangroup_4_main_main_default = lv.style_t()
style_screen_set_spangroup_4_main_main_default.init()
style_screen_set_spangroup_4_main_main_default.set_radius(0)
style_screen_set_spangroup_4_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_4_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_4_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_spangroup_4_main_main_default.set_bg_opa(0)
style_screen_set_spangroup_4_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_set_spangroup_4_main_main_default.set_border_width(0)
style_screen_set_spangroup_4_main_main_default.set_border_opa(255)
style_screen_set_spangroup_4_main_main_default.set_pad_left(0)
style_screen_set_spangroup_4_main_main_default.set_pad_right(0)
style_screen_set_spangroup_4_main_main_default.set_pad_top(0)
style_screen_set_spangroup_4_main_main_default.set_pad_bottom(0)

# add style for screen_set_spangroup_4
screen_set_spangroup_4.add_style(style_screen_set_spangroup_4_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_set_spangroup_5 = lv.spangroup(screen_set)
screen_set_spangroup_5.set_pos(int(13),int(218))
screen_set_spangroup_5.set_size(181,16)
screen_set_spangroup_5.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_set_spangroup_5.set_align(lv.TEXT_ALIGN.LEFT)
screen_set_spangroup_5.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_set_spangroup_5.set_mode(lv.SPAN_MODE.BREAK)
screen_set_spangroup_5_span = screen_set_spangroup_5.new_span()
screen_set_spangroup_5_span.set_text("手持环境检测仪V1.0.1")
screen_set_spangroup_5_span.style.set_text_color(lv.color_make(0xff,0xff,0xff))
screen_set_spangroup_5_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
try:
    screen_set_spangroup_5_span.style.set_text_font(lv.font_simsun_16)
except AttributeError:
    try:
        screen_set_spangroup_5_span.style.set_text_font(lv.font_montserrat_16)
    except AttributeError:
        screen_set_spangroup_5_span.style.set_text_font(lv.font_montserrat_16)
screen_set_spangroup_5.refr_mode()
# create style style_screen_set_spangroup_5_main_main_default
style_screen_set_spangroup_5_main_main_default = lv.style_t()
style_screen_set_spangroup_5_main_main_default.init()
style_screen_set_spangroup_5_main_main_default.set_radius(0)
style_screen_set_spangroup_5_main_main_default.set_bg_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_5_main_main_default.set_bg_grad_color(lv.color_make(0x21,0x95,0xf6))
style_screen_set_spangroup_5_main_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_set_spangroup_5_main_main_default.set_bg_opa(0)
style_screen_set_spangroup_5_main_main_default.set_border_color(lv.color_make(0x00,0x00,0x00))
style_screen_set_spangroup_5_main_main_default.set_border_width(0)
style_screen_set_spangroup_5_main_main_default.set_border_opa(255)
style_screen_set_spangroup_5_main_main_default.set_pad_left(0)
style_screen_set_spangroup_5_main_main_default.set_pad_right(0)
style_screen_set_spangroup_5_main_main_default.set_pad_top(0)
style_screen_set_spangroup_5_main_main_default.set_pad_bottom(0)

# add style for screen_set_spangroup_5
screen_set_spangroup_5.add_style(style_screen_set_spangroup_5_main_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)




# content from custom.py

# Load the default screen
lv.scr_load(screen_main)

while SDL.check():
    time.sleep_ms(5)
