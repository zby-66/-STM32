
#ifndef __GEC_GPIO_H__
#define __GEC_GPIO_H__

#include <stdint.h>

#include "gec_lib.h"



//GPIO的模式
enum GPIO_MODE
{
	GPIO_MODE_IN_FLOATING = (1 << 0), // 输入悬空
		/*
		输入悬空(Input Floating): 不接上拉电阻也不接下拉电阻
				上拉电阻：接一个正电源。
				下拉电阻：接一个地。
				每个GPIO口内部都有一个上拉电阻和一个电阻。
				悬空(Floating)就是逻辑器件的输入引脚内部既不接上拉电阻，也不接下拉电阻。
				此时，IO引脚的状态完全则外部输入所决定的，这时CPU能够通过读数据的操作
				知道IO口的电平状态。
		*/
	

	
	GPIO_MODE_IN_PULLUP = (1 << 1), //带上拉的输入
		/*
		带上拉输入(input pull-up): 接上拉电阻
				pull-up逻辑器件的输入引脚内部接一个上拉电阻。此时，IO引脚
				在外部没有输入信号时，CPU读到的就是一个高电平。唯有当被外部
				信号下拉时，CPU读取的电平为低电平。
		*/

	
	GPIO_MODE_IN_PULLDOWN = (1 << 2), //带下拉的输入
		/*
		带下拉输入(input pull-down): 接下拉电阻
				pull-down逻辑器的输入引脚内部接一个下拉电阻。此时，IO引脚
				在外部没有输入信号时，CPU读到的就是一个低电平。唯有当被外部
				信号上拉时，CPU读取的电平为高电平。

		*/

	
	GPIO_MODE_IN_ANALOG = (1 << 3), //模拟输入
		/*
		模拟输入(Input analog)
				该引脚被设置为模块输入时，能够获取的模拟信号，通过ADC部分转换
				为数字量，如变化的电压值，才能被CPU识别。

		*/


	GPIO_MODE_OUT_PP = (1 << 4), //输出推挽
		/*
		输出推挽(Push-Pull): 可以输出高电压、低电压。
				
				1  -> 高电平(电压)
				0  -> 低电平(电压)
				其原理请参考图示 《输出推挽》
		*/
		
	GPIO_MODE_OUT_OD = (1 << 5), //输出开漏
		/*
		输出开漏(Open-Drain):
				不输出电压。CPU写0时引脚接地，CPU写1时不接地(悬空状态)
				如果(用户)外接上拉电阻，则在CPU写1时，IO口会接到用户的上拉电阻。
				其原理请参考图示《输出开漏》

		*/
	
	

	GPIO_MODE_EXTI_Rising = (1 << 6),  //外部中断　“上升沿中断”
	GPIO_MODE_EXTI_Falling = (1 << 7), //外部中断	    "下降沿中断"
	GPIO_MODE_EXTI_Both = (1 << 8),		//外部中断，“双边沿中断 上升沿 || 下降沿”


	GPIO_MODE_AF_OD = (1 << 9), //复用输出功能(开漏) 针对10x
	GPIO_MODE_AF_PP = (1 << 10), //复用输出功能(推挽) 针对10x
	

	GPIO_MODE_UNKOWN = 0x00, //未知
};



//#define IS_GEC_EXTI_MODE(mode) (((mode) == GPIO_MODE_EXTI_Rising) || \
//                                  ((mode) == GPIO_MODE_EXTI_Falling) || \
//                                  ((mode) == GPIO_MODE_EXTI_Both))

#define IS_GEC_EXTI_MODE(mode)  (( mode & GPIO_MODE_EXTI_Both) || \
								 ( mode & GPIO_MODE_EXTI_Rising) || \
								 ( mode & GPIO_MODE_EXTI_Falling))


/*
	Gec_GPIO_Init: 初始化一个GPIO引脚
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	@gpio_mode: 指定GPIO的模式

	返回值:
		成功返回0
		失败返回-1

*/
void Gec_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, enum GPIO_MODE gpio_mode);


/*
	Gec_GPIO_Get_Value: 获取某个GPIO的值
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	返回值:
		0  -> 低电平
		1  -> 高电平
*/
int Gec_GPIO_Get_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/*
	Gec_GPIO_Set_Value: 设置某个GPIO的值
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	@value:要设置的值
		0  -> 低电平
		1  -> 高电平
	返回值：
		无。
*/

void Gec_GPIO_Set_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t value);



/*
	Gec_GPIO_Toggle_Value:翻转某个GPIO引脚的值
					“翻转”： 0 -> 1
							1 -> 0
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	返回值:
		无。

*/
void Gec_GPIO_Toggle_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);



/*
	Gec_GPIO_Get_ITFlag:获取GPIO外部中断的标记
	@EXTI_Line:
		相应GPIO外部中断的中断编号，如:
			EXTI_Line0, EXTI_Line1, ...
	返回值:
		1 -> 相应的GPIO引脚产生了中断
		0 -> 相应的GPIO引脚没产生中断
*/
int Gec_GPIO_Get_ITFlag(uint32_t EXTI_Line);



/*
	Gec_GPIO_Clear_ITFlag: 清掉GPIO外部中断的标记
	@EXTI_Line:
		相应GPIO外部中断的中断编号，如:
			EXTI_Line0, EXTI_Line1, ...
	返回值:
		无
*/
void Gec_GPIO_Clear_ITFlag(uint32_t EXTI_Line);
#endif
