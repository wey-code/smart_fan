
#include "DS18B.h"


uchar Init_DS18B20()
{
    EALLOW;
    DQ_DIR=1;     //����Ϊ���״̬
    EDIS;

    DQ=0;        //���͵�ƽ
    DELAY_US(650);//�ӳ�650us

    DQ=1;       //�ͷ�����
    DELAY_US(15);//�ӳ�15us

    EALLOW;
    DQ_DIR=0;   //����Ϊ����״̬
    EDIS;

    while (DQ)
    {
    	DELAY_US(5000);//�ӳ�5ms
    	return 0;      //��ʼ��ʧ��
    }

   return 1;//�ɹ�

}

/*************************************************/
/*                ���ֽ��Ӻ���                  */
/*************************************************/
uchar ReadOneChar(void)
  {

    uchar i=0,dat=0;
    for (i=8;i>0;i--)
    {

        EALLOW;
        DQ_DIR=1;     //����Ϊ���״̬
        EDIS;

        DQ=0;
        DELAY_US(5);//�ӳ�5us

        DQ=1;
		DELAY_US(6);//�ӳ�6us,�ȴ������ȶ�


		dat>>=1;//�������ݺ�����һλ

		EALLOW;
		DQ_DIR=0; //����Ϊ����״̬
		EDIS;

        if(DQ)
        	dat|=0x80;//ȡ�����λ����
        DELAY_US(60);//�ӳ�60us
    }

   return dat;

}

 /*************************************************/
 /*                д�ֽ��Ӻ���                  */
 /*************************************************/
void WriteOneChar(uchar dat)
{
    uchar i;

	for(i=8;i>0;i--)
	{
		EALLOW;
		DQ_DIR=1; //����Ϊ���״̬
		EDIS;

		DQ=0;
		DELAY_US(5);//�ӳ�5us

		DQ=dat & 0x01;
		DELAY_US(68);//�ӳ�68us

		DQ=1;
		DELAY_US(5);//�ӳ�5us
		dat >>=1;
	}
 }

//��ȡ�¶�
unsigned int ReadTemperature()
{

	uchar a=0;
	uchar b=0;
	float t=0;
	uchar temp;
	unsigned char tempL=0; 		//��ȫ�ֱ���
	unsigned char tempH=0;
	unsigned int sdata;			//���������¶ȵ���������
	unsigned char xiaoshu1;		//С����һλ
	unsigned char xiaoshu2;		//С���ڶ�λ
	unsigned char xiaoshu;		//��λС��

	Init_DS18B20();
	DELAY_US(1000);//�ӳ�1ms
	WriteOneChar(0xCC); // ����������кŵĲ���
	WriteOneChar(0x44); // �����¶�ת��

	Init_DS18B20();
	DELAY_US(1000);//�ӳ�1ms
	WriteOneChar(0xCC); // ����������кŵĲ���
	WriteOneChar(0xbe); //����������кŵĲ���

	a=ReadOneChar();//���ֽ�
	b=ReadOneChar();//���ֽ�

	temp=b;
	temp<<=8;
	temp=temp|a;
	t=temp*0.0625;  //�õ�����ʵ���¶�

	if(b>0x7f)      				//���λΪ1ʱ�¶��Ǹ�
		{
			a=~a;					//����ת����ȡ����һ
			b=~b+1;
			//fg=0;      						//��ȡ�¶�Ϊ��ʱfg=0
		}
		sdata = a/16+b*16;      	//��������
		xiaoshu1 = (a&0x0f)*10/16; 		//С����һλ
		xiaoshu2 = (a&0x0f)*100/16%10;	//С���ڶ�λ
		xiaoshu=xiaoshu1*10+xiaoshu2; 		//С����λ

	return(sdata);

}

