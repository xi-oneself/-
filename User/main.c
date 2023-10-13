#include "gd32f4xx.h"
#include "systick.h"
#include "bsp_led.h"
#include "lcdinit.h"
#include "lcdgui.h"
#include "bsp_usart.h"
#include "bsp_aht21.h"
#include "stdio.h"
#include "bsp_ags10.h"
#include "bsp_wf183d.h"
#include "string.h"
#include "bsp_voltage.h"
#include "bsp_key.h"
#include "bsp_hc05.h"

#include "lv_conf.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"

#include "gui_guider.h"
#include "events_init.h"
#include "guider_lv_conf.h"


lv_ui gui_ui;

//云端
//从机

/**********************************************************
 * 函 数 名 称：BKP_init
 * 函 数 功 能：开启备份域存储区
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：共有20个寄存器可以存储，RTC_BKP0~RTC_BKP19
**********************************************************/
void BKP_init(void)
{
	//使能电源管理时钟
	rcu_periph_clock_enable(RCU_PMU);
		
	//启用对备份域中寄存器的写访问
	pmu_backup_write_enable();

	//	RTC_BKP0
}


/******************************************************************
 * 函 数 名 称：main
 * 函 数 说 明：主函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
int main(void)
{
    Bluetooth_Init();
    int i = 0;
    char disp_buff[200] = {0};
    unsigned int AirPressure = 0;
    unsigned int tvoc_value = 0;
    int time = 0;
    uint8_t signal_value = 0;
    int lcd_time = 0;
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    //滴答定时器初始化 1us
    systick_config();

    //sram初始化
    exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);   
    
    //串口0初始化（调试）
    usart_gpio_config( 9600U );
    printf("start\r\n");;

    
    //温湿度传感器初始化
    aht21_gpio_init();
    
    //气压传感器初始化
    wf183d_gpio_config();
    
    //有害气体传感器初始化
    ags10_gpio_init();
    
    //按键初始化
    key_gpio_init();
    
	//开启备份域存储
	BKP_init();
	
    //初始化LVGL
	lv_init();						
    //LVGL显示接口初始化
	lv_port_disp_init();  
		
    //开启定时器固定LCD刷屏与按键扫描
	Lcd_Show_Time_config();    

    //设置UI
    setup_ui(&gui_ui);

	//亮度设置
	set_lcd_backlight(RTC_BKP0);
	//云端传输设置
	set_switch_server(RTC_BKP1);
	//从机显示设置
	set_switch_slave(RTC_BKP2);		

	while(1)
	{
		if( get_switch_slave() == SWITCH_OPEN )
				{
				//接收蓝牙数据
        char* data = Receive_Bluetooth_Data();
        if(data != NULL)
        {
            //如果接收到的数据是 "1"，打印 "one"
            if(strcmp(data, "1") == 0)
            {
							//获取气压数据
								sprintf(disp_buff, "C=%d PPB T=%.0f C H=%.0f RH P=%d PA", \
					      tvoc_value,get_temperature(),get_humidity(),AirPressure );
							//发送到手机
                Send_Bluetooth_Data(disp_buff);
               delay_1ms(10);
						}else{
							Send_Bluetooth_Data( " 输入1即可获取数据 ");
              delay_1ms(10);
						}
            //清除接收缓存
            Clear_BLERX_BUFF();
        }								
				}		
		//当前在主界面
		if( get_mode_select() == MAIN_MODE )
		{

			//读取温湿度数据
			aht21_read_data();        
			
			//显示温度			
			sprintf(disp_buff, "%.0f C", get_temperature());
			lv_label_set_text(gui_ui.screen_label_temp_val, disp_buff);
			lv_meter_set_indicator_end_value(gui_ui.screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_scaleline_0, get_temperature());
			
			//显示湿度
			sprintf(disp_buff, "%.0f %%RH", get_humidity());
			lv_label_set_text(gui_ui.screen_label_humi_val, disp_buff);
			lv_meter_set_indicator_end_value(gui_ui.screen_meter_tempandhumi, screen_meter_tempandhumi_scale_1_arc_0, get_humidity());

			//显示气压数据
			AirPressure = GetAirPressureValue();
			sprintf(disp_buff, "%d", AirPressure);
			lv_label_set_text(gui_ui.screen_label_pressure_val, disp_buff);
			lv_meter_set_indicator_end_value(gui_ui.screen_meter_pressure, screen_meter_pressure_scale_1_scaleline_0, AirPressure);
				
			//判断是否可以采集有害气体数据
			if( get_tvoc_measured_accomplish_flag() == 1 )
			{
				set_tvoc_measured_accomplish_flag(0);
				//显示有害气体数据
				tvoc_value = ags10_read();
				sprintf(disp_buff, "%d", tvoc_value);
				lv_label_set_text(gui_ui.screen_label_vtoc_val, disp_buff);
				lv_meter_set_indicator_end_value(gui_ui.screen_meter_tvoc, screen_meter_tvoc_scale_1_scaleline_0, tvoc_value);

				//如果开启从机显示开关
				
				//电压检测初始化
				power_voltage_gpio_config();	
				
				//电量图标刷新
				electric_quantity_display(&gui_ui);        
				
				//sdram重新初始化
				exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);   

			}
			
			//如果开启了上传云端开关
			if( get_switch_server() == SWITCH_OPEN )
			{
				//按一定时间上传数据到手机显示
				time++;
				if( time > 50 )
			{
					time = 0;
					//上传云端
					sprintf(disp_buff, "%.2f", get_temperature() );
					//上传云端
					sprintf(disp_buff, "%.0f", get_humidity() );

					//上传云端
					sprintf(disp_buff, "%d", AirPressure );

					//上传云端
					sprintf(disp_buff, "%d", tvoc_value );
				}
			}

			//按键判断
			all_key_scan(&gui_ui);
			//更新LVGL界面
			lv_scr_load(gui_ui.screen);     
		}
	else
		{
			//按键判断
			all_key_scan(&gui_ui);
			
			//更新LVGL界面
			lv_scr_load(gui_ui.screen_set);   

		}
        lv_task_handler();

    }
}
