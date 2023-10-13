 /********************************************************************************
   * ����Ӳ������������ɽ�ɿ�����GD32F470ZGT6    ʹ����Ƶ200Mhz    ����25Mhz
   * �� �� ��: V1.0
   * �޸�����: LC
   * �޸�����: 2023��06��12��
   * ���ܽ���:      
   ******************************************************************************
   * ��ɽ����Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ  
   * �����������www.lckfb.com   
   * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ  
   * ������̳��club.szlcsc.com   
   * ����ģ����ֲ�ֲ᣺����������ɽ�ɿ����塿ģ����ֲ�ֲ�
   * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
   * ��������׬Ǯ���������й�����ʦΪ����
 *********************************************************************************/


#include "lcdinit.h"
#include "lvgl.h"
#include "bsp_key.h"
#include "bsp_ags10.h"
#include "bsp_usart.h"
#include "stdio.h"

//LCD��ʾ������
__align(32) uint16_t Show_Gram[LCD_RAM_NUMBER]  __attribute__((at(SDRAM_DEVICE0_ADDR)));  

uint8_t show_over_flag;//�Ƿ���ʾ����
uint8_t show_update_flag;  //�Ƿ����

/**********************************************************
 * �� �� �� �ƣ�lcd_backlight_init
 * �� �� �� �ܣ�LCD������Ƴ�ʼ��
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LCKFB
 * ��       ע����
**********************************************************/
void lcd_backlight_init(void)
{
	 /*	�رն�ʱ��		*/
	 timer_deinit(TIMER1);
	/*	������Ӧ����ʱ��	*/
	rcu_periph_clock_enable(RCU_LCD_BLK);	
	/*	������ʱ��ʱ��	*/
	rcu_periph_clock_enable(RCU_TIMER1);	
	/*  ���ö�ʱ��ʱ��   CK_TIMERx = 4 x CK_APB1 = 4x50M = 200MHZ */
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	/*	��������ģʽΪ���ø������	*/
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);	
	/*	���ø����ߣ�����������ֲ�46ҳ��ʼ	*/
	gpio_af_set(PORT_LCD_BLK, GPIO_AF_1, GPIO_LCD_BLK);

	/* 	��ʱ����ʼ�������ṹ�嶨�� */
	timer_parameter_struct timer_initpara={0};
	timer_initpara.prescaler         = 10-1;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//������ʽ���ø��ݱ��ؾ���
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//����Ϊ���ϼ���
	timer_initpara.period            = 8000-1;			    //����Ƶ��=10KHz ����=0.0001s
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER1, &timer_initpara);					    //��ʱ��1��������

	/* ��ʱ��PWM��ʼ�������ṹ�嶨�� */
	timer_oc_parameter_struct timer_ocintpara={0};
	timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;	//ͨ���������Ϊ�ߵ�ƽ��Ч
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;		    //ʹ�� PWM ������˿�
	timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;	//ͨ�������������Ϊ�ߵ�ƽ
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;	    //ͨ���������״̬ʧ��
	timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;	//�ŵ�����Ŀ���״̬Ϊ��
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;	//�ŵ���������Ŀ���״̬Ϊ��

	/* ���޸ĵ��������õ���ʱ����ͨ����    */
	timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);

	/*���ö�ʱ��1ͨ��0���ռ�ձ�0 */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);
	
	/*	���ö�ʱ��ͨ�������ģʽΪPWM0ģʽ	*/
	timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);	

	/*	���ö�ʱ��1����ͨ����ֹ����Ƚ�Ӱ�ӼĴ���	*/
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	
	/* ʹ��TIMERx_CAR�Ĵ�����Ӱ�ӼĴ��� */
	timer_auto_reload_shadow_enable(TIMER1);
	/* ����ͨ�����ʹ�� */
	timer_primary_output_config(TIMER1, ENABLE);
	/* ʹ�ܶ�ʱ�� */
	timer_enable(TIMER1);
}


