/********************************************************************************
  * �� �� ��: bsp_bluetooth
  * �� �� ��: ����
  * �޸�����: LC
  * �޸�����: 2022��06��05��
  * ���ܽ���: HC-05����ģ��ĵײ�����         
  ******************************************************************************
  * �����������www.lckfb.com        
*********************************************************************************/

#include "bsp_hc05.h"
#include "stdio.h"
#include "bsp_usart.h"

unsigned char Bluetooth_ConnectFlag = 0; //��������״̬ =0û���ֻ�����		=1���ֻ�����
unsigned char BLERX_BUFF[BLERX_LEN_MAX];     
unsigned char BLERX_FLAG = 0;
unsigned char BLERX_LEN = 0;

/************************************************************
 * �������ƣ�Bluetooth_GPIO_Init
 * ����˵��������RXTX���ų�ʼ��
 * ��    �Σ�bund=���ڲ�����
 * �� �� ֵ����
 * ��    ע����
*************************************************************/
void Bluetooth_GPIO_Init(unsigned int bund)
{
	/* ʹ������ʱ�� */
	rcu_periph_clock_enable(BLUETOOTH_TXD_RCU);
    rcu_periph_clock_enable(BLUETOOTH_RXD_RCU);
	/* ʹ�ܴ���ʱ�� */
	rcu_periph_clock_enable(BLE_USART_RCU);
	
	/*	��������Ϊ���ù���x */
	gpio_af_set(BLUETOOTH_RXD_PORT, BLE_USART_AF, BLUETOOTH_RXD_GPIO);
	gpio_af_set(BLUETOOTH_TXD_PORT, BLE_USART_AF, BLUETOOTH_TXD_GPIO);
	
	/*	��������Ϊ��������ģʽ */
	gpio_mode_set(BLUETOOTH_RXD_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUETOOTH_RXD_GPIO);
	gpio_mode_set(BLUETOOTH_TXD_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUETOOTH_TXD_GPIO);
	
	/*	��������ΪΪ���ģʽ */
	gpio_output_options_set(BLUETOOTH_RXD_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUETOOTH_RXD_GPIO);
	gpio_output_options_set(BLUETOOTH_TXD_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUETOOTH_TXD_GPIO);
	
	/*	����USART�Ĳ����� */
	usart_baudrate_set(BLE_USART, bund);
	
	/*	����USART��У��λΪ�� */
	usart_parity_config(BLE_USART, USART_PM_NONE);
	
	/*	����USART������λΪ8λ */
	usart_word_length_set(BLE_USART, USART_WL_8BIT);
	
	/*	����USART��ֹͣλΪ1λ */
	usart_stop_bit_set(BLE_USART, USART_STB_1BIT);
	
	/*	ʹ�ܴ��� */
	usart_enable(BLE_USART);
	
	/*	ʹ��USART���� */
	usart_transmit_config(BLE_USART, USART_TRANSMIT_ENABLE);
	
	/*	ʹ��USART���� */
	usart_receive_config(BLE_USART, USART_RECEIVE_ENABLE);
	
	/*	ʹ��USART�����жϱ�־λ */
	usart_interrupt_enable(BLE_USART, USART_INT_RBNE);   
	
  /*	ʹ��USART�����жϱ�־λ */
	usart_interrupt_enable(BLE_USART, USART_INT_IDLE); // DLE �߼���ж�

	/* �����ж����ȼ� */
	nvic_irq_enable(BLE_USART_IRQ, 2, 2); // �����ж����ȼ�
}

/******************************************************************
 * �� �� �� �ƣ�BLE_Send_Bit
 * �� �� ˵ �������������͵����ַ�
 * �� �� �� �Σ�ch=ASCII�ַ�
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void BLE_Send_Bit(unsigned char ch)
{
	//�����ַ�
	usart_data_transmit(BLE_USART, ch);
	// �ȴ��������ݻ�������־�Զ���λ
	while(RESET == usart_flag_get(BLE_USART, USART_FLAG_TBE) );
}  

/******************************************************************
 * �� �� �� �ƣ�BLE_send_String
 * �� �� ˵ ���������������ַ��� 
 * �� �� �� �Σ�str=���͵��ַ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void BLE_send_String(unsigned char *str)
{
	while( str && *str ) // ��ַΪ�ջ���ֵΪ������
	{	
		BLE_Send_Bit(*str++);
	}	
}

/******************************************************************
 * �� �� �� �ƣ�Clear_BLERX_BUFF
 * �� �� ˵ ����������ڽ��յ�����
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Clear_BLERX_BUFF(void)
{
	unsigned char i = BLERX_LEN_MAX-1;
	while(i)        
	{
		BLERX_BUFF[i--] = 0;
	}
	BLERX_LEN = 0;
}

/******************************************************************
 * �� �� �� �ƣ�Bluetooth_Link_Gpio_Init
 * �� �� ˵ �����������ӳɹ�ָʾ����state��ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Bluetooth_Link_Gpio_Init(void)	
{
	//��4��LED������ʱ��
	rcu_periph_clock_enable(BLUETOOTH_LINK_RCU);
	//��������ģʽΪ��������
	gpio_mode_set(BLUETOOTH_LINK_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BLUETOOTH_LINK_GPIO);	
}

/******************************************************************
 * �� �� �� �ƣ�Bluetooth_Init
 * �� �� ˵ ����������ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��Ĭ�ϲ�����Ϊ9600
******************************************************************/
void Bluetooth_Init(void)
{
	
	Bluetooth_GPIO_Init(9600);
    Bluetooth_Link_Gpio_Init();

    #if	DEBUG
 	//�ڵ���ʱ��ͨ��AT�����Ѿ����ú�ģʽ
	printf("Bluetooth_Init succeed!\r\n");

    #endif
}

