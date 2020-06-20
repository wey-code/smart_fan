

#ifndef BODY_H_
#define BODY_H_


#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件

#define BODY_DATA	GpioDataRegs.GPCDAT.bit.GPIO69

void body_init();


#endif /* BODY_H_ */