/**********************************************************
 * �� �� �� �ƣ�set_lcd_backlight
 * �� �� �� �ܣ�����LCD��������
 * �� �� �� ����value=���Ȱٷֱ� ��Χ0~100   100����
 * �� �� �� �أ���
 * ��       �ߣ�LCKFB
 * ��       ע����
**********************************************************/
void set_lcd_backlight(uint32_t value)
{
	uint32_t timer_value = 0;
	if( value < 10 ) value = 10; 
	if( value > 100 ) value = 100; 
	timer_value = value * ( 8000 - 1 ) / 100.0;
	
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, timer_value);
}


void dma_spi_init(void)
{
	dma_single_data_parameter_struct dma_init_struct;
    
    /* ʹ��DMA1ʱ�� */
    rcu_periph_clock_enable(RCU_DMA1);	

    /* ��ʼ��DMA1��ͨ��3 */
    dma_deinit(DMA1, DMA_CH3);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;               //�ڴ�������
    dma_init_struct.memory0_addr = (uint32_t)0;                     //�ڴ��ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;        //�����ڴ��ַ����
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;    //�ڴ����ݿ��
    dma_init_struct.number = LCD_W * LCD_H / 2;                     //������ 240*280/2 = 67200/2 = 33600
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(PORT_SPI) ;   //�����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;       //�ر������ַ����
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;             //���ȼ���
    dma_single_data_mode_init(DMA1, DMA_CH3, &dma_init_struct);     //�����ϲ�������DMA1��ͨ��3
    
    // ����DMAѭ��ģʽ
    dma_circulation_disable(DMA1, DMA_CH3);	
	
    // DMAͨ������ѡ�� 011 
    dma_channel_subperipheral_select(DMA1, DMA_CH3, DMA_SUBPERI3);
    
    // ����DMA0��������ж� 
    dma_interrupt_enable(DMA1, DMA_CH3, DMA_CHXCTL_FTFIE);
    
    // �����ж����ȼ�(����Ϊ���)
    nvic_irq_enable(DMA1_Channel3_IRQn, 0, 0);
    
    // ʧ��DMA1��ͨ��3
    dma_channel_disable(DMA1, DMA_CH3);
}


/******************************************************************
 * �� �� �� �ƣ�lcd_gpio_config
 * �� �� ˵ ������LCD���ų�ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע��ע����ʹ�����SPI����Ӳ��SPI
******************************************************************/
void lcd_gpio_config(void)
{

#if 	USE_SOFTWARE
	
	  /* ʹ��ʱ�� */
	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);
	
	
	 	/* ����SCL */
	gpio_mode_set(PORT_LCD_SCL,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SCL);
	gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET);
	
	 	/* ����SDA */
	gpio_mode_set(PORT_LCD_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SDA);
	gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET);
	
	 	/* ����DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);

	 	/* ����CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);
	
	 	/* ����RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);

	 	/* ����BLK */
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);


#else

	spi_parameter_struct spi_init_struct;

	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
//	rcu_periph_clock_enable(RCU_LCD_BLK);

	rcu_periph_clock_enable(RCU_SPI_HARDWARE);  // ʹ��SPI

		/* ���� SPI��SCK GPIO */
	gpio_af_set(PORT_LCD_SCL, LINE_AF_SPI, GPIO_LCD_SCL);
	gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
	gpio_bit_set(PORT_LCD_SCL,GPIO_LCD_SCL);
	  /* ���� SPI��MOSI  GPIO */
	gpio_af_set(PORT_LCD_SDA, LINE_AF_SPI, GPIO_LCD_SDA);
	gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);
	gpio_bit_set(PORT_LCD_SDA, GPIO_LCD_SDA);	 
	
   	/* ����DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
	 	/* ����RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
	 	/* ����BLK */
