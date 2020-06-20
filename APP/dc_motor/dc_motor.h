/*
 * dc_motor.h
 *
 *  Created on: 2018-1-23
 *      Author: Administrator
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_


#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件




void EPWM2_Init(Uint16 tbprd);
void EPwm2A_SetCompare(Uint16 val);
void EPwm2B_SetCompare(Uint16 val);
void DC_Motor_Init(void);
#endif /* DC_MOTOR_H_ */
