/*
 * beep.c
 *
 *  Created on: 2018-1-20
 *      Author: Administrator
 */

#include "beep.h"

/*******************************************************************************
* �� �� ��         : BEEP_Init
* ��������		   : ��������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void BEEP_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
	//BEEP�˿�����
	GpioCtrlRegs.GPAMUX1.bit.GPIO6=0;
	GpioCtrlRegs.GPADIR.bit.GPIO6=1;
	GpioCtrlRegs.GPAPUD.bit.GPIO6=0;

	EDIS;

	GpioDataRegs.GPACLEAR.bit.GPIO6=1;

}

