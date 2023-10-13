#include "bsp_voltage.h"
#include "bsp_usart.h"
#include "stdio.h"

void power_voltage_gpio_config(void)
{
	//ʹ������ʱ��
    rcu_periph_clock_enable(RCU_GPIOC);   
    
    //ʹ��ADCʱ��
    rcu_periph_clock_enable(RCU_ADC0);
    //����ADCʱ��
    adc_clock_config(ADC_ADCCK_HCLK_DIV5);
	
	//��������Ϊģ������ģʽ
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3); 
    
	//����ADCΪ����ģʽ      
	adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
		
	//ʹ��ɨ��ģʽ
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

	//�����Ҷ���     
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	   
	//ADC0����Ϊ12λ�ֱ���               
	adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
	
	//ADC0����Ϊ������  һ��ʹ�� 1 ��ͨ��
	adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL, 1);

	//ADC�ⲿ��������, ��ֻ��ʹ���������
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

	//ADC0ʹ��
	adc_enable(ADC0);

	//����ADC��У׼
	adc_calibration_enable(ADC0); 
}

/**********************************************************
 * �� �� �� �ƣ�get_voltage_value
 * �� �� �� �ܣ���ȡ��ѹֵ
 * �� �� �� ����ADC_CHANNEL_x=Ҫ�ɼ���ͨ��
 * �� �� �� �أ��������ĵ�ѹ�ٷֱ�
 * ��       �ߣ�LC
 * ��       ע��PC3 = ADC_CHANNEL_13
**********************************************************/
float get_voltage_value(void)
{     
    unsigned int adc_value = 0;
    float voltage = 0;
    //���òɼ�ͨ��
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_15);
    //��ʼ���ת��
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    //�ȴ� ADC0 ������� 
    while ( adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET ) 
    {
        ;
    }
    //��ȡ����ֵ
    adc_value = adc_regular_data_read(ADC0);
    if( adc_value == 0 ) return 0;
    //�����ѹ��ʽ��U = ( R / R�� ) * UԴ    ��R1=7600 R2=8580 R1+R2=16180��
    //ADC��ռ�ĵ�ѹ������10K/20k=0.5  �� 7600/16180=0.47��
    //��ص������ʱ��ADC�˷�ѹ�õ��ĵ�ѹΪ��0.5*4.2V=2.1V  ��0.47*4.2=1.974��
    //��ص������ʱ��ADC�˷�ѹ�õ��ĵ�ѹΪ��0.5*3.2V=1.6V  ��0.47*3.2=1.504��
    voltage = adc_value / 4095.0 * 3.3;
//    printf("voltage = %f\r\n",voltage);
//    //����ʵ�ʵ�ѹ : �ɼ����ĵ�ѹ / 0.5 
//    printf("virtual voltage = %f\r\n",voltage / 0.5);
    
    voltage = (((voltage-1.6) / 0.5) * 100.0);
    //��ѹ�ٷֱ� = ��ǰֵ / ��ֵ * 100
//    printf("Percentage voltage = %f\r\n",voltage);
    //���ز���ֵ
    return voltage;
}


//extern lv_ui gui_ui;

//��ص�����ʾ
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