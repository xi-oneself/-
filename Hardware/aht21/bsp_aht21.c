#include "bsp_aht21.h"
#include "systick.h"
#include "bsp_usart.h"
#include "stdio.h"

static float temperature = 0;
static float humidity = 0;
/******************************************************************
 * �� �� �� �ƣ�aht21_gpio_init
 * �� �� ˵ ������AHT21��IIC���ų�ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC 
 * ��       ע��
******************************************************************/
void aht21_gpio_init(void)
{                                
    //��SDA��SCL������ʱ��
    rcu_periph_clock_enable(RCU_AHT21_SCL);
    rcu_periph_clock_enable(RCU_AHT21_SDA);

    //����SCL����ģʽΪ�������
    gpio_mode_set(PORT_AHT21_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AHT21_SCL);
    //��������Ϊ��©ģʽ����ת�ٶ�2MHz
    gpio_output_options_set(PORT_AHT21_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AHT21_SCL);        
    //������������ߵ�ƽSCL�ȴ��ź�
    gpio_bit_write(PORT_AHT21_SCL, GPIO_AHT21_SCL, SET);
    
    //����SDA����
    gpio_mode_set(PORT_AHT21_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_AHT21_SDA);
    gpio_output_options_set(PORT_AHT21_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_AHT21_SDA);        
    gpio_bit_write(PORT_AHT21_SDA, GPIO_AHT21_SDA, SET);
}


//��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();
	SDA(1);
	SCL(1); 
	delay_1us(5);
	SDA(0);
	delay_1us(5);
	SCL(0);
	delay_1us(5);               
}

//ֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	SCL(0);
	SDA(0);
	SCL(1);
	delay_1us(5);
	SDA(1);
	delay_1us(5);
}

//���ͷ�Ӧ��
void IIC_Send_Nack(void)
{
	SDA_OUT();
	SCL(0);
	SDA(0);
	SDA(1);
	SCL(1);
	delay_1us(5);
	SCL(0);
	SDA(0);
}

//����Ӧ��
void IIC_Send_Ack(void)
{
	SDA_OUT();
	SCL(0);
	SDA(1);
	SDA(0);
	SCL(1);
	delay_1us(5);
	SCL(0);
	SDA(1);
}