//	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
//	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
//	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);
	 	/* ����CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

    // ���� SPI ���� 
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;// ����ģʽȫ˫��
	spi_init_struct.device_mode          = SPI_MASTER;              // ����Ϊ����
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;      // 8λ����
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE; // ���Ը���λ2
	spi_init_struct.nss                  = SPI_NSS_SOFT;            // ���cs
	spi_init_struct.prescale             = SPI_PSC_2;               // 2��Ƶ
	spi_init_struct.endian               = SPI_ENDIAN_MSB;          // ��λ��ǰ
	spi_init(PORT_SPI, &spi_init_struct);

	//ʹ��DMA����
	spi_dma_enable(PORT_SPI,SPI_DMA_TRANSMIT);

	// ʹ�� SPI 
	spi_enable(PORT_SPI);

    //��ʼ��DMA
    dma_spi_init();
#endif

}

/******************************************************************
 * �� �� �� �ƣ�LCD_Writ_Bus
 * �� �� ˵ ����LCD��������д�뺯��
 * �� �� �� �Σ�dat  Ҫд��Ĵ�������
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע��ע����ʹ�����SPI����Ӳ��SPI
******************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
#if USE_SOFTWARE                               
	uint8_t i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
  LCD_CS_Set();	

#else

	LCD_CS_Clr();
    
    //�ȴ��������
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE));
	
    //��������
    spi_i2s_data_transmit(PORT_SPI, dat);
    
    //�ȴ��������
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_RBNE));
    
    //�ȴ��������������
    while(SET == spi_i2s_flag_get(PORT_SPI,I2S_FLAG_TRANS));
    
    //��������
	spi_i2s_data_receive(PORT_SPI);
			
	LCD_CS_Set();	

#endif
}
/******************************************************************
 * �� �� �� �ƣ�LCD_WR_DATA8
 * �� �� ˵ ����LCDд��8λ����
 * �� �� �� �Σ�dat д�������
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************
 * �� �� �� �ƣ�LCD_WR_DATA
 * �� �� ˵ ����LCDд��16λ����
 * �� �� �� �Σ�dat д�������
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}

/******************************************************************
 * �� �� �� �ƣ�LCD_WR_REG
 * �� �� ˵ ����LCDд������
 * �� �� �� �Σ�dat д�������
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}

/******************************************************************
 * �� �� �� �ƣ�LCD_Address_Set
 * �� �� ˵ ����������ʼ�ͽ�����ַ
 * �� �� �� �Σ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}

/******************************************************************
 * �� �� �� �ƣ�LCD_Init
 * �� �� ˵ ����LCD��ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ� LC
 * ��       ע����
******************************************************************/
void LCD_Init(void)
{
	lcd_gpio_config();//��ʼ��GPIO
	lcd_backlight_init();
	
	LCD_RES_Clr();//��λ
	delay_1ms(100);
	LCD_RES_Set();//ֹͣ��λ
	delay_1ms(100);
	
	set_lcd_backlight(50);
//	LCD_BLK_Set();//�򿪱���
	delay_1ms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //�ر�˯��ģʽ����
	delay_1ms(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);//����ɨ��˳������
	

	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);
        
	//����������ɫ�����ʽ����
	LCD_WR_REG(0x3A);		
	//8λ�������ߣ�16λ/����(RGB 5-6-5λ����)��65K-Colors, 3Ah= " 05h "
	LCD_WR_DATA8(0x05);


	LCD_WR_REG(0xB2);			
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x33); 			

	LCD_WR_REG(0xB7);			
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);			
	LCD_WR_DATA8(0x32); //Vcom=1.35V
					
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			
	LCD_WR_DATA8(0x15); //GVDD=4.8V  ��ɫ���
				
	LCD_WR_REG(0xC4);			
	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	LCD_WR_REG(0xC6);			
	LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	LCD_WR_REG(0xD0);			
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1); 

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);

	
}


//20ms����
void Lcd_Show_Time_config(void)
{
    timer_parameter_struct timer_initpara;
     nvic_irq_enable(TIMER3_IRQn, 0, 1); // �����ж����ȼ�
    rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER3);  // ��ʱ����λ

    timer_initpara.prescaler         = 200 - 1;  // Ԥ��Ƶ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // ����ģʽ
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // ��������
    timer_initpara.period            = 30000 -1; // ����
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // ʱ�ӷ�Ƶ
    timer_initpara.repetitioncounter = 0; // �ظ�������
    timer_init(TIMER3,&timer_initpara);
    
    timer_master_output_trigger_source_select(TIMER3,TIMER_TRI_OUT_SRC_UPDATE);
    timer_interrupt_enable(TIMER3,TIMER_INT_UP);  // �ж�ʹ��
    timer_enable(TIMER3);
}


