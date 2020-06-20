

#ifndef DS18B_H_
#define DS18B_H_


#include "DSP2833x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"   // DSP2833x �������ͷ�ļ�


	//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#define  DQ_DIR GpioCtrlRegs.GPCDIR.bit.GPIO64  //����
#define  DQ    GpioDataRegs.GPCDAT.bit.GPIO64//��ֵ

uchar Init_DS18B20();
uchar ReadOneChar(void);
void WriteOneChar(uchar dat);
unsigned int ReadTemperature();



#endif /* DS18B_H_ */
