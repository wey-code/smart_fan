/*
 * dc_motor.c
 *
 *  Created on: 2018-1-23
 *      Author: Administrator
 */


#include "dc_motor.h"


void DC_Motor_Init(void)
{
	EALLOW;

	//DC_MOTOR端口配置--1路
	GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
	GpioCtrlRegs.GPADIR.bit.GPIO2=1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
	GpioCtrlRegs.GPADIR.bit.GPIO3=1;

/*
	//DC_MOTOR端口配置--2路
	GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;
	GpioCtrlRegs.GPADIR.bit.GPIO4=1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO5=0;
	GpioCtrlRegs.GPADIR.bit.GPIO5=1;
*/
	EDIS;

	GpioDataRegs.GPACLEAR.bit.GPIO2=1;
	GpioDataRegs.GPASET.bit.GPIO3=1;

	/*
	GpioDataRegs.GPACLEAR.bit.GPIO4=1;
	GpioDataRegs.GPACLEAR.bit.GPIO5=1;
	*/
}







// 宏定义每个定时器周期寄存器的周期值；
#define EPWM2_TIMER_TBPRD  3750  // 周期值
#define EPWM2_MAX_CMPA     3700
#define EPWM2_MIN_CMPA       0
#define EPWM2_MAX_CMPB     3700
#define EPWM2_MIN_CMPB       0

/***************全局变量定义****************/
Uint16 pwm_stepValue=0;  //高电平时间
Uint16 Direction=0;//转速方向

void EPWM2_Init(Uint16 tbprd)
{
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Disable TBCLK within the ePWM
	SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;  // ePWM6
	EDIS;

	InitEPwm2Gpio();

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
	EDIS;

	// Setup Sync
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;  // Pass through
	// Allow each timer to be sync'ed
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;
	EPwm2Regs.TBPHS.half.TBPHS = 0;
	EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm2Regs.TBPRD = tbprd;
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
	EPwm2Regs.TBCTL.bit.HSPCLKDIV=TB_DIV4;
	EPwm2Regs.TBCTL.bit.CLKDIV=TB_DIV4;

	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	// Set Compare values
	EPwm2Regs.CMPA.half.CMPA = 0;    // Set compare A value
	EPwm2Regs.CMPB = 0;              // Set Compare B value

	// Set actions
	EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // Set PWM1A on Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;          // Clear PWM1A on event A, up count
	EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // Set PWM1B on Zero
	EPwm2Regs.AQCTLB.bit.CBU = AQ_SET;          // Clear PWM1B on event B, up count

	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTEN = 1;  // Enable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
	EDIS;
}

void EPwm2A_SetCompare(Uint16 val)
{
	EPwm2Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm2B_SetCompare(Uint16 val)
{
	EPwm2Regs.CMPB = val;  //设置占空比
}

