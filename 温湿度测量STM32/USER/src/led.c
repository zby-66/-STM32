#include "led.h"
#include "gec_gpio.h"

void led_control(int led_num, int cmd)
{
	if(led_num==1)
	{
		if(cmd==1)
		{
			Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_9,cmd);	//led1亮(改了)
		}
		else if(cmd==0)
		{
			Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_9,cmd); //led1灭
		}
	}
	
	if(led_num==2)
	{
		if(cmd==1)
		{
			Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_10,cmd);//led2亮	
		}
		else if(cmd==0)
		{
			Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_10,cmd);//led2灭	
		}
	}

	if(led_num==3)
	{
		if(cmd==1)
		{
			Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_13,cmd);//led3亮	
		}
		else if(cmd==0)
		{
			Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_13,cmd);//led3灭	
		}
	}

	if(led_num==4)
	{
		if(cmd==1)
		{
			Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_14,cmd);//led4亮	
		}
		else if(cmd==0)
		{
			Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_14,cmd);//led4灭	
		}
	}

}
