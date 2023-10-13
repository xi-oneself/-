#include "bsp_wf183d.h"

#define DATA_LENGTH_MAX     200
uint8_t AirPressure_Data_Buff[DATA_LENGTH_MAX];
uint8_t AirPressure_data_length = 0;
uint8_t AirPressure_data_flag = 0;

//Ĭ�ϲ�����9600
void wf183d_gpio_config(void)
{
	/* ����ʱ�� */
	rcu_periph_clock_enable(BSP_WF183D_USART_TX_RCU);   // ��������ʱ��
	rcu_periph_clock_enable(BSP_WF183D_USART_RX_RCU);   // �����˿�ʱ��
	rcu_periph_clock_enable(BSP_WF183D_USART_RCU);      // �����˿�ʱ��
	
	/* ����GPIO���ù��� */
    gpio_af_set(BSP_WF183D_USART_TX_PORT,BSP_WF183D_USART_AF,BSP_WF183D_USART_TX_PIN);	
	gpio_af_set(BSP_WF183D_USART_RX_PORT,BSP_WF183D_USART_AF,BSP_WF183D_USART_RX_PIN);	
	
	/* ����GPIO��ģʽ */
	/* ����TXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BSP_WF183D_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_WF183D_USART_TX_PIN);
	/* ����RXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BSP_WF183D_USART_RX_PORT, GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_WF183D_USART_RX_PIN);
	
	/* ����TXΪ������� 50MHZ */
	gpio_output_options_set(BSP_WF183D_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_WF183D_USART_TX_PIN);
	/* ����RXΪ������� 50MHZ */
	gpio_output_options_set(BSP_WF183D_USART_RX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_WF183D_USART_RX_PIN);

	/* ���ô��ڵĲ��� */
	usart_deinit(BSP_WF183D_USART);                                 // ��λ����
	usart_baudrate_set(BSP_WF183D_USART,9600);                      // ���ò�����
	usart_parity_config(BSP_WF183D_USART,USART_PM_NONE);            // û��У��λ
	usart_word_length_set(BSP_WF183D_USART,USART_WL_8BIT);          // 8λ����λ
	usart_stop_bit_set(BSP_WF183D_USART,USART_STB_1BIT);     			 // 1λֹͣλ

    /* ʹ�ܴ��� */
	usart_enable(BSP_WF183D_USART);                                 // ʹ�ܴ���
	usart_transmit_config(BSP_WF183D_USART,USART_TRANSMIT_ENABLE);  // ʹ�ܴ��ڷ���
	usart_receive_config(BSP_WF183D_USART,USART_RECEIVE_ENABLE);    // ʹ�ܴ��ڽ���
	
	/* �ж����� */
	nvic_irq_enable(BSP_WF183D_USART_IRQ, 1, 2); 						     // �����ж����ȼ�
	usart_interrupt_enable(BSP_WF183D_USART,USART_INT_RBNE);				 // �����ݻ������ǿ��жϺ���������ж�
	usart_interrupt_enable(BSP_WF183D_USART,USART_INT_IDLE);				 // ���м���ж�
}

//���ڷ���ָ����������
void wf183d_send_command(uint8_t *ucstr, uint16_t length)
{
	while(length--)        
	{
        usart_data_transmit(BSP_WF183D_USART,*ucstr++);					    // ��������
        while(RESET == usart_flag_get(BSP_WF183D_USART,USART_FLAG_TBE));    // �ȴ��������ݻ�������־��λ
	}
}

//������ڽ���ȫ������
void wf183d_data_clear(void)
{
    uint16_t i = DATA_LENGTH_MAX - 1;
    while( i )
    {
        AirPressure_Data_Buff[ i-- ] = 0;
    }       
    AirPressure_data_length = 0;
}

//CRCУ��
uint8_t Cal_uart_buf_CRC (uint8_t *arr, uint8_t len)
{
    uint8_t crc=0 ;
    uint8_t i=0 ;
    while(len--)
    {
        crc ^= *arr++;
        for(i = 0 ;i < 8 ;i++)
        {
            if(crc & 0x01) crc = (crc >> 1) ^ 0x8c;
            else crc >>= 1 ;
        }
    }
    return crc ;
}
/******************************************************************
 * �� �� �� �ƣ�FrameResolution
 * �� �� ˵ ����֡��ʽ����
 * �� �� �� �Σ�buff��֡����
* �� �� �� �أ�1��֡ͷ����  2:У׼����    ��������ѹ����
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint32_t FrameResolution(uint8_t *buff)
{
    uint32_t data = 0;
    //���֡ͷ����0XAA
    if( buff[0] != 0xAA ) return 1; 
    //���CRCУ���ֵ�����յ�У��ֵ��һ��
    if( Cal_uart_buf_CRC( buff, 7 ) != buff[7] ) return 2;
    //��������
    data = buff[3] | (buff[4] << 8) | (buff[5] << 16) | (buff[6] << 24);
    
    return data;
}

uint8_t get_TemperatureCmd_buff[4] = {0X55, 0X04, 0X0E, 0X6A};
uint8_t get_AirPressureCmd_buff[4] = {0X55, 0X04, 0X0D, 0X88};

/******************************************************************
 * �� �� �� �ƣ�GetAirPressureValue
 * �� �� ˵ ������ȡ����ѹֵ
 * �� �� �� �Σ���
 * �� �� �� �أ�����ѹֵ����λPA
 * ��       �ߣ�LCKFB
 * ��       ע������ת��ѹ����Ҫ���ݵ�ǰ�¶Ƚ��в�����������Ҫ�Ƚ��вɼ�ת���¶�
******************************************************************/
uint32_t GetAirPressureValue(void)
{
    uint32_t AirPressureValue = 0;
    
    //���Ͳɼ��¶�����
    wf183d_send_command( get_TemperatureCmd_buff, 4 );
    //�ȴ����ݽ������
    while( AirPressure_data_flag != 1 );
    //���������ɱ�׼λ
    AirPressure_data_flag = 0;
    //�����ǰ���յ����¶�����֡
    wf183d_data_clear();
    
    //���Ͳɼ��¶�����
    wf183d_send_command( get_AirPressureCmd_buff, 4 );
    //�ȴ����ݽ������
    while( AirPressure_data_flag != 1 );
    //���������ɱ�׼λ
    AirPressure_data_flag = 0;
    //������ѹ֡����������ѹ����
    AirPressureValue = FrameResolution(AirPressure_Data_Buff);
    return AirPressureValue;
}

//����1�жϷ�����
void USART1_IRQHandler(void)
{
	if(usart_interrupt_flag_get(BSP_WF183D_USART,USART_INT_FLAG_RBNE) == SET)   // ���ջ�������Ϊ��
	{
		AirPressure_Data_Buff[AirPressure_data_length++] = usart_data_receive(BSP_WF183D_USART);// �ѽ��յ������ݷŵ���������
	}
	
	if(usart_interrupt_flag_get(BSP_WF183D_USART,USART_INT_FLAG_IDLE) == SET)   // ��⵽֡�ж�
	{
		usart_data_receive(BSP_WF183D_USART);                                   // ����Ҫ������������ֵ����Ҫ
		AirPressure_Data_Buff[AirPressure_data_length] = '\0';// ���ݽ�����ϣ����������־
        AirPressure_data_flag = 1;
	}
	
}