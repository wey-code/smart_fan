#include "time.h"
#include "leds.h"
#include "duoji.h"
#include "dc_motor.h"

Uint32 turn_count=0;
char _turn_en=0;
char angle_dir=0;
extern unsigned int _set_time;
float pass_time=0;
unsigned int temperature=0;
float angle_range=180;
float angle=90;
extern char gear;

extern char _set_time_en;
void TIM0_Init(float Freq, float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	EDIS;

	EALLOW;
	PieVectTable.TINT0 = &TIM0_IRQn;
	EDIS;

	// CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer0, Freq, Period);

	CpuTimer0Regs.TCR.bit.TSS=0;

	IER |= M_INT1;

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	EINT;
	ERTM;

}

interrupt void TIM0_IRQn(void)
{
	EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK1=1;
	EDIS;
	if(_turn_en==0)
		set_angle(90);
	else
	{
		if(angle_dir==0)
			angle+=1;
		else
			angle-=1;
		if(angle<0)
			{angle=0;angle_dir=0;}
		if(angle>180)
			{angle=180;angle_dir=1;}
		set_angle(angle);

	}

	CpuTimer0Regs.TCR.bit.TIF=1;
	CpuTimer0Regs.TCR.bit.TRB=1;


}


void TIM1_Init(float Freq, float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
	EDIS;

	EALLOW;
	PieVectTable.XINT13 = &TIM1_IRQn;
	EDIS;

	// Initialize address pointers to respective timer registers:
	CpuTimer1.RegsAddr = &CpuTimer1Regs;
	// Initialize timer period to maximum:
	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer1Regs.TPR.all  = 0;
	CpuTimer1Regs.TPRH.all = 0;
	// Make sure timers are stopped:
	CpuTimer1Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer1Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer1.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer1, Freq, Period);

	CpuTimer1Regs.TCR.bit.TSS=0;

	IER |= M_INT13;

	EINT;
	ERTM;

}

//extern unsigned char sec;
//extern unsigned char mms;

extern char _auto_gear;
extern char _auto_off;
#include "body.h"
#include "DS18B.h"
#include "LED.h"

interrupt void TIM1_IRQn(void)
{
	static float j=0;
	float auto_gear_temp=0;
	EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK1=1;
	EDIS;

	//每5s读取一次温度
	j++;
	if(j>5)
	{
	temperature=ReadTemperature();
	j=0;
	}

	if(BODY_DATA==0)
	{
		LED3_OFF;
		if(_auto_off==1)

		{
		EPwm2B_SetCompare(3999);
		return 0;
		}
	}
	else
	{
	LED3_ON	;
	}
	//自动档位设置
	if(_auto_gear==1)
	{
		if(temperature<27)
			{EPwm2B_SetCompare(2500);}
		if(temperature>=27&&temperature<=32)
			{auto_gear_temp=(int)(2500-2000*(temperature-27)/5);
			EPwm2B_SetCompare((int)auto_gear_temp);
			}
		if(temperature>32)
			{EPwm2B_SetCompare(500);}
	}
	else
	{
		switch(gear)
		{
		case 0:EPwm2B_SetCompare(3999);break;
		case 1:EPwm2B_SetCompare(2500);break;
		case 2:EPwm2B_SetCompare(1500);break;
		case 3:EPwm2B_SetCompare(500);break;
		}
	}



	if(_set_time_en==0)
	{
		pass_time=0;
	}
	else
	{
		pass_time++;
	}

	if(pass_time>=_set_time&&_set_time_en!=0)
	{
		gear=0;
		_set_time_en=0;
		_set_time=0;
		_auto_gear=0;
	}


	CpuTimer1Regs.TCR.bit.TIF=1;
	CpuTimer1Regs.TCR.bit.TRB=1;

}



void TIM2_Init(float Freq, float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2
	EDIS;

	EALLOW;
	PieVectTable.TINT2 = &TIM2_IRQn;
	EDIS;

	// Initialize address pointers to respective timer registers:
	CpuTimer2.RegsAddr = &CpuTimer2Regs;
	// Initialize timer period to maximum:
	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer2Regs.TPR.all  = 0;
	CpuTimer2Regs.TPRH.all = 0;
	// Make sure timers are stopped:
	CpuTimer2Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer2Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer2.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer2, Freq, Period);

	CpuTimer2Regs.TCR.bit.TSS=0;

	IER |= M_INT14;

	EINT;
	ERTM;

}

interrupt void TIM2_IRQn(void)
{
	EALLOW;
	LED4_TOGGLE;
	EDIS;

}