/**********************************************************
 * �� �� �� �ƣ�I2C_WaitAck
 * �� �� �� �ܣ��ȴ��ӻ�Ӧ��
 * �� �� �� ������
 * �� �� �� �أ�1=��Ӧ��	 0=Ӧ��
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
unsigned char I2C_WaitAck(void)
{
	char ack = 0;
	unsigned char ack_flag = 10;
	SCL(0);
	SDA(1);
	SDA_IN();
	delay_1us(5);
	SCL(1);
    delay_1us(5);

	while( (GETSDA()==1)  &&  ( ack_flag ) )
	{
		ack_flag--;
		delay_1us(5);
	}
	
	//��Ӧ��
	if( ack_flag <= 0 )
	{
		IIC_Stop();
		return 1;
	}
	else//Ӧ��
	{
		SCL(0);
		SDA_OUT();
	}
	return ack;
}

//����һ���ֽ�
void IIC_Send_Byte(uint8_t dat)
{
	int i = 0;
	SDA_OUT();
	SCL(0);
		
	for( i = 0; i < 8; i++ )
	{
		SDA( dat >> 7 );		
		delay_1us(1);
		SCL(1);
		delay_1us(5);
		SCL(0);
		delay_1us(5);
		dat<<=1;
	}	
}

//����һ���ֽ�
unsigned char IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		SCL(0);
		delay_1us(5);
		SCL(1);
		delay_1us(5);
		receive<<=1;
		if( GETSDA() )
		{	
			receive |= 1;   
		} 
	}
	SCL(0); 
	return receive;
}

//��ȡAHT21��״̬�Ĵ���
uint8_t aht21_read_status(void)
{
    uint8_t status_register_address = 0x71;
    
	uint8_t status_byte;	
    
	IIC_Start();
    
    IIC_Send_Byte( status_register_address );
    if( I2C_WaitAck() == 1 ) printf("warning -1\r\n");
    
	status_byte = IIC_Read_Byte();
	IIC_Send_Nack();	
	IIC_Stop();

	return status_byte;
}

//��AHT21���Ͳɼ�����
uint8_t aht21_send_gather_command(void)
{
    uint8_t device_addr = 0x70;//������ַ
    uint8_t gather_command = 0xac;//�ɼ�����
    uint8_t gather_command_parameter_1 = 0x33;//�ɼ�����1
    uint8_t gather_command_parameter_2 = 0x00;//�ɼ�����2
    
    IIC_Start();
	IIC_Send_Byte(device_addr);//����������ַ
	if( I2C_WaitAck() == 1 ) printf("warning -1\r\n");
	IIC_Send_Byte(gather_command);//���Ͳɼ�����
	if( I2C_WaitAck() == 1 ) printf("warning -2\r\n");
	IIC_Send_Byte(gather_command_parameter_1);//���Ͳɼ�����1
	if( I2C_WaitAck() == 1 ) printf("warning -3\r\n");
	IIC_Send_Byte(gather_command_parameter_2);//���Ͳɼ�����2
	if( I2C_WaitAck() == 1 ) printf("warning -4\r\n");
	IIC_Stop();
}

//ͨ�������ֽڳ�ʼ��AHT21
void aht21_device_init(void)  
{	
    uint8_t device_addr = 0x70;//������ַ
    uint8_t init_command = 0xBE;//��ʼ������
    uint8_t init_command_parameter_1 = 0x08;//�ɼ�����1
    uint8_t init_command_parameter_2 = 0x00;//�ɼ�����2

    IIC_Start();
	IIC_Send_Byte(device_addr);//����������ַ
	if( I2C_WaitAck() == 1 ) printf("warning -5\r\n");
	IIC_Send_Byte(init_command);//���ͳ�ʼ������
	if( I2C_WaitAck() == 1 ) printf("warning -6\r\n");
	IIC_Send_Byte(init_command_parameter_1);//���ͳ�ʼ������1
	if( I2C_WaitAck() == 1 ) printf("warning -7\r\n");
	IIC_Send_Byte(init_command_parameter_2);//���ͳ�ʼ������2
	if( I2C_WaitAck() == 1 ) printf("warning -8\r\n");
	IIC_Stop();
}

/******************************************************************
 * �� �� �� �ƣ�aht21_read_data
 * �� �� ˵ ������ȡ��ʪ��
 * �� �� �� �Σ���
* �� �� �� �أ�1��δУ׼  2����ȡ��ʱ  0����ȡ�ɹ�
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
char aht21_read_data(void)
{
    uint8_t data[6] = {0};
    uint32_t temp = 0;
    
    uint8_t aht21_status_byte = 0;
    uint8_t timeout = 0;
    //��ȡAHT21��״̬
    aht21_status_byte = aht21_read_status();
    //���δУ׼���򷵻�1
    if( (aht21_status_byte & (1<<3)) == 0 ) 
    {
        aht21_device_init();
        delay_1ms(50);
        return 1;
    }
    
    //���Ͳɼ�����
    aht21_send_gather_command();
    
    do
    {
        delay_1ms(1);
        timeout++;
        //��ȡAHT21��״̬
        aht21_status_byte = aht21_read_status();
    }while( ( ( aht21_status_byte & (1<<7) ) != 0 ) && ( timeout >= 80 ) );
    //�����ȡ��ʱ���򷵻�2
    if( timeout >= 80 ) return 2;
       
    IIC_Start();
    IIC_Send_Byte(0x71);
    if( I2C_WaitAck() == 1 ) printf("error -1\r\n");
	IIC_Read_Byte();//��ȡ״̬,����Ҫ����
	IIC_Send_Ack();	
         
    //��ȡ6λ����
    data[0] = IIC_Read_Byte();
    IIC_Send_Ack();
    data[1] = IIC_Read_Byte();
    IIC_Send_Ack();
    data[2] = IIC_Read_Byte();
    IIC_Send_Ack();
    data[3] = IIC_Read_Byte();
    IIC_Send_Ack();
    data[4] = IIC_Read_Byte();
    IIC_Send_Ack();
    data[5] = IIC_Read_Byte();
    IIC_Send_Nack();
    
    IIC_Stop();
     
    //����ʪ������
    temp = (data[0]<<12) | (data[1]<<4);
    temp = temp | (data[2]>>4);
    //����ʪ������
    //2��20�η� = 1048576
    humidity = temp / 1048576.0 * 100.0;
    
    
    //����ʪ������
    temp = ( (data[2]&0x0f)<< 16 ) | (data[3]<<8);
    temp = temp | data[4];
    //����ʪ������
    //2��20�η� = 1048576
    temperature = temp / 1048576.0 * 200.0 - 50;

    return 0;
}

//���ض�ȡ�����¶�
float get_temperature(void)
{
    return temperature;
}
//���ض�ȡ����ʪ��
float get_humidity(void)
{
    return humidity;
}
