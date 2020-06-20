

#include "menu.h"
#include "lcd9648.h"
#include "KEY.h"
#include "oled.h"
#include "dc_motor.h"
extern char key_val;
extern char _turn_en;
char _auto_gear=0;
char _auto_off=0;
char _set_time_en=0;
unsigned int _set_time=0;
//unsigned int temperature=0;
//unsigned int _set_time=0;
extern unsigned int temperature;
char gear=0;
char menu=0;
extern float pass_time;
#include "body.h"


//普通模式三级子菜单
void gear_set()
{
	/*
	LCD9648_Clear();
	LCD9648_Write16EnCHAR(0,0,"set_gear");
	LCD9648_Write16EnCHAR(0,0,"input:");
	LCD9648_Write16EnCHAR(0,0,"0,1,2,3");
	*/

	OLED_Clear();
	OLED_ShowString(20,0,"SET_GEAR",16);
	OLED_ShowString(0,3,"input:0,1,2,3",12);

	//OLED_ShowString(0,5,"input # to return",12);

	menu=2;
	while(1)
	{
		key_val=KEY_Scan(0);
		switch(key_val)
		{
			case '0': ;gear = 0;menu=1;break;
			case '1': ;gear = 1;menu=1;break;
			case '2': ;gear = 2;menu=1;break;
			case '3': ;gear = 3;menu=1;break;
			case '#': ;menu=1;break;
			default:;
		}
		if(menu==1)
			{//LCD9648_Clear();
			OLED_Clear();
			break;}
	}
}

void turn_set()
{
	/*
	LCD9648_Clear();
	LCD9648_Write16EnCHAR(0,0,"set_turn");
	LCD9648_Write16EnCHAR(0,2,"0-turn_off");
	LCD9648_Write16EnCHAR(0,4,"1-turn_en ");
	*/
	OLED_Clear();
	OLED_ShowString(20,0,"SET_TURN",16);
	OLED_ShowString(0,3,"0-turn_off",12);
	OLED_ShowString(0,5,"1-turn_en",12);
	if(_turn_en==0)
		OLED_ShowString(0,7,"now:off",12);
	else
		OLED_ShowString(0,7,"now:on",12);


	menu=2;
	while(1)
	{
		key_val=KEY_Scan(0);
		switch(key_val)
		{
			case '0': _turn_en=0;menu=1;break;
			case '1': _turn_en=1;menu=1;break;
			case '#': ;menu=1;break;
			default:;
		}
		if(menu==1)
			{//LCD9648_Clear();
			OLED_Clear();
			break;}
	}
}
//


//普通模式选项菜单（二级）
void normal_serve()
{
	/*
	LCD9648_Clear();
	LCD9648_Write16EnCHAR(0,0,"normal");
	LCD9648_Write16EnCHAR(0,2,"1-gear");
	LCD9648_Write16EnCHAR(0,4,"2-turn");
	*/

	OLED_Clear();
	OLED_ShowString(20,0,"NORMAL",16);
	OLED_ShowString(0,3,"1-gear",12);
	OLED_ShowString(0,5,"2-turn ",12);


	menu = 1;
	while(1)
	{
		key_val=KEY_Scan(0);
		switch(key_val)
		{
			case '1': gear_set();menu=0;break;
			case '2': turn_set();menu=0;break;
			case '#': menu=0;break;
			default:;
		}
		if(menu==0)
			//{LCD9648_Clear();break;}
		{OLED_Clear();break;}

	}
}

//智能模式

void auto_off()
{
	menu = 2;
	OLED_Clear();
	OLED_ShowString(20,0,"AUTO_OFF",16);
	OLED_ShowString(0,2,"based on human ",12);
	OLED_ShowString(0,3,"detection ",12);

	OLED_ShowString(0,5,"0-turn off ",12);
	OLED_ShowString(0,7,"1-turn on ",12);
	while(1)
	{
		key_val=KEY_Scan(0);

		switch(key_val)
		{
		case '1': _auto_off=1;menu=1;break;
		case '0': _auto_off=0;menu=1;;break;
		case '#': menu=1;break;
		default: break;
		}
		if(menu==1){
			//LCD9648_Clear();
			OLED_Clear();
			break;
		}

	}
}

void auto_gear()
{
	menu = 2;
	OLED_Clear();
	OLED_ShowString(20,0,"AUTO_GEAR",16);
	OLED_ShowString(0,2,"based on ",12);
	OLED_ShowString(0,3,"temperature ",12);

	OLED_ShowString(0,5,"0-turn off ",12);
	OLED_ShowString(0,7,"1-turn on ",12);
	while(1)
	{
		key_val=KEY_Scan(0);

		switch(key_val)
		{
		case '1': _auto_gear=1;menu=1;break;
		case '0': _auto_gear=0;menu=1;;break;
		case '#': menu=1;break;
		default: break;
		}
		if(menu==1){

			OLED_Clear();
			break;
		}

	}
}


void set_time_receive()
{
	char receive[3]={0};
	int i=0;
	OLED_Clear();
	OLED_ShowString(0,0,"SET TIME ",16);
	OLED_ShowString(0,2,"please input_min",12);

	for(i=0;i<3;i++)
	{
		while(1)
		{
			key_val=KEY_Scan(0);
			if(key_val!=' ')
			{
				receive[i]=key_val;
				OLED_ShowChar(i*6,4,key_val,12);
				break;
			}
		}
	}
	while(1)
	{
		key_val=KEY_Scan(0);

		if(key_val=='*')
		{
			_set_time=atoi(receive);pass_time=0;break;
		}

	}
}


void set_time()
{
	menu = 2;
	OLED_Clear();
	OLED_ShowString(20,0,"SET TIME OFF",16);
	OLED_ShowString(0,2,"now:set time:",12);
	if(_set_time_en==1)
		OLED_ShowNumber(0,3,_set_time,3,12);
	else
		OLED_ShowString(0,3,"disable",12);

	OLED_ShowString(0,5,"0-turn off ",12);
	OLED_ShowString(0,7,"1-turn on ",12);
	while(1)
	{



		key_val=KEY_Scan(0);

		switch(key_val)
		{
		case '1': set_time_receive();_set_time_en=1;menu=1;
		break;
		case '0': _set_time=0;_set_time_en=0;menu=1;break;
		case '#': menu=1;break;
		default: break;
		}
		if(menu==1){
			OLED_Clear();
			break;
		}

	}
}

void smart_serve()
{
	char page=0;
	//LCD9648_Clear();
	//OLED_Clear();
	menu = 1;
	OLED_Clear();
	OLED_ShowString(20,0,"SMART",16);
	OLED_ShowString(0,3,"1-auto off",12);
	OLED_ShowString(0,5,"2-auto gear",12);
	OLED_ShowString(0,7,"3-set time off",12);


	while(1)
	{
		/*
		switch(page)
	{
	case 0:LCD9648_Write16EnCHAR(0,0,"smart");
			LCD9648_Write16EnCHAR(0,2,"1-auto_off");
			LCD9648_Write16EnCHAR(0,4,"2-autogear");
	case 1:LCD9648_Write16EnCHAR(0,0,"3-time_off");
	}
	*/


		key_val=KEY_Scan(0);

		switch(key_val)
		{
		case '1': auto_off();menu=0;break;
		case '2': auto_gear();menu=0;;break;
		case '3': set_time();menu=0;;break;
		/*
		case 'A':  if(page<=0)
						page=0;
					else
						page--;
				    	LCD9648_Clear();break;
		case 'B':  if(page>=1)
				    	page=1;
				    else
				    	page++;
				    	LCD9648_Clear();break;
				    	*/
		case '#':	menu=0;break;
		default: break;
		}
		if(menu==0){
			//LCD9648_Clear();
			OLED_Clear();
			break;
		}

	}
}

void state()
{
	char page=0;
	menu = 1;

	while(1)
	{

	switch(page)
	{
	case 0:
			OLED_Clear();
			OLED_ShowString(20,0,"STATE",16);
			OLED_ShowString(0,3,"gear:",12);
			OLED_ShowString(0,4,"auto_off:",12);
			OLED_ShowString(0,5,"set_time:",12);
			OLED_ShowString(0,6,"remain_time:",12);



			if(_auto_off==0)
				OLED_ShowString(70,4,"disable",12);
			else
				OLED_ShowString(70,4,"enable",12);


			break;
	case 1:OLED_Clear();
			OLED_ShowString(20,0,"STATE",16);
			OLED_ShowString(0,3,"temperature:",12);

			break;
	}

	while(1)
		{
		switch(page)
			{
			case 0:	if(_auto_gear==0)
						OLED_ShowNumber(50,3,gear,1,12);
					else
						OLED_ShowString(50,3,"auto",12);
					if(_set_time_en==0)
					{
						OLED_ShowString(70,5,"disable",12);
						OLED_ShowString(70,7,"disable",12);
					}
					else
					{
						OLED_ShowNumber(70,5,_set_time,3,12);
						//OLED_ShowNumber(70,7,(_set_time-(int)(pass_time/30)),3,12);
						OLED_ShowNumber(70,7,(int)(_set_time-pass_time),3,12);

					}break;
			case 1:
					OLED_ShowNumber(0,4,temperature,3,12);
/*
					if(BODY_DATA)
						OLED_ShowString(0,5,"invade ",12);
					else
						OLED_ShowString(0,5,"else  ",12);
						break;
*/
			}


		key_val=KEY_Scan(0);

				switch(key_val)
				{
			    case 'A':  if(page<=0)
			    				page=0;
			    			else
			    				page--;
			    			break;
			    case 'B':  if(page>=1)
			    				page=1;
			    			else
			    				page++;
			    			break;
				case '#':	menu=0;break;
				default: break;
				}
				if(menu==0)
				{
					OLED_Clear();
					break;
				}
				if(key_val=='A'||key_val=='B')
				{
					break;
				}
		}
		if(menu==0){
			OLED_Clear();
			break;
	}
	}
}
