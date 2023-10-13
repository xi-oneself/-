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

#ifndef __LCDINIT_H__
#define __LCDINIT_H__

#include "gd32f4xx.h"
#include "systick.h"
#include "exmc_sdram.h"

#define USE_SOFTWARE   0   //是否使用软件SPI 0使用硬件SPI  1使用软件SPI

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280
#else
#define LCD_W 280
#define LCD_H 240
#endif

#define LCD_RAM_NUMBER  67200	//240x280=67200

extern __align(32) uint16_t Show_Gram[LCD_RAM_NUMBER]  __attribute__((at(SDRAM_DEVICE0_ADDR)));  

//GND - GND
//VCC - 3.3V
//SCL - PB3     SPI0_SCK
//SDA - PB5     SPI0_MOSI
//RES - PG14    (可以接入复位)
//DC  - PB5
//CS  - PB6     
//BLK - PB8     TIMER1_CH0

/**************     LCD端口移植    *******************/
#define RCU_LCD_SCL  	    RCU_GPIOB				//SCK
#define PORT_LCD_SCL 	    GPIOB
#define GPIO_LCD_SCL 	    GPIO_PIN_3

#define RCU_LCD_SDA  	    RCU_GPIOB				//MOSI
#define PORT_LCD_SDA 	    GPIOB
#define GPIO_LCD_SDA 	    GPIO_PIN_5

#define RCU_LCD_CS  	    RCU_GPIOB				//NSS
#define PORT_LCD_CS 	    GPIOB
#define GPIO_LCD_CS 	    GPIO_PIN_6

#define RCU_LCD_DC		    RCU_GPIOB				//DC
#define PORT_LCD_DC 	    GPIOB
#define GPIO_LCD_DC 	    GPIO_PIN_4

#define RCU_LCD_RES		    RCU_GPIOG				//RES
#define PORT_LCD_RES 	    GPIOG
#define GPIO_LCD_RES 	    GPIO_PIN_14

#define RCU_LCD_BLK		    RCU_GPIOB				//BLK
#define PORT_LCD_BLK 	    GPIOB
#define GPIO_LCD_BLK 	    GPIO_PIN_8

/**************     如使用硬件SPI请修改此处    *******************/
#define RCU_SPI_HARDWARE  	RCU_SPI0
#define PORT_SPI 			SPI0
#define LINE_AF_SPI 		GPIO_AF_5





/**************     LCD端口输出定义    *******************/
#define LCD_SCLK_Clr() gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, RESET)//SCL=SCLK
#define LCD_SCLK_Set() gpio_bit_write(PORT_LCD_SCL, GPIO_LCD_SCL, SET)

#define LCD_MOSI_Clr() gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, RESET)//SDA=MOSI
#define LCD_MOSI_Set() gpio_bit_write(PORT_LCD_SDA, GPIO_LCD_SDA, SET)

#define LCD_RES_Clr()  gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, RESET)//RES
#define LCD_RES_Set()  gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, SET)

#define LCD_DC_Clr()   gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, RESET)//DC
#define LCD_DC_Set()   gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET)
 		      
#define LCD_CS_Clr()   gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, RESET)//CS
#define LCD_CS_Set()   gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET)

#define LCD_BLK_Clr()  gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, RESET)//BLK
#define LCD_BLK_Set()  gpio_bit_write(PORT_LCD_BLK, GPIO_LCD_BLK, SET)



void lcd_gpio_config(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);


void Lcd_Gram_Fill(uint16_t color);
void LCD_Show_Gram(void);
void Lcd_Show_Time_config(void);

uint8_t get_show_over_flag(void);
void set_show_over_flag(uint8_t flag);
uint8_t get_show_update_flag(void);
void set_show_update_flag(uint8_t flag);
void set_lcd_backlight(uint32_t value);
#endif