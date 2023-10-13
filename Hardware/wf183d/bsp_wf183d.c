#include "bsp_wf183d.h"

#define DATA_LENGTH_MAX     200
uint8_t AirPressure_Data_Buff[DATA_LENGTH_MAX];
uint8_t AirPressure_data_length = 0;
uint8_t AirPressure_data_flag = 0;

//默认波特率9600
void wf183d_gpio_config(void)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_WF183D_USART_TX_RCU);   // 开启串口时钟
	rcu_periph_clock_enable(BSP_WF183D_USART_RX_RCU);   // 开启端口时钟
	rcu_periph_clock_enable(BSP_WF183D_USART_RCU);      // 开启端口时钟
	
	/* 配置GPIO复用功能 */
    gpio_af_set(BSP_WF183D_USART_TX_PORT,BSP_WF183D_USART_AF,BSP_WF183D_USART_TX_PIN);	
	gpio_af_set(BSP_WF183D_USART_RX_PORT,BSP_WF183D_USART_AF,BSP_WF183D_USART_RX_PIN);	
	
	/* 配置GPIO的模式 */
	/* 配置TX为复用模式 上拉模式 */
	gpio_mode_set(BSP_WF183D_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_WF183D_USART_TX_PIN);
	/* 配置RX为复用模式 上拉模式 */
	gpio_mode_set(BSP_WF183D_USART_RX_PORT, GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_WF183D_USART_RX_PIN);
	
	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_WF183D_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_WF183D_USART_TX_PIN);
	/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_WF183D_USART_RX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_WF183D_USART_RX_PIN);

	/* 配置串口的参数 */
	usart_deinit(BSP_WF183D_USART);                                 // 复位串口
	usart_baudrate_set(BSP_WF183D_USART,9600);                      // 设置波特率
	usart_parity_config(BSP_WF183D_USART,USART_PM_NONE);            // 没有校验位
	usart_word_length_set(BSP_WF183D_USART,USART_WL_8BIT);          // 8位数据位
	usart_stop_bit_set(BSP_WF183D_USART,USART_STB_1BIT);     			 // 1位停止位

    /* 使能串口 */
	usart_enable(BSP_WF183D_USART);                                 // 使能串口
	usart_transmit_config(BSP_WF183D_USART,USART_TRANSMIT_ENABLE);  // 使能串口发送
	usart_receive_config(BSP_WF183D_USART,USART_RECEIVE_ENABLE);    // 使能串口接收
	
	/* 中断配置 */
	nvic_irq_enable(BSP_WF183D_USART_IRQ, 1, 2); 						     // 配置中断优先级
	usart_interrupt_enable(BSP_WF183D_USART,USART_INT_RBNE);				 // 读数据缓冲区非空中断和溢出错误中断
	usart_interrupt_enable(BSP_WF183D_USART,USART_INT_IDLE);				 // 空闲检测中断
}

//串口发送指定长度数据
void wf183d_send_command(uint8_t *ucstr, uint16_t length)
{
	while(length--)        
	{
        usart_data_transmit(BSP_WF183D_USART,*ucstr++);					    // 发送数据
        while(RESET == usart_flag_get(BSP_WF183D_USART,USART_FLAG_TBE));    // 等待发送数据缓冲区标志置位
	}
}

//清除串口接收全部缓存
void wf183d_data_clear(void)
{
    uint16_t i = DATA_LENGTH_MAX - 1;
    while( i )
    {
        AirPressure_Data_Buff[ i-- ] = 0;
    }       
    AirPressure_data_length = 0;
}

//CRC校验
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
 * 函 数 名 称：FrameResolution
 * 函 数 说 明：帧格式解析
 * 函 数 形 参：buff：帧数据
* 函 数 返 回：1：帧头不对  2:校准不对    其他：气压数据
 * 作       者：LC
 * 备       注：无
******************************************************************/
uint32_t FrameResolution(uint8_t *buff)
{
    uint32_t data = 0;
    //如果帧头不是0XAA
    if( buff[0] != 0xAA ) return 1; 
    //如果CRC校验的值跟接收的校验值不一致
    if( Cal_uart_buf_CRC( buff, 7 ) != buff[7] ) return 2;
    //数据整合
    data = buff[3] | (buff[4] << 8) | (buff[5] << 16) | (buff[6] << 24);
    
    return data;
}

uint8_t get_TemperatureCmd_buff[4] = {0X55, 0X04, 0X0E, 0X6A};
uint8_t get_AirPressureCmd_buff[4] = {0X55, 0X04, 0X0D, 0X88};

/******************************************************************
 * 函 数 名 称：GetAirPressureValue
 * 函 数 说 明：获取大气压值
 * 函 数 形 参：无
 * 函 数 返 回：大气压值，单位PA
 * 作       者：LCKFB
 * 备       注：由于转换压力需要根据当前温度进行补偿，所以需要先进行采集转换温度
******************************************************************/
uint32_t GetAirPressureValue(void)
{
    uint32_t AirPressureValue = 0;
    
    //发送采集温度命令
    wf183d_send_command( get_TemperatureCmd_buff, 4 );
    //等待数据接收完成
    while( AirPressure_data_flag != 1 );
    //清除接收完成标准位
    AirPressure_data_flag = 0;
    //清除当前接收到的温度数据帧
    wf183d_data_clear();
    
    //发送采集温度命令
    wf183d_send_command( get_AirPressureCmd_buff, 4 );
    //等待数据接收完成
    while( AirPressure_data_flag != 1 );
    //清除接收完成标准位
    AirPressure_data_flag = 0;
    //解析气压帧，并返回气压数据
    AirPressureValue = FrameResolution(AirPressure_Data_Buff);
    return AirPressureValue;
}

//串口1中断服务函数
void USART1_IRQHandler(void)
{
	if(usart_interrupt_flag_get(BSP_WF183D_USART,USART_INT_FLAG_RBNE) == SET)   // 接收缓冲区不为空
	{
		AirPressure_Data_Buff[AirPressure_data_length++] = usart_data_receive(BSP_WF183D_USART);// 把接收到的数据放到缓冲区中
	}
	
	if(usart_interrupt_flag_get(BSP_WF183D_USART,USART_INT_FLAG_IDLE) == SET)   // 检测到帧中断
	{
		usart_data_receive(BSP_WF183D_USART);                                   // 必须要读，读出来的值不能要
		AirPressure_Data_Buff[AirPressure_data_length] = '\0';// 数据接收完毕，数组结束标志
        AirPressure_data_flag = 1;
	}
	
}