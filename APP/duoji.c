

#include "duoji.h"
#include "epwm.h"

void set_angle(float a)
{
	float b;
	Uint16 val;
	b = a*0.0037037+0.16667;
	val = b*28125;
	EPwm6A_SetCompare(val);
}
