#include "bsp_ags10.h"
#include "bsp_usart.h"
#include "stdio.h"

//等待采集完成标志      =1表示传感器采集完成     =0表示传感器没有采集完成
uint8_t tvoc_measured_accomplish_flag = 0;

/******************************************************************
 * 函 数 名 称：ags10_gpio_init
 * 函 数 说 明：对AGS10的IIC引脚初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC 
 * 备       注：
******************************************************************/
void ags10_gpio_init(void)
{                                
    //打开SDA与SCL的引脚时钟
    rcu_periph_clock_enable(RCU_AGS10_SCL);
    rcu_periph_clock_enable(RCU_AGS10_SDA);

    //设置SCL引脚模式为上拉输出
    gpio_mode_set(PORT_AGS10_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AGS10_SCL);
    //设置引脚为开漏模式，翻转速度2MHz
    gpio_output_options_set(PORT_AGS10_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AGS10_SCL);        
    //设置引脚输出高电平SCL等待信号
    gpio_bit_write(PORT_AGS10_SCL, GPIO_AGS10_SCL, SET);
    
    //设置SDA引脚
    gpio_mode_set(PORT_AGS10_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AGS10_SDA);
    gpio_output_options_set(PORT_AGS10_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AGS10_SDA);        
    gpio_bit_write(PORT_AGS10_SDA, GPIO_AGS10_SDA, SET);
}


//起始信号
void AGS10_IIC_Start(void)
{
	AGS10_SDA_OUT();
	AGS10_SDA(1);
	AGS10_SCL(1); 
	delay_1us(5);
	AGS10_SDA(0);
	delay_1us(5);
	AGS10_SCL(0);
	delay_1us(5);               
}

//停止信号
void AGS10_IIC_Stop(void)
{
	AGS10_SDA_OUT();
	AGS10_SCL(0);
	AGS10_SDA(0);
	AGS10_SCL(1);
	delay_1us(5);
	AGS10_SDA(1);
	delay_1us(5);
}

//发送非应答
void AGS10_IIC_Send_Nack(void)
{
	AGS10_SDA_OUT();
	AGS10_SCL(0);
	AGS10_SDA(0);
	AGS10_SDA(1);
	AGS10_SCL(1);
	delay_1us(5);
	AGS10_SCL(0);
	AGS10_SDA(0);
}

//发送应答
void AGS10_IIC_Send_Ack(void)
{
	AGS10_SDA_OUT();
	AGS10_SCL(0);
	AGS10_SDA(1);
	AGS10_SDA(0);
	AGS10_SCL(1);
	delay_1us(5);
	AGS10_SCL(0);
	AGS10_SDA(1);
}


/**********************************************************
 * 函 数 名 称：I2C_WaitAck
 * 函 数 功 能：等待从机应答
 * 传 入 参 数：无
 * 函 数 返 回：1=非应答	 0=应答
 * 作       者：LC
 * 备       注：无
**********************************************************/
unsigned char AGS10_I2C_WaitAck(void)
{
	char ack = 0;
	unsigned char ack_flag = 10;
	AGS10_SCL(0);
	AGS10_SDA(1);
	AGS10_SDA_IN();
	delay_1us(5);
	AGS10_SCL(1);
    delay_1us(5);

	while( (AGS10_GETSDA()==1)  &&  ( ack_flag ) )
	{
		ack_flag--;
		delay_1us(5);
	}
	
	//非应答
	if( ack_flag <= 0 )
	{
		AGS10_IIC_Stop();
		return 1;
	}
	else//应答
	{
		AGS10_SCL(0);
		AGS10_SDA_OUT();
	}
	return ack;
}

//发送一个字节
void AGS10_IIC_Send_Byte(uint8_t dat)
{
	int i = 0;
	AGS10_SDA_OUT();
	AGS10_SCL(0);
		
	for( i = 0; i < 8; i++ )
	{
//		AGS10_SDA( (dat & 0x80) >> 7 );
		AGS10_SDA( dat >> 7 );
		delay_1us(1);
		AGS10_SCL(1);
		delay_1us(5);
		AGS10_SCL(0);
		delay_1us(5);
		dat<<=1;
	}	
}

//接收一个字节
unsigned char AGS10_IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	AGS10_SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		AGS10_SCL(0);
		delay_1us(5);
		AGS10_SCL(1);
		delay_1us(5);
		receive<<=1;
		if( AGS10_GETSDA() )
		{	
			receive |= 1;   
		} 
	}
	AGS10_SCL(0); 
	return receive;
}

//********************************************************************
//函数名称：Calc_CRC8
//功能 ：CRC8 计算，初值：0xFF，多项式：0x31(x8 + x5 + x4 +1)
//参数 ：u8* dat：需要校验数据的首地址；u8 Num：CRC 校验数据长度
//返回 ：crc：计算出的校验值
//********************************************************************
uint8_t Calc_CRC8(uint8_t *dat, uint8_t Num)
{
    uint8_t i, byte, crc=0xFF;
    for(byte=0; byte<Num; byte++)
    {
        crc ^= (dat[byte]);
        for( i = 0; i < 8; i++ )
        {
            if(crc & 0x80)  crc = ( crc << 1 ) ^ 0x31;
            else            crc = ( crc << 1 );
        }
    }
    return crc;
}

uint32_t ags10_read(void)
{
    uint8_t timeout = 0;
    uint8_t data[5] = {0};   
    uint32_t TVOC_data = 0;

    AGS10_IIC_Start();
    AGS10_IIC_Send_Byte(0X34);
    if( AGS10_I2C_WaitAck() == 1 ) return 1;
    AGS10_IIC_Send_Byte(0X00);
    if( AGS10_I2C_WaitAck() == 1 ) return 2;
    AGS10_IIC_Stop();
    
    do{
        delay_1ms(1);
        timeout++;
        AGS10_IIC_Start();
        AGS10_IIC_Send_Byte(0X35);
    }while( (AGS10_I2C_WaitAck() == 1) && (timeout >= 50) );
    
    //如果超时
    if( timeout >= 50 ) return 3;
    
    data[0] = AGS10_IIC_Read_Byte();
    AGS10_IIC_Send_Ack();
    data[1] = AGS10_IIC_Read_Byte();
    AGS10_IIC_Send_Ack();
    data[2] = AGS10_IIC_Read_Byte();
    AGS10_IIC_Send_Ack();
    data[3] = AGS10_IIC_Read_Byte();
    AGS10_IIC_Send_Ack();
    data[4] = AGS10_IIC_Read_Byte();
    AGS10_IIC_Send_Nack();
    
    AGS10_IIC_Stop();
    
    if( Calc_CRC8(data,4) != data[4] )
    {
//        printf("Check failed\r\n");
        return 4;
    }
    TVOC_data = (data[1]<<16) | (data[2]<<8) | data[3];
    return TVOC_data;//printf("TVOC_data = %d\r\n",TVOC_data);
//    return 0;
}

//不可频繁发送“数据采集”命令，否则会导致传感器无法正常采集数据，从而使得Status的状态位
//RDY位数值一直为1。“数据采集”命令间隔发送时间不可小于1.5s。
uint8_t tvoc_measured(void)
{
    static uint8_t outtime = 0;
    //800ms
    if( outtime >= 40 )
    {
        outtime = 0;
        tvoc_measured_accomplish_flag = 1;
    }
    else
    {
        outtime++;
    }
}

uint8_t get_tvoc_measured_accomplish_flag(void)
{
    return tvoc_measured_accomplish_flag;
}
void set_tvoc_measured_accomplish_flag(uint8_t flag)
{
    tvoc_measured_accomplish_flag = flag;
}