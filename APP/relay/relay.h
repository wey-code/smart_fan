/*
 * relay.h
 *
 *  Created on: 2018-1-22
 *      Author: Administrator
 */

#ifndef RELAY_H_
#define RELAY_H_



#include "DSP2833x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"   // DSP2833x �������ͷ�ļ�


#define RELAY_ON			(GpioDataRegs.GPASET.bit.GPIO15=1)
#define RELAY_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO15=1)


void Relay_Init(void);


#endif /* RELAY_H_ */
