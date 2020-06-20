
#include "DS18B.h"


uchar Init_DS18B20()
{
    EALLOW;
    DQ_DIR=1;     //设置为输出状态
    EDIS;

    DQ=0;        //拉低电平
    DELAY_US(650);//延迟650us

    DQ=1;       //释放总线
    DELAY_US(15);//延迟15us

    EALLOW;
    DQ_DIR=0;   //设置为输入状态
    EDIS;

    while (DQ)
    {
    	DELAY_US(5000);//延迟5ms
    	return 0;      //初始化失败
    }

   return 1;//成功

}

/*************************************************/
/*                读字节子函数                  */
/*************************************************/
uchar ReadOneChar(void)
  {

    uchar i=0,dat=0;
    for (i=8;i>0;i--)
    {

        EALLOW;
        DQ_DIR=1;     //设置为输出状态
        EDIS;

        DQ=0;
        DELAY_US(5);//延迟5us

        DQ=1;
		DELAY_US(6);//延迟6us,等待数据稳定


		dat>>=1;//读到数据后右移一位

		EALLOW;
		DQ_DIR=0; //设置为输入状态
		EDIS;

        if(DQ)
        	dat|=0x80;//取回最高位数据
        DELAY_US(60);//延迟60us
    }

   return dat;

}

 /*************************************************/
 /*                写字节子函数                  */
 /*************************************************/
void WriteOneChar(uchar dat)
{
    uchar i;

	for(i=8;i>0;i--)
	{
		EALLOW;
		DQ_DIR=1; //设置为输出状态
		EDIS;

		DQ=0;
		DELAY_US(5);//延迟5us

		DQ=dat & 0x01;
		DELAY_US(68);//延迟68us

		DQ=1;
		DELAY_US(5);//延迟5us
		dat >>=1;
	}
 }

//读取温度
unsigned int ReadTemperature()
{

	uchar a=0;
	uchar b=0;
	float t=0;
	uchar temp;
	unsigned char tempL=0; 		//设全局变量
	unsigned char tempH=0;
	unsigned int sdata;			//测量到的温度的整数部分
	unsigned char xiaoshu1;		//小数第一位
	unsigned char xiaoshu2;		//小数第二位
	unsigned char xiaoshu;		//两位小数

	Init_DS18B20();
	DELAY_US(1000);//延迟1ms
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换

	Init_DS18B20();
	DELAY_US(1000);//延迟1ms
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0xbe); //跳过读序号列号的操作

	a=ReadOneChar();//低字节
	b=ReadOneChar();//高字节

	temp=b;
	temp<<=8;
	temp=temp|a;
	t=temp*0.0625;  //得到的是实际温度

	if(b>0x7f)      				//最高位为1时温度是负
		{
			a=~a;					//补码转换，取反加一
			b=~b+1;
			//fg=0;      						//读取温度为负时fg=0
		}
		sdata = a/16+b*16;      	//整数部分
		xiaoshu1 = (a&0x0f)*10/16; 		//小数第一位
		xiaoshu2 = (a&0x0f)*100/16%10;	//小数第二位
		xiaoshu=xiaoshu1*10+xiaoshu2; 		//小数两位

	return(sdata);

}

