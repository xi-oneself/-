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

#ifndef _LCD_GUI_H
#define _LCD_GUI_H


#include "gd32f4xx.h"
#include "systick.h"


void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//ָ�����������ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//��ָ��λ�û�һ������
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ���ִ�
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����12x12����
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����32x32����

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾһ���ַ�
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ�ַ���
uint32_t mypow(uint8_t m,uint8_t n);//����
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//��ʾͼƬ

void LCD_DrawPointFlush( uint16_t x, uint16_t y, uint16_t color);
//������ɫ
#define WHITE            0xFFFF
#define BLACK            0x0000  
#define BLUE             0x1F00  
#define BRED             0X1FF8
#define GRED             0XE0FF
#define GBLUE            0XFF07
#define RED              0x00F8
#define MAGENTA          0x1FF8
#define GREEN            0xE007
#define CYAN             0xFF7F
#define YELLOW           0xE0FF
#define BROWN            0X40BC //��ɫ
#define BRRED            0X07FC //�غ�ɫ
#define GRAY             0X3084 //��ɫ
#define DARKBLUE         0XCF01 //����ɫ
#define LIGHTBLUE        0X7C7D //ǳ��ɫ  
#define GRAYBLUE         0X5854 //����ɫ
#define LIGHTGREEN       0X1F84 //ǳ��ɫ
#define LGRAY            0X18C6 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0X51A6 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X122B //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif