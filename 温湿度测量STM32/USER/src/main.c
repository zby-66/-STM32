#include "uart.h"
#include "gec_gpio.h"
#include "gec_time.h"
#include "gec_init.h"

int main()
{
	Gec_Lib_Init();
	Gec_GPIO_Init(GPIOF,GPIO_Pin_9,GPIO_MODE_OUT_PP );       //对LED1灯的初始化；
	Gec_GPIO_Init(GPIOF,GPIO_Pin_10,GPIO_MODE_OUT_PP );		 //对LED2灯的初始化；
	Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,1);					 //开始导入程序时将LED1灯设置为灭
	Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,1);				 //开始导入程序时将LED2灯设置为灭
	Gec_GPIO_Init(GPIOF, GPIO_Pin_8,GPIO_MODE_OUT_PP);		 //将蜂鸣器输出设置为推挽输出模式
	
	uart1_init();											 //将串口初始化

	while(1)
	{
		read_dht11();//通过串口读温湿度
		mdelay(1000);  //延时一秒；
	}
}


	