void Lcd_Gram_Fill(uint16_t color)
{
    uint32_t i;
    
    for(i=0;i<67200;i++) 
	{
        Show_Gram[i] = color;
	}
}

//��ʼ��ʾ
void LCD_Show_Gram(void)
{
    //���ñ�־λΪδ��ʾ���״̬
    show_over_flag=1;
    
    //������ʾ��Χ
    LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    
    LCD_CS_Clr();
		
	//���ȫ���жϱ�־λ(�������ͨ��3��ȫ���жϱ�־λ)
    DMA_INTC0(DMA1) = 0xfffffff;
	
    //���ô������ݴ�С
    DMA_CHCNT(DMA1, DMA_CH3) = 33600;
	
    //���ô����ַ
	DMA_CH3M0ADDR(DMA1) = (uint32_t)Show_Gram;
	
    //��ʼ����
    DMA_CHCTL(DMA1, DMA_CH3) |= DMA_CHXCTL_CHEN;
}

//��ȡ һ֡�Ƿ���ʾ��ɱ�־λ  =0��� =1δ���
uint8_t get_show_over_flag(void)
{
    return show_over_flag;
}
//���� һ֡�Ƿ���ʾ��ɱ�־λ
void set_show_over_flag(uint8_t flag)
{
    show_over_flag = flag;
}
//��ȡ �Ƿ���Ҫ���±�־λ  =1��Ҫ���� =0δ��Ҫ����
uint8_t get_show_update_flag(void)
{
    return show_update_flag;
}
//���� �Ƿ���Ҫ���±�־λ
void set_show_update_flag(uint8_t flag)
{
    show_update_flag = flag;
}
//��Ļ�������
void DMA1_Channel3_IRQHandler(void)
{
    static uint8_t Show_Number=0;
		
		//ȫ��Ļ��Ҫ����4��
    if((++Show_Number) < 4)
	{
        //���ȫ��DMA1�жϱ�־λ	 
        DMA_INTC0(DMA1) = 0xfffffff;		
        //�������Ҫ���˵�������	
        DMA_CHCNT(DMA1, DMA_CH3) = 33600;
        //�ڴ���˵�ַ
        DMA_CH3M0ADDR(DMA1) = (uint32_t)Show_Gram+33600*Show_Number;
        //��������
        DMA_CHCTL(DMA1, DMA_CH3) |= DMA_CHXCTL_CHEN;
    }
    else
    {
		//���DMA��������жϱ�־λ
        dma_interrupt_flag_clear(DMA1, DMA_CH3, DMA_INT_FLAG_FTF);
		//�ȴ�SPI�������
        while(SPI_STAT(PORT_SPI) & SPI_STAT_TRANS);
		//SPIƬѡ����
        LCD_CS_Set();
		//���˴�������
        Show_Number=0;
		//һ֡������ɱ�־λ
        show_over_flag=0;
    }
}


/******************************************************************
 * �� �� �� �ƣ�TIMER3_IRQHandler
 * �� �� ˵ ������ʱ��3�жϷ�����
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LCKFB
 * ��       ע��LCD����ʾBUFF�л� �� ����ɨ��
******************************************************************/
void TIMER3_IRQHandler(void)
{   
    //����жϱ�־λ
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
        
    //�����Ļ��ʾ����DMA�������
    if(show_update_flag)
    {
        //�����ɱ�־
        show_update_flag=0;
        //������ʾ
        LCD_Show_Gram();
    }
    
    
    //��ʱ��ѭ���̶�����ˢ��
    if( get_show_update_flag() == 0 )
    {
		
        if(get_show_over_flag() == 0){
            set_show_update_flag(1);
        }
    }   
    
    //�к�����ɼ�ʱ���ж�
    tvoc_measured();
    
    //���ü�ɨ��
    set_key_scan();

    //���ɨ��
    left_key_scan();
    
    //�Ҽ�ɨ��
    right_key_scan();
	
	//���ؼ�ɨ��
	return_key_scan();
	
	
}