

#include "body.h"

void body_init()
{

	GpioCtrlRegs.GPCMUX1.bit.GPIO69=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO69=0;
	GpioCtrlRegs.GPCPUD.bit.GPIO69=0;
}
