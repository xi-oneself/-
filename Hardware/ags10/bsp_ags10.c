#include "bsp_ags10.h"
#include "bsp_usart.h"
#include "stdio.h"

//�ȴ��ɼ���ɱ�־      =1��ʾ�������ɼ����     =0��ʾ������û�вɼ����
uint8_t tvoc_measured_accomplish_flag = 0;

/******************************************************************
 * �� �� �� �ƣ�ags10_gpio_init
 * �� �� ˵ ������AGS10��IIC���ų�ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC 
 * ��       ע��
******************************************************************/
void ags10_gpio_init(void)
{                                
    //��SDA��SCL������ʱ��
    rcu_periph_clock_enable(RCU_AGS10_SCL);
    rcu_periph_clock_enable(RCU_AGS10_SDA);

    //����SCL����ģʽΪ�������
    gpio_mode_set(PORT_AGS10_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AGS10_SCL);
    //��������Ϊ��©ģʽ����ת�ٶ�2MHz
    gpio_output_options_set(PORT_AGS10_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AGS10_SCL);        
    //������������ߵ�ƽSCL�ȴ��ź�
    gpio_bit_write(PORT_AGS10_SCL, GPIO_AGS10_SCL, SET);
    
    //����SDA����
    gpio_mode_set(PORT_AGS10_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AGS10_SDA);
    gpio_output_options_set(PORT_AGS10_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AGS10_SDA);        
    gpio_bit_write(PORT_AGS10_SDA, GPIO_AGS10_SDA, SET);
}


//��ʼ�ź�
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

//ֹͣ�ź�
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

//���ͷ�Ӧ��
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

//����Ӧ��
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
 * �� �� �� �ƣ�I2C_WaitAck
 * �� �� �� �ܣ��ȴ��ӻ�Ӧ��
 * �� �� �� ������
 * �� �� �� �أ�1=��Ӧ��	 0=Ӧ��
 * ��       �ߣ�LC
 * ��       ע����
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
	
	//��Ӧ��
	if( ack_flag <= 0 )
	{
		AGS10_IIC_Stop();
		return 1;
	}
	else//Ӧ��
	{
		AGS10_SCL(0);
		AGS10_SDA_OUT();
	}
	return ack;
}

//����һ���ֽ�
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

//����һ���ֽ�
unsigned char AGS10_IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	AGS10_SDA_IN();//SDA����Ϊ����
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
//�������ƣ�Calc_CRC8
//���� ��CRC8 ���㣬��ֵ��0xFF������ʽ��0x31(x8 + x5 + x4 +1)
//���� ��u8* dat����ҪУ�����ݵ��׵�ַ��u8 Num��CRC У�����ݳ���
//���� ��crc���������У��ֵ
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
    
    //�����ʱ
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

//����Ƶ�����͡����ݲɼ����������ᵼ�´������޷������ɼ����ݣ��Ӷ�ʹ��Status��״̬λ
//RDYλ��ֵһֱΪ1�������ݲɼ�������������ʱ�䲻��С��1.5s��
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