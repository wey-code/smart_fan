#include "uart.h"
#include "stdio.h"
#include <string.h>



void UARTa_Init(Uint32 baud)
{
	unsigned char scihbaud=0;
	unsigned char scilbaud=0;
	Uint16 scibaud=0;

	scibaud=37500000/(8*baud)-1;
	scihbaud=scibaud>>8;
	scilbaud=scibaud&0xff;


	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
	EDIS;

	InitSciaGpio();

	//Initalize the SCI FIFO
	//SciaRegs.SCIFFTX.all=0xE040;
	//SciaRegs.SCIFFRX.all=0x204f;

	   //SCI发送FIFO寄存器SCIFFTX
	   SciaRegs.SCIFFTX.bit.SCIFFENA=1;//使能FIFO
	   SciaRegs.SCIFFTX.bit.SCIRST=1;//SCI复位标志位，1.SCI接收和发送FIFO功能继续工作
	   SciaRegs.SCIFFTX.bit.TXFFIENA=0;//不使能发送FIFO中断
	   SciaRegs.SCIFFTX.bit.TXFFIL=0x0C;//12级FIFO使能
	   SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;//发送FIFO中断清除标志位，1，清除TXFFINT位
	   SciaRegs.SCIFFTX.bit.TXFIFOXRESET=0;//SCI发送FIFO复位，0，复位发送FIFO指针

	   //SCI接收FIFO寄存器SCIFFRX
	   SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;//SCI接收FIFO溢出清除标志位，1，清除RXFFOVF
	   SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;//接收FIFO中断清除标志位
	   SciaRegs.SCIFFRX.bit.RXFIFORESET=0;//SCI接收FIFO复位，0，复位接收FIFO指针
	   SciaRegs.SCIFFRX.bit.RXFFIENA=1;//接收FIFO中断使能位，
	   SciaRegs.SCIFFRX.bit.RXFFIL=0x01;//12级FIFO使能

	  // SciaRegs.SCIFFRX.all=0x0021;

	SciaRegs.SCIFFCT.all=0x0;

	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function
	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
								   // No parity,8 char bits,
								   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	SciaRegs.SCIHBAUD    =scihbaud;  // 9600 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD    =scilbaud;
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

	   SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	   SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

}


// Transmit a character from the SCI'
void UARTa_SendByte(int a)
{
	while (SciaRegs.SCIFFTX.bit.TXFFST != 0);
	SciaRegs.SCITXBUF=a;
}

void UARTa_SendString(char * msg)
{
	int i=0;

	while(msg[i] != '\0')
	{
		UARTa_SendByte(msg[i]);
		i++;
	}
}

void UARTa_SendString2(char * msg)
{
	int i=0;

	while(msg[i] != '\r')
	{
		UARTa_SendByte(msg[i]);
		i++;
	}
}

#include "stdlib.h"

extern unsigned int temperature;
extern char _turn_en;
extern char gear;
extern char _auto_off;
extern char _auto_gear;
extern float pass_time;
extern unsigned int _set_time;
extern char _set_time_en;
interrupt void sciaRxFifoIsr(void)
{
        Uint16 i;
        static Uint16 j=0;
        static char receive_m[32]={0};
        char *msg;
        char gear_char=0;
        char tmeperature_char[2]={0};
        char time_char[3]={0};
        char remain_time_char[3]={0};
        i = SciaRegs.SCIRXBUF.all;

    	receive_m[j]=i;
    	j++;

        if(i=='\r')
        {
        //msg = "\r\nYou just sent the following characters : ";
        //UARTa_SendString(msg);
        //UARTa_SendString2(receive_m);
        //UARTa_SendString2("\r\n");

        /*下面开始解析收到的命令
        1.挡位设置 gear：0/1/2/3		0 1 2 3
        2.摇头设置 turn:on/off			4 5
        3.自动关闭 auto_off:on/off		6 7
        4.自动挡位 auto_gear:on/off		8 9
        5.定时关闭 set:time
        6.反馈现在状态 					a
        */

        switch(receive_m[0])
        {
        case '0':gear=0;UARTa_SendString("gear=0\r\n");break;
        case '1':gear=1;UARTa_SendString("gear=1\r\n");break;
        case '2':gear=2;UARTa_SendString("gear=2\r\n");break;
        case '3':gear=3;UARTa_SendString("gear=3\r\n");break;
        case '4':_turn_en=1;UARTa_SendString("turn: on\r\n");break;
        case '5':_turn_en=0;UARTa_SendString("turn: off\r\n");break;
        case '6':_auto_off=1;UARTa_SendString("auto_off: on\r\n");break;
        case '7':_auto_off=0;UARTa_SendString("auto_off: off\r\n");break;
        case '8':_auto_gear=1;UARTa_SendString(" auto_gear: on\r\n");break;
        case '9':_auto_gear=0;UARTa_SendString("auto_gear: off\r\n");break;
        case 's':	_set_time=atoi(receive_m+2);
        			pass_time=0;
        			_set_time_en=1;
        			UARTa_SendString("enable set time:");
        			sprintf(time_char,"%d",_set_time);
        			UARTa_SendString(time_char);
        			UARTa_SendString("\r\n");break;
        case 'b':_set_time=0;_set_time_en=0;
        			UARTa_SendString("disable set time\r\n");break;

        case 'a':UARTa_SendString("----------------------------\r\n");
        		  UARTa_SendString("now situation:\r\n");
        		  //挡位是否开启
        		  if(_auto_gear==0)
        		  {
        			  //itoa(gear,gear_char,1);
        			  sprintf(&gear_char,"%d",gear);
        			  UARTa_SendString("gear:");
        			  UARTa_SendString(&gear_char);UARTa_SendString("\r\n");

        		  }//OLED_ShowNumber(50,3,gear,1,12);
        		  else
        			  UARTa_SendString("gear:auto\r\n");
        		  		//LED_ShowString(50,3,"auto",12);
        		  //是否开启自动摇头
        		  if(_turn_en==1)
        			  UARTa_SendString("turn:on\r\n");
        		  else
        			  UARTa_SendString("turn:off\r\n");

        		  if(_auto_off==1)
        			  UARTa_SendString("auto_off:on\r\n");
        		  else
        			  UARTa_SendString("auto_off:off\r\n");

        		  if(_auto_gear==1)
        		  {
        			  UARTa_SendString("auto_gear:on\r\n");

        		  }
        		  else{
        			  UARTa_SendString("auto_gear:off\r\n");
        		  }
        		  //打印温度
        		  UARTa_SendString("temperature:");
        		  sprintf(tmeperature_char,"%d",temperature);
        		  UARTa_SendString(tmeperature_char);UARTa_SendString("\r\n");

        		  //定时功能
        		  if(_set_time_en==1)
        		  {
        			  sprintf(time_char,"%d",_set_time);
        			  sprintf(remain_time_char,"%d",(int)(_set_time-pass_time));
        			  UARTa_SendString("set time:"); UARTa_SendString(time_char);UARTa_SendString("\r\n");
        			  UARTa_SendString("remain time:"); UARTa_SendString(remain_time_char);UARTa_SendString("\r\n");
        		  }
        		  else
        			  UARTa_SendString("set time:off\r\n");
        		  UARTa_SendString("----------------------------\r\n");

        		  break;
        default :break;
        }



        strcpy(receive_m,"");
        j=0;
        }



        SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;
//        ScibRegs.SCIFFTX.bit.TXFFINTCLR=1;
        PieCtrlRegs.PIEACK.bit.ACK9=1;
        //PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

