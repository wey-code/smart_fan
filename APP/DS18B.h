

#ifndef DS18B_H_
#define DS18B_H_


#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件


	//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#define  DQ_DIR GpioCtrlRegs.GPCDIR.bit.GPIO64  //方向
#define  DQ    GpioDataRegs.GPCDAT.bit.GPIO64//数值

uchar Init_DS18B20();
uchar ReadOneChar(void);
void WriteOneChar(uchar dat);
unsigned int ReadTemperature();



#endif /* DS18B_H_ */
