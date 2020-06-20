/*
 * lcd1602.c
 *
 *  Created on: 2018-2-28
 *      Author: Administrator
 */

#include "lcd1602.h"


void LCD1602_GPIOInit(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // ʹ��GPIO0 �����ڲ�����
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   // ��ֹGPIO1 �����ڲ�����
	GpioCtrlRegs.GPAMUX1.all = 0;   // ����GPIO0-GPIO15Ϊͨ��I/O��
	GpioCtrlRegs.GPADIR.all = 0x00FFF;// ����GPIO0-GPIO11Ϊ�������

	// ÿ������ڿ����в�ͬ�������޶�
	// a) ������ϵͳʱ�� SYSCLKOUTͬ��
	// b) ���뱻ָ���Ĳ��������޶�
	// c) �����첽 (��������������Ч)
	GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15��ϵͳʱ��SYSCLKOUT ͬ��
	//�������LCD_RS��LCD_EN����
	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;

	EDIS;
}

void LCD1602_WriteCmd(unsigned char cmd)
{
	LCD1602_EN_SETL;
	LCD1602_RS_SETL;
	LCD1602_DATAPORT=cmd<<2;
	DELAY_US(500);
	LCD1602_EN_SETH;
	DELAY_US(1000);
	LCD1602_EN_SETL;
}

void LCD1602_WriteData(unsigned char dat)
{
	LCD1602_EN_SETL;
	LCD1602_RS_SETH;
	LCD1602_DATAPORT=dat<<2|0x0001;
	DELAY_US(500);
	LCD1602_EN_SETH;
	DELAY_US(1000);
	LCD1602_EN_SETL;
}

void LCD1602_Init(void)
{
	LCD1602_GPIOInit();

	DELAY_US(15000);//�ӳ�15ms
	LCD1602_WriteCmd(0x38);//����8λ��ʽ��2�У�5x7
	DELAY_US(5000);//�ӳ�5ms
	LCD1602_WriteCmd(0x38);//����8λ��ʽ��2�У�5x7
	DELAY_US(5000);
	LCD1602_WriteCmd(0x38);//����8λ��ʽ��2�У�5x7
	LCD1602_WriteCmd(0x38);//����8λ��ʽ��2�У�5x7
	LCD1602_WriteCmd(0x08);//����ʾ������ʾ��꣬��겻��˸��
	LCD1602_WriteCmd(0x01);//�����Ļ��ʾ������ָ�����㣬������ʾ���㣻
	LCD1602_WriteCmd(0x06);//�趨���뷽ʽ����������λ
	LCD1602_WriteCmd(0x0c);//������ʾ���ع�꣬����˸
}

void LCD1602_DispString(char line,char *str)
{
	if(line==1)
		LCD1602_WriteCmd(0x80);
	else if(line==2)
		LCD1602_WriteCmd(0x80+0x40);
	else
		return;
	while(*str!='\0')
	{
		LCD1602_WriteData(*str);
		DELAY_US(500);  //��ʱ500us
		str++;
	}
}

void LCD1602_DispStringEx(char line,char x,char *str)
{
	if(line==1)
		LCD1602_WriteCmd(0x80+x);
	else if(line==2)
		LCD1602_WriteCmd(0x80+0x40+x);
	else
		return;
	while(*str!='\0')
	{
		LCD1602_WriteData(*str);
		DELAY_US(500);  //��ʱ500us
		str++;
	}
}
