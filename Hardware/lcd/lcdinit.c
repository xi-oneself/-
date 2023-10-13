 /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LC
   * 修改日期: 2023年06月12日
   * 功能介绍:      
   ******************************************************************************
   * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源  
   * 开发板官网：www.lckfb.com   
   * 技术支持常驻论坛，任何技术问题欢迎随时交流学习  
   * 立创论坛：club.szlcsc.com   
   * 其余模块移植手册：【立创·梁山派开发板】模块移植手册
   * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
   * 不靠卖板赚钱，以培养中国工程师为己任
 *********************************************************************************/


#include "lcdinit.h"
#include "lvgl.h"
#include "bsp_key.h"
#include "bsp_ags10.h"
#include "bsp_usart.h"
#include "stdio.h"

//LCD显示缓冲区
__align(32) uint16_t Show_Gram[LCD_RAM_NUMBER]  __attribute__((at(SDRAM_DEVICE0_ADDR)));  

uint8_t show_over_flag;//是否显示结束
uint8_t show_update_flag;  //是否更新

/**********************************************************
 * 函 数 名 称：lcd_backlight_init
 * 函 数 功 能：LCD背光控制初始化
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
**********************************************************/
void lcd_backlight_init(void)
{
	 /*	关闭定时器		*/
	 timer_deinit(TIMER1);
	/*	开启对应引脚时钟	*/
	rcu_periph_clock_enable(RCU_LCD_BLK);	
	/*	开启定时器时钟	*/
	rcu_periph_clock_enable(RCU_TIMER1);	
	/*  配置定时器时钟   CK_TIMERx = 4 x CK_APB1 = 4x50M = 200MHZ */
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	/*	设置引脚模式为复用浮空输出	*/
	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_BLK);
	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);	
	/*	设置复用线，具体见数据手册46页开始	*/
	gpio_af_set(PORT_LCD_BLK, GPIO_AF_1, GPIO_LCD_BLK);

	/* 	定时器初始化参数结构体定义 */
	timer_parameter_struct timer_initpara={0};
	timer_initpara.prescaler         = 10-1;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//触发方式设置根据边沿决定
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//设置为向上计数
	timer_initpara.period            = 8000-1;			    //设置频率=10KHz 周期=0.0001s
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER1, &timer_initpara);					    //定时器1更新配置

	/* 定时器PWM初始化参数结构体定义 */
	timer_oc_parameter_struct timer_ocintpara={0};
	timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;	//通道输出极性为高电平有效
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;		    //使能 PWM 输出到端口
	timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;	//通道互补输出极性为高电平
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;	    //通道互补输出状态失能
	timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;	//信道输出的空闲状态为低
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;	//信道互补输出的空闲状态为低

	/* 将修改的内容设置到定时器的通道上    */
	timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);

	/*设置定时器1通道0输出占空比0 */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);
	
	/*	设置定时器通道的输出模式为PWM0模式	*/
	timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);	

	/*	设置定时器1所有通道禁止输出比较影子寄存器	*/
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	
	/* 使能TIMERx_CAR寄存器的影子寄存器 */
	timer_auto_reload_shadow_enable(TIMER1);
	/* 所有通道输出使能 */
	timer_primary_output_config(TIMER1, ENABLE);
	/* 使能定时器 */
	timer_enable(TIMER1);
}


/**********************************************************
 * 函 数 名 称：set_lcd_backlight
 * 函 数 功 能：设置LCD背光亮度
 * 传 入 参 数：value=亮度百分比 范围0~100   100最亮
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
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
    
    /* 使能DMA1时钟 */
    rcu_periph_clock_enable(RCU_DMA1);	

    /* 初始化DMA1的通道3 */
    dma_deinit(DMA1, DMA_CH3);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;               //内存往外设
    dma_init_struct.memory0_addr = (uint32_t)0;                     //内存地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;        //开启内存地址增量
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;    //内存数据宽度
    dma_init_struct.number = LCD_W * LCD_H / 2;                     //数据量 240*280/2 = 67200/2 = 33600
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(PORT_SPI) ;   //外设地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;       //关闭外设地址增量
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;             //优先级高
    dma_single_data_mode_init(DMA1, DMA_CH3, &dma_init_struct);     //将以上参数填入DMA1的通道3
    
    // 禁用DMA循环模式
    dma_circulation_disable(DMA1, DMA_CH3);	
	
    // DMA通道外设选择 011 
    dma_channel_subperipheral_select(DMA1, DMA_CH3, DMA_SUBPERI3);
    
    // 启用DMA0传输完成中断 
    dma_interrupt_enable(DMA1, DMA_CH3, DMA_CHXCTL_FTFIE);
    
    // 配置中断优先级(必须为最高)
    nvic_irq_enable(DMA1_Channel3_IRQn, 0, 0);
    
    // 失能DMA1的通道3
    dma_channel_disable(DMA1, DMA_CH3);
}


/******************************************************************
 * 函 数 名 称：lcd_gpio_config
 * 函 数 说 明：对LCD引脚初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：注意是使用软件SPI还是硬件SPI
******************************************************************/
void lcd_gpio_config(void)
{

#if 	USE_SOFTWARE
	
	  /* 使能时钟 */
	rcu_periph_clock_enable(RCU_LCD_SCL);
	rcu_periph_clock_enable(RCU_LCD_SDA);
	rcu_periph_clock_enable(RCU_LCD_CS);
	rcu_periph_clock_enable(RCU_LCD_DC);
	rcu_periph_clock_enable(RCU_LCD_RES);
	rcu_periph_clock_enable(RCU_LCD_BLK);
	
	
	 	/* 配置SCL */
	gpio_mode_set(PORT_LCD_SCL,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SCL);
	gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET);
	
	 	/* 配置SDA */
	gpio_mode_set(PORT_LCD_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_SDA);
	gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET);
	
	 	/* 配置DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);

	 	/* 配置CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);
	
	 	/* 配置RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);

	 	/* 配置BLK */
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

	rcu_periph_clock_enable(RCU_SPI_HARDWARE);  // 使能SPI

		/* 配置 SPI的SCK GPIO */
	gpio_af_set(PORT_LCD_SCL, LINE_AF_SPI, GPIO_LCD_SCL);
	gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SCL);
	gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
	gpio_bit_set(PORT_LCD_SCL,GPIO_LCD_SCL);
	  /* 配置 SPI的MOSI  GPIO */
	gpio_af_set(PORT_LCD_SDA, LINE_AF_SPI, GPIO_LCD_SDA);
	gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_LCD_SDA);
	gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);
	gpio_bit_set(PORT_LCD_SDA, GPIO_LCD_SDA);	 
	
   	/* 配置DC */
	gpio_mode_set(PORT_LCD_DC,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_DC);
	gpio_output_options_set(PORT_LCD_DC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_DC);
	gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET);
	 	/* 配置RES */
	gpio_mode_set(PORT_LCD_RES,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_LCD_RES);
	gpio_output_options_set(PORT_LCD_RES,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_RES);
	gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET);
	 	/* 配置BLK */
//	gpio_mode_set(PORT_LCD_BLK, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LCD_BLK);
//	gpio_output_options_set(PORT_LCD_BLK, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_BLK);
//	gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET);
	 	/* 配置CS */
	gpio_mode_set(PORT_LCD_CS,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_LCD_CS);
	gpio_output_options_set(PORT_LCD_CS,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_LCD_CS);
	gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET);

    // 配置 SPI 参数 
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;// 传输模式全双工
	spi_init_struct.device_mode          = SPI_MASTER;              // 配置为主机
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;      // 8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE; // 极性高相位2
	spi_init_struct.nss                  = SPI_NSS_SOFT;            // 软件cs
	spi_init_struct.prescale             = SPI_PSC_2;               // 2分频
	spi_init_struct.endian               = SPI_ENDIAN_MSB;          // 高位在前
	spi_init(PORT_SPI, &spi_init_struct);

	//使能DMA发送
	spi_dma_enable(PORT_SPI,SPI_DMA_TRANSMIT);

	// 使能 SPI 
	spi_enable(PORT_SPI);

    //初始化DMA
    dma_spi_init();
#endif

}

/******************************************************************
 * 函 数 名 称：LCD_Writ_Bus
 * 函 数 说 明：LCD串行数据写入函数
 * 函 数 形 参：dat  要写入的串行数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：注意是使用软件SPI还是硬件SPI
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
    
    //等待发送完成
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_TBE));
	
    //发送数据
    spi_i2s_data_transmit(PORT_SPI, dat);
    
    //等待接收完成
	while(RESET == spi_i2s_flag_get(PORT_SPI, SPI_FLAG_RBNE));
    
    //等待缓冲区传输完成
    while(SET == spi_i2s_flag_get(PORT_SPI,I2S_FLAG_TRANS));
    
    //接收数据
	spi_i2s_data_receive(PORT_SPI);
			
	LCD_CS_Set();	

#endif
}
/******************************************************************
 * 函 数 名 称：LCD_WR_DATA8
 * 函 数 说 明：LCD写入8位数据
 * 函 数 形 参：dat 写入的数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************
 * 函 数 名 称：LCD_WR_DATA
 * 函 数 说 明：LCD写入16位数据
 * 函 数 形 参：dat 写入的数据
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}

/******************************************************************
 * 函 数 名 称：LCD_WR_REG
 * 函 数 说 明：LCD写入命令
 * 函 数 形 参：dat 写入的命令
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}

/******************************************************************
 * 函 数 名 称：LCD_Address_Set
 * 函 数 说 明：设置起始和结束地址
 * 函 数 形 参：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

/******************************************************************
 * 函 数 名 称：LCD_Init
 * 函 数 说 明：LCD初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者： LC
 * 备       注：无
******************************************************************/
void LCD_Init(void)
{
	lcd_gpio_config();//初始化GPIO
	lcd_backlight_init();
	
	LCD_RES_Clr();//复位
	delay_1ms(100);
	LCD_RES_Set();//停止复位
	delay_1ms(100);
	
	set_lcd_backlight(50);
//	LCD_BLK_Set();//打开背光
	delay_1ms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //关闭睡眠模式命令
	delay_1ms(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);//设置扫描顺序命令
	

	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);
        
	//设置数据颜色编码格式命令
	LCD_WR_REG(0x3A);		
	//8位数据总线，16位/像素(RGB 5-6-5位输入)，65K-Colors, 3Ah= " 05h "
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
	LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度
				
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


//20ms周期
void Lcd_Show_Time_config(void)
{
    timer_parameter_struct timer_initpara;
     nvic_irq_enable(TIMER3_IRQn, 0, 1); // 设置中断优先级
    rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER3);  // 定时器复位

    timer_initpara.prescaler         = 200 - 1;  // 预分频
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // 对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // 计数方向
    timer_initpara.period            = 30000 -1; // 周期
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // 时钟分频
    timer_initpara.repetitioncounter = 0; // 重复计数器
    timer_init(TIMER3,&timer_initpara);
    
    timer_master_output_trigger_source_select(TIMER3,TIMER_TRI_OUT_SRC_UPDATE);
    timer_interrupt_enable(TIMER3,TIMER_INT_UP);  // 中断使能
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

//开始显示
void LCD_Show_Gram(void)
{
    //设置标志位为未显示完成状态
    show_over_flag=1;
    
    //设置显示范围
    LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    
    LCD_CS_Clr();
		
	//清除全部中断标志位(至少清除通道3的全部中断标志位)
    DMA_INTC0(DMA1) = 0xfffffff;
	
    //设置传输数据大小
    DMA_CHCNT(DMA1, DMA_CH3) = 33600;
	
    //设置传输地址
	DMA_CH3M0ADDR(DMA1) = (uint32_t)Show_Gram;
	
    //开始传输
    DMA_CHCTL(DMA1, DMA_CH3) |= DMA_CHXCTL_CHEN;
}

//获取 一帧是否显示完成标志位  =0完成 =1未完成
uint8_t get_show_over_flag(void)
{
    return show_over_flag;
}
//设置 一帧是否显示完成标志位
void set_show_over_flag(uint8_t flag)
{
    show_over_flag = flag;
}
//获取 是否需要更新标志位  =1需要更新 =0未需要更新
uint8_t get_show_update_flag(void)
{
    return show_update_flag;
}
//设置 是否需要更新标志位
void set_show_update_flag(uint8_t flag)
{
    show_update_flag = flag;
}
//屏幕搬运完成
void DMA1_Channel3_IRQHandler(void)
{
    static uint8_t Show_Number=0;
		
		//全屏幕需要搬运4次
    if((++Show_Number) < 4)
	{
        //清除全部DMA1中断标志位	 
        DMA_INTC0(DMA1) = 0xfffffff;		
        //重新填充要搬运的数据量	
        DMA_CHCNT(DMA1, DMA_CH3) = 33600;
        //内存搬运地址
        DMA_CH3M0ADDR(DMA1) = (uint32_t)Show_Gram+33600*Show_Number;
        //开启搬运
        DMA_CHCTL(DMA1, DMA_CH3) |= DMA_CHXCTL_CHEN;
    }
    else
    {
		//清除DMA搬运完成中断标志位
        dma_interrupt_flag_clear(DMA1, DMA_CH3, DMA_INT_FLAG_FTF);
		//等待SPI发送完毕
        while(SPI_STAT(PORT_SPI) & SPI_STAT_TRANS);
		//SPI片选拉高
        LCD_CS_Set();
		//搬运次数清零
        Show_Number=0;
		//一帧搬运完成标志位
        show_over_flag=0;
    }
}


/******************************************************************
 * 函 数 名 称：TIMER3_IRQHandler
 * 函 数 说 明：定时器3中断服务函数
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：LCD的显示BUFF切换 与 按键扫描
******************************************************************/
void TIMER3_IRQHandler(void)
{   
    //清除中断标志位
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
        
    //如果屏幕显示数据DMA搬运完成
    if(show_update_flag)
    {
        //清除完成标志
        show_update_flag=0;
        //更新显示
        LCD_Show_Gram();
    }
    
    
    //定时器循环固定数据刷屏
    if( get_show_update_flag() == 0 )
    {
		
        if(get_show_over_flag() == 0){
            set_show_update_flag(1);
        }
    }   
    
    //有害气体采集时间判断
    tvoc_measured();
    
    //设置键扫描
    set_key_scan();

    //左键扫描
    left_key_scan();
    
    //右键扫描
    right_key_scan();
	
	//返回键扫描
	return_key_scan();
	
	
}