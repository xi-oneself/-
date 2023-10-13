#include "bsp_voltage.h"
#include "bsp_usart.h"
#include "stdio.h"

void power_voltage_gpio_config(void)
{
	//使能引脚时钟
    rcu_periph_clock_enable(RCU_GPIOC);   
    
    //使能ADC时钟
    rcu_periph_clock_enable(RCU_ADC0);
    //配置ADC时钟
    adc_clock_config(ADC_ADCCK_HCLK_DIV5);
	
	//配置引脚为模拟输入模式
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3); 
    
	//配置ADC为独立模式      
	adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
		
	//使能扫描模式
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

	//数据右对齐     
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	   
	//ADC0设置为12位分辨率               
	adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
	
	//ADC0设置为规则组  一共使用 1 个通道
	adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL, 1);

	//ADC外部触发禁用, 即只能使用软件触发
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

	//ADC0使能
	adc_enable(ADC0);

	//开启ADC自校准
	adc_calibration_enable(ADC0); 
}

/**********************************************************
 * 函 数 名 称：get_voltage_value
 * 函 数 功 能：读取电压值
 * 传 入 参 数：ADC_CHANNEL_x=要采集的通道
 * 函 数 返 回：测量到的电压百分比
 * 作       者：LC
 * 备       注：PC3 = ADC_CHANNEL_13
**********************************************************/
float get_voltage_value(void)
{     
    unsigned int adc_value = 0;
    float voltage = 0;
    //设置采集通道
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_15);
    //开始软件转换
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    //等待 ADC0 采样完成 
    while ( adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET ) 
    {
        ;
    }
    //读取采样值
    adc_value = adc_regular_data_read(ADC0);
    if( adc_value == 0 ) return 0;
    //电阻分压公式：U = ( R / R总 ) * U源    （R1=7600 R2=8580 R1+R2=16180）
    //ADC端占的电压比例：10K/20k=0.5  （ 7600/16180=0.47）
    //电池电量最高时，ADC端分压得到的电压为：0.5*4.2V=2.1V  （0.47*4.2=1.974）
    //电池电量最低时，ADC端分压得到的电压为：0.5*3.2V=1.6V  （0.47*3.2=1.504）
    voltage = adc_value / 4095.0 * 3.3;
//    printf("voltage = %f\r\n",voltage);
//    //换算实际电压 : 采集到的电压 / 0.5 
//    printf("virtual voltage = %f\r\n",voltage / 0.5);
    
    voltage = (((voltage-1.6) / 0.5) * 100.0);
    //电压百分比 = 当前值 / 总值 * 100
//    printf("Percentage voltage = %f\r\n",voltage);
    //返回采样值
    return voltage;
}


//extern lv_ui gui_ui;

//电池电量显示
void electric_quantity_display(lv_ui *ui)
{
	float oltage_value = 0;
    oltage_value = get_voltage_value();

    if( oltage_value <= 25 )
    {
		lv_label_set_text(ui->label_power, LV_SYMBOL_BATTERY_1 " ");
    }
    else if( oltage_value > 25 && oltage_value <= 50 )
    {
		lv_label_set_text(ui->label_power, LV_SYMBOL_BATTERY_2 " ");
    }
    else if( oltage_value > 50 && oltage_value <= 75 )
    {
		lv_label_set_text(ui->label_power, LV_SYMBOL_BATTERY_3 " ");
    }       
    else 
    {
		lv_label_set_text(ui->label_power, LV_SYMBOL_BATTERY_FULL " ");
    } 
}