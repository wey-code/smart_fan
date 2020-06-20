/*
 * public.h
 *
 *  Created on: 2019��7��10��
 *      Author: Administrator
 */

#ifndef APP_PUBLIC_PUBLIC_H_
#define APP_PUBLIC_PUBLIC_H_


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "stdlib.h"

#define  u8 unsigned char
#define  u32 unsigned int
#define u16 Uint16


void delay(Uint16 time);                    //��ʱ����
void delay_ms(unsigned int nDelay);


#endif /* APP_PUBLIC_PUBLIC_H_ */