/******************************************************************
 * �� �� �� �ƣ�Get_Bluetooth_ConnectFlag
 * �� �� ˵ ������ȡ�ֻ�����״̬ 
 * �� �� �� �Σ���
 * �� �� �� �أ�����1=������		����0=δ����
 * ��       �ߣ�LC
 * ��       ע��ʹ�øú���ǰ�������ȵ��� Bluetooth_Mode ����
******************************************************************/
unsigned char Get_Bluetooth_ConnectFlag(void)
{
	return Bluetooth_ConnectFlag;
}

/******************************************************************
 * �� �� �� �ƣ�Bluetooth_Mode
 * �� �� ˵ �����ж�����ģ�������״̬
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��δ����ʱSTATE�͵�ƽ   ���ӳɹ�ʱSTATE�ߵ�ƽ
******************************************************************/
void Bluetooth_Mode(void)
{
	static char flag = 0;
	//���û���ֻ�����
	if( DISCONNECT == BLUETOOTH_LINK ) 
	{            
        //����״̬Ϊδ����
		Bluetooth_ConnectFlag = 0;
        //���֮ǰ������״̬
		if( flag == 1 )     
		{
			flag = 0;//�޸�״̬
		}
		return;
	}
	//����ֻ��Ѿ�����
	if( CONNECT == BLUETOOTH_LINK )
	{
		Bluetooth_ConnectFlag = 1;
        //���֮ǰ�ǶϿ�״̬
		if( flag == 0 )
		{
			flag = 1;//�޸�״̬
		}
	}
}

/******************************************************************
 * �� �� �� �ƣ�Receive_Bluetooth_Data_jieshou
 * �� �� ˵ ����������������
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/

void Receive_Bluetooth_Data_jieshou(void)
{
    //��ȡ����״̬
    Bluetooth_Mode();

	//����ֻ��Ѿ�����
	if( Bluetooth_ConnectFlag == 1 )
	{
		if( BLERX_FLAG == 1 )//���յ���������
		{
			
			BLERX_FLAG = 0;		
            
      //��ʾ�������͹���������
      printf("data = %s\r\n",BLERX_BUFF);
            
			Clear_BLERX_BUFF();
			//������ջ���
		}
	}
}

/******************************************************************
 * �� �� �� �ƣ�Receive_Bluetooth_Data
 * �� �� ˵ ����������������
 * �� �� �� �Σ���
 * �� �� �� �أ����յ�����������
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/

char* Receive_Bluetooth_Data(void)
{
    //��ȡ����״̬
    Bluetooth_Mode();
    //����ֻ��Ѿ�����
    if( Bluetooth_ConnectFlag == 1 )
    {
        if( BLERX_FLAG == 1 )//���յ���������
        {
            BLERX_FLAG = 0;     
            //�����������͹���������
            return BLERX_BUFF;
					Clear_BLERX_BUFF();
        }
    }
    //���û�н��յ����ݣ����ؿ�ָ��
    return NULL;
}

/******************************************************************
 * �� �� �� �ƣ�Send_Bluetooth_Data
 * �� �� ˵ ����������ģ�鷢������
 * �� �� �� �Σ�dat=Ҫ���͵��ַ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��������ֻ��������������������ֻ��������ݣ�
******************************************************************/


void Send_Bluetooth_Data(char *dat)
{
    //��ȡ����״̬
   Bluetooth_Mode();
	//����ֻ��Ѿ�����
	if( Bluetooth_ConnectFlag == 1 )
	{
        //��������
        BLE_send_String((unsigned char*)dat);
   }
}




/******************************************************************
 * �� �� �� �ƣ�BLE_USART_IRQHandler
 * �� �� ˵ �������������Ĵ����жϷ�����
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void BLE_USART_IRQHandler(void)
{
	if(usart_interrupt_flag_get(BLE_USART,USART_INT_FLAG_RBNE) != RESET) // ���ջ�������Ϊ��
	{
		BLERX_BUFF[ BLERX_LEN ] = usart_data_receive(BLE_USART);
		#if	DEBUG
        printf("%c",BLERX_BUFF[ BLERX_LEN ]);
        #endif
        BLERX_LEN = ( BLERX_LEN + 1 ) % BLERX_LEN_MAX;
	}
	if(usart_interrupt_flag_get(BLE_USART,USART_INT_FLAG_IDLE) == SET) // ��⵽�����ж�
	{
		usart_data_receive(BLE_USART); // ����Ҫ������������ֵ����Ҫ
		BLERX_BUFF[BLERX_LEN] = '\0';
		BLERX_FLAG = SET;// �������
	}
}

