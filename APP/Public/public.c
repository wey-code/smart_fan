/*
 * public.c
 *
 *  Created on: 2019��7��10��
 *      Author: Administrator
 */


#include "public.h"

void delay(Uint16 time)                     //��ʱ����
{
    for(; time>0 ; time--)
    {
        asm(" nop");
    }
}

void  delay_ms(unsigned int nDelay)
{
    int ii,jj,kk=0;
    for ( ii=0;ii<nDelay;ii++ )
    {
        for ( jj=0;jj<512;jj++ )
        {
            kk++;
        }
    }
}
