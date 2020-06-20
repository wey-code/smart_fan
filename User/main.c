/*
 * main.c
 *
 *  Created on: 2020-4
 *      Author: Administrator
 */

extern unsigned int sdata;			//测量到的温度的整数部分
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "leds.h"
#include "time.h"
#include "uart.h"
#include "stdio.h"
#include "epwm.h"
#include "lcd9648.h"
#include "DS18B.h"
#include "smg.h"
#include "KEY.h"
#include "body.h"
#include "string.h"
#include "duoji.h"
#include "menu.h"
#include "oled.h"
#include "dc_motor.h"
// Prototype statements for functions found within this file.

char key_val=" ";
unsigned char Step_table_ZTurn[]={0xfff7,0xffdf,0xfffb,0xffef};
unsigned char Step_table_FTurn[]={0xffef,0xfffb,0xffdf,0xfff7};
interrupt void sciaRxFifoIsr(void);

void scia_fifo_init(void);
void error(void);

void scia_xmit(int a);
void scia_msg(char *msg);
void delay2(void)
{
    Uint16 		i;
	Uint32      j;
	for(i=0;i<32;i++)
		for (j = 0; j < 100000; j++);
}

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main()
{
	unsigned char i=0;
	unsigned char temp=0;
	char page=0;

	Uint16 ReceivedChar=0;
	char *msg;
	//float angle=0;

	InitSysCtrl();

	 InitSciGpio();
	DINT;

	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	 EALLOW;        // This is needed to write to EALLOW protected registers
	 PieVectTable.SCIRXINTA = &sciaRxFifoIsr;
	 //PieVectTable.SCITXINTA = &sciaTxFifoIsr;
	 IER |= M_INT9;    //开启CPU中断 组9  SCI_RX  CANB中断线1

	 PieCtrlRegs.PIEIER9.bit.INTx1=1;     //串口B接收中断  PIE Group 9, INT1

	 EDIS;   // This is needed to disable write to EALLOW protected registers
	 UARTa_Init(9600);

	Init_DS18B20();
	LED_Init();
	SMG_Init();
	TIM0_Init(150,20000);//200ms
	TIM1_Init(750,200000);//2s
	KEY_Init();
	body_init();




	//LCD9648_Init();
	//LCD9648_Clear();
	//LCD9648_Write16EnCHAR(0,0,"you are");

	EPWM6_Init(28125);
	EPWM2_Init(4000);
	EPwm2A_SetCompare(0);
	EPwm2B_SetCompare(3999);
	//DC_Motor_Init();



	msg = "Hello World!\r\n";
	UARTa_SendString(msg);
	msg = "Welcome to use new intelligent electric fan!\r\n";
	UARTa_SendString(msg);





	//delay_ms(5000);
    OLED_Init();
    OLED_Clear();
	//oled_first_show();
	while(1)
	{
		/*
		switch(page)
		{
		case 0:LCD9648_Write16EnCHAR(0,0,"  WELCOME");
				LCD9648_Write16EnCHAR(0,2,"1-normal");
				LCD9648_Write16EnCHAR(0,4,"2-smart ");
				break;
		case 1:	LCD9648_Write16EnCHAR(0,4,"3-status ");
		}
		*/
				OLED_ShowString(20,0,"WELCOME",16);
				OLED_ShowString(0,3,"1-normal",12);
				OLED_ShowString(0,5,"2-smart ",12);
				OLED_ShowString(0,7,"3-state",12);




		{
			//set_angle(180);
			DELAY_US(100000);
		}


		key_val=KEY_Scan(0);

		switch(key_val)
		{
		    case '1': normal_serve();break;
		    case '2': smart_serve();break;
		    case '3': state();break;
		    case 'A':  if(page<=0)
		    				page=0;
		    			else
		    				page--;
		    			LCD9648_Clear();break;
		    case 'B':  if(page>=1)
		    				page=1;
		    			else
		    				page++;
		    			LCD9648_Clear();break;
			default: break;
		}


		if(key_val!=' ')
		{
			LCD9648_Write16EnCHAR(0,4,&key_val);
		}
		//if(BODY_DATA)
		//	LCD9648_Write16EnCHAR(0,0,"invade");
		//else
		//	LCD9648_Write16EnCHAR(0,0,"else");
		//for(i = 0; i < 5; i+=2)
		//{
			//LCD9648_Write16CnCHAR(15,i,"普中科技");
			//DELAY_US(1000);
			//LCD9648_Clear();
			//temp = ReadTemperature();
			//SMG_DisplayInt(temp);

		//}
//		LCD9648_ClearTest();
	}
}


