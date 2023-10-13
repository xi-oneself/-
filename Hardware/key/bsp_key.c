#include "bsp_key.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "lcdinit.h"

//上传云端开关  =1开启  =0关闭
char switch_server;
//从机显示开关  =1开启  =0关闭
char switch_slave;

//模式选择  =SET_MODE 设置界面	=MAIN_MODE 主界面
char mode_select = MAIN_MODE;

//状态 = KEY_DOWN     ：被按下
//状态 = KEY_RELEASE  ：未被按下
uint8_t set_button_state      = KEY_RELEASE;         //设置键状态
uint8_t left_button_state     = KEY_RELEASE;         //左键状态
uint8_t right_button_state    = KEY_RELEASE;         //右键状态
uint8_t return_button_state   = KEY_RELEASE;         //返回键状态


/******************************************************************
 * 函 数 名 称：key_gpio_init
 * 函 数 说 明：按键初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void key_gpio_init(void)
{
    /* 使能时钟 */
    rcu_periph_clock_enable(RCU_SET);
    rcu_periph_clock_enable(RCU_LEFT);
    rcu_periph_clock_enable(RCU_RIGHT);
    rcu_periph_clock_enable(RCU_RETURN);

    /* 配置设置键为上拉输入模式 */
    gpio_mode_set(PORT_SET,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_SET);
    gpio_bit_write(PORT_SET, GPIO_SET, SET);

    /* 配置左键为上拉输入模式 */
    gpio_mode_set(PORT_LEFT,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_LEFT);
    gpio_bit_write(PORT_LEFT, GPIO_LEFT, SET);

    /* 配置右键为上拉输入模式 */
    gpio_mode_set(PORT_RIGHT,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_RIGHT);
    gpio_bit_write(PORT_RIGHT, GPIO_RIGHT, SET);

    /* 配置返回键为上拉输入模式 */
    gpio_mode_set(PORT_RETURN,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_RETURN);
    gpio_bit_write(PORT_RETURN, GPIO_RETURN, SET); 
}

/******************************************************************
 * 函 数 名 称：set_key_scan
 * 函 数 说 明：设置键扫描函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：放在定时器中断服务函数中，10ms扫描一次
******************************************************************/
void set_key_scan(void)
{
    //超时时间
    static int outtime = 0;
    //如果设置键按下
    if( gpio_input_bit_get(PORT_SET, GPIO_SET) == RESET )
    {
        outtime++;
        //20ms * 3 = 60ms
        //按下超过50ms
        if( outtime >= 3 )
        {
            outtime = 0;
            //设置键状态设置为被按下
            set_button_state = KEY_DOWN;
        }
    }
    else
    {
        outtime = 0;
        set_button_state = KEY_RELEASE;
    }
}
//获取设置键的状态
uint8_t get_set_button_state(void)
{
    return set_button_state; 
}
//清除设置键的状态
void set_set_button_state(uint8_t state)
{
    set_button_state = state; 
}



/******************************************************************
 * 函 数 名 称：left_key_scan
 * 函 数 说 明：左键扫描函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：放在定时器中断服务函数中，10ms扫描一次
******************************************************************/
void left_key_scan(void)
{
    //超时时间
    static int outtime = 0;
    //如果设置键按下
    if( gpio_input_bit_get(PORT_LEFT, GPIO_LEFT) == RESET )
    {
        outtime++;
        //20ms * 3 = 60ms
        //按下超过60ms
        if( outtime >= 3 )
        {
            outtime = 0;
            //左键状态设置为被按下
            left_button_state = KEY_DOWN;
        }
    }
    else
    {
        outtime = 0;
        left_button_state = KEY_RELEASE;
    }
}
//获取左键的状态
uint8_t get_left_button_state(void)
{
    return left_button_state; 
}
//清楚左键的状态
void set_left_button_state(uint8_t state)
{
    left_button_state = state; 
}

/******************************************************************
 * 函 数 名 称：right_key_scan
 * 函 数 说 明：右键扫描函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：放在定时器中断服务函数中，10ms扫描一次
******************************************************************/
void right_key_scan(void)
{
    //超时时间
    static int outtime = 0;
    //如果设置键按下
    if( gpio_input_bit_get(PORT_RIGHT, GPIO_RIGHT) == RESET )
    {
        outtime++;
        //20ms * 3 = 60ms
        //按下超过60ms
        if( outtime >= 3 )
        {
            outtime = 0;
            //左键状态设置为被按下
            right_button_state = KEY_DOWN;
        }
    }
    else
    {
        outtime = 0;
        right_button_state = KEY_RELEASE;
    }
}
//获取右键的状态
uint8_t get_right_button_state(void)
{
    return right_button_state; 
}
//清楚右键的状态
void set_right_button_state(uint8_t state)
{
    right_button_state = state; 
}


/******************************************************************
 * 函 数 名 称：return_key_scan
 * 函 数 说 明：返回键扫描函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：放在定时器中断服务函数中，10ms扫描一次
******************************************************************/
void return_key_scan(void)
{
    //超时时间
    static int outtime = 0;
    //如果设置键按下
    if( gpio_input_bit_get(PORT_RETURN, GPIO_RETURN) == RESET )
    {
        outtime++;
        //20ms * 3 = 60ms
        //按下超过60ms
        if( outtime >= 3 )
        {
            outtime = 0;
            //左键状态设置为被按下
            return_button_state = KEY_DOWN;
        }
    }
    else
    {
        outtime = 0;
        return_button_state = KEY_RELEASE;
    }
}
//获取返回键的状态
uint8_t get_return_button_state(void)
{
    return return_button_state; 
}
//清除返回键的状态
void set_return_button_state(uint8_t state)
{
    return_button_state = state; 
}




//全部按键扫描
void all_key_scan(lv_ui *gui_ui)
{
    int light_val = RTC_BKP0;
	
	//0=亮度调节选项	
	//1=上传云端功能开关选项	
	//2=从机显示功能开关选项
	static char set_select_flag = 0;

    char disp_buf[10]={0};
	//当前样式
	static lv_style_t style_screen_set_spangroup_new_main_main_default;
	//之前样式
	static lv_style_t style_screen_set_spangroup_again_main_main_default;

    //左按键按下
    if( get_left_button_state() == 1 )
    {
        set_left_button_state(0);
		if( mode_select == SET_MODE )
		{
			if( set_select_flag != 0 ) 
				set_select_flag = set_select_flag - 1;
			else
				set_select_flag = 2;
		
			
			//设置背景色
			lv_style_set_bg_color(&style_screen_set_spangroup_new_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
			//设置透明度
			lv_style_set_bg_opa(&style_screen_set_spangroup_new_main_main_default, 127);
			//更新样式
			lv_obj_add_style((set_select_flag==0)?gui_ui->screen_set_spangroup_6:(set_select_flag==1)?gui_ui->screen_set_spangroup_1:gui_ui->screen_set_spangroup_2,\
							&style_screen_set_spangroup_new_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
					
			//设置透明度
			lv_style_set_bg_opa(&style_screen_set_spangroup_again_main_main_default, 0);
			//更新样式
			lv_obj_add_style((set_select_flag==0)?gui_ui->screen_set_spangroup_1:(set_select_flag==1)?gui_ui->screen_set_spangroup_2:gui_ui->screen_set_spangroup_6,\
							&style_screen_set_spangroup_again_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
		}		
	}
    //右按键按下
     if( get_right_button_state() == 1 )
    {
        set_right_button_state(0);
		if( mode_select == SET_MODE )
		{
			set_select_flag = ( set_select_flag + 1 ) % 3;

			//设置背景色
			lv_style_set_bg_color(&style_screen_set_spangroup_new_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
			//设置透明度
			lv_style_set_bg_opa(&style_screen_set_spangroup_new_main_main_default, 127);
			//更新样式
			lv_obj_add_style((set_select_flag==0)?gui_ui->screen_set_spangroup_6:(set_select_flag==1)?gui_ui->screen_set_spangroup_1:gui_ui->screen_set_spangroup_2,\
							&style_screen_set_spangroup_new_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

			//设置透明度
			lv_style_set_bg_opa(&style_screen_set_spangroup_again_main_main_default, 0);
			//更新样式
			lv_obj_add_style((set_select_flag==0)?gui_ui->screen_set_spangroup_2:(set_select_flag==1)?gui_ui->screen_set_spangroup_6:gui_ui->screen_set_spangroup_1,\
							&style_screen_set_spangroup_again_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
		}
	}   
    
	//设置键按下
    if( get_set_button_state() == 1 )
	{
        set_set_button_state(0);
		
		lv_disp_t * d = lv_obj_get_disp(lv_scr_act());
		if (d->prev_scr == NULL && d->scr_to_load == NULL)
		{
			//当前在主界面
			if( mode_select == MAIN_MODE )
			{
				//切换设置界面
				if (gui_ui->screen_set_del == true)
					setup_scr_screen_set(gui_ui);
				lv_scr_load_anim(gui_ui->screen_set, LV_SCR_LOAD_ANIM_OVER_TOP, 0, 0, true);
				gui_ui->screen_del = true;
				mode_select = SET_MODE;
				//设置界面显示内容初始化
				set_interface_disp_init(gui_ui);
			}
			else//当前在设置界面
			{
				//切换主界面
				if (gui_ui->screen_del == true)
					setup_scr_screen(gui_ui);
				lv_scr_load_anim(gui_ui->screen, LV_SCR_LOAD_ANIM_OVER_TOP, 0, 0, true);
				gui_ui->screen_set_del = true;
				mode_select = MAIN_MODE;
			}
		}
	}
	
	//返回键按下
	if( get_return_button_state() == 1 )
	{
        set_return_button_state(0);
		if( mode_select == SET_MODE )
		{
			switch( set_select_flag )
			{
				case 0://亮度调整
					//亮度值自增
					light_val = ( light_val + 10 ) % 110;
					//滑动条根据亮度值进行变化
					lv_bar_set_value(gui_ui->screen_set_bar_light, light_val, LV_ANIM_ON);
					//屏幕显示设置的亮度值
					sprintf(disp_buf, "%d%%",light_val);
					lv_label_set_text(gui_ui->screen_set_label_light, disp_buf);
					//设置LCD亮度
					set_lcd_backlight(light_val);
						
					//将数据保存到备份域0
					RTC_BKP0 = light_val;				
					break;
				
				case 1://云端上传开关
					//如果当前是打开状态
					if( lv_obj_has_state(gui_ui->screen_set_sw_server, LV_STATE_CHECKED) == true )
					{
						//设置云端上传开关为关闭状态
						lv_obj_clear_state(gui_ui->screen_set_sw_server, LV_STATE_CHECKED);	
						switch_server = SWITCH_CLOSE;
						//将数据保存到备份域1
						RTC_BKP1 = switch_server;
					}
					else//如果当前是关闭状态
					{
						//设置云端上传开关为选中状态
						lv_obj_add_state(gui_ui->screen_set_sw_server, LV_STATE_CHECKED);
						switch_server = SWITCH_OPEN;
						//将数据保存到备份域1
						RTC_BKP1 = switch_server;
					}
					break;
					
				case 2://从机显示开关
					//如果当前是打开状态
					if( lv_obj_has_state(gui_ui->screen_set_sw_slave, LV_STATE_CHECKED) == true )
					{
						//设置从机显示开关为关闭状态
						lv_obj_clear_state(gui_ui->screen_set_sw_slave, LV_STATE_CHECKED);	
						switch_slave = SWITCH_CLOSE;
						//将数据保存到备份域2
						RTC_BKP2 = switch_slave;
					}
					else//如果当前是关闭状态
					{
						//设置从机显示开关为选中状态
						lv_obj_add_state(gui_ui->screen_set_sw_slave, LV_STATE_CHECKED);
						switch_slave = SWITCH_OPEN;
						//将数据保存到备份域2
						RTC_BKP2 = switch_slave;		
					}				
					break;
			}
		}
	}
}
//获取当前模式
char get_mode_select(void)
{
	return mode_select;
}
//设置当前模式
void set_mode_select(char flag)
{
	mode_select = flag;
}

//获取上传云端开关的状态
char get_switch_server(void)
{
	return switch_server;
}
//设置上传云端开关的状态
void set_switch_server(char flag)
{
	switch_server = flag;
}

//获取从机显示开关的状态
char get_switch_slave(void)
{
	return switch_slave;
}
//设置从机显示开关的状态
void set_switch_slave(char flag)
{
	switch_slave = flag;
}


/**********************************************************
 * 函 数 名 称：set_interface_disp_init
 * 函 数 功 能：设置界面显示初始化
 * 传 入 参 数：gui_ui：控件对象结构体
 * 函 数 返 回：无
 * 作       者：LCKFB
* 备       注：根据备份域存储的设置内容，修改UI
**********************************************************/

void set_interface_disp_init(lv_ui *gui_ui)
{
	char disp_buf[10];
	
	//滑动条根据备份域0进行变化
	lv_bar_set_value(gui_ui->screen_set_bar_light, RTC_BKP0, LV_ANIM_ON);
	//屏幕显示设置的亮度值
	sprintf(disp_buf, "%d%%",RTC_BKP0);
	lv_label_set_text(gui_ui->screen_set_label_light, disp_buf);
	
	//备份域1
	if( RTC_BKP1 == SWITCH_OPEN )
	{
		lv_obj_add_state(gui_ui->screen_set_sw_server, LV_STATE_CHECKED);
	}
	else
	{
		lv_obj_clear_state(gui_ui->screen_set_sw_server, LV_STATE_CHECKED);	
	}
	
	//备份域2
	
	if( RTC_BKP2 == SWITCH_OPEN )
	{
		lv_obj_add_state(gui_ui->screen_set_sw_slave, LV_STATE_CHECKED);
	}
	else
	{
		lv_obj_clear_state(gui_ui->screen_set_sw_slave, LV_STATE_CHECKED);	
	}
	
		
	
}