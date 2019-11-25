

#include "gec_lib.h"

#include "gec_gpio.h"


static uint32_t get_gpio_exti_pin(uint32_t GPIO_Pin)
{
	uint32_t exti_pin = 111;

	switch(GPIO_Pin)
	{
		case GPIO_Pin_0:
			exti_pin = EXTI_PIN0;
			break;
		case GPIO_Pin_1:
			exti_pin = EXTI_PIN1;
			break;
		case GPIO_Pin_2:
			exti_pin = EXTI_PIN2;
			break;
		case GPIO_Pin_3:
			exti_pin = EXTI_PIN3;
			break;
		case GPIO_Pin_4:
			exti_pin = EXTI_PIN4;
			break;
		case GPIO_Pin_5:
			exti_pin = EXTI_PIN5;
			break;
		case GPIO_Pin_6:
			exti_pin = EXTI_PIN6;
			break;
		case GPIO_Pin_7:
			exti_pin = EXTI_PIN7;
			break;
		case GPIO_Pin_8:
			exti_pin = EXTI_PIN8;
			break;
		case GPIO_Pin_9:
			exti_pin = EXTI_PIN9;
			break;
		case GPIO_Pin_10:
			exti_pin = EXTI_PIN10;
			break;
		case GPIO_Pin_11:
			exti_pin = EXTI_PIN11;
			break;
		case GPIO_Pin_12:
			exti_pin = EXTI_PIN12;
			break;
		case GPIO_Pin_13:
			exti_pin = EXTI_PIN13;
			break;
		case GPIO_Pin_14:
			exti_pin = EXTI_PIN14;
			break;
		case GPIO_Pin_15:
			exti_pin = EXTI_PIN15;
			break;
		default:
			break;
	}

	
	return exti_pin;
}

static uint32_t get_gpio_exti_port(GPIO_TypeDef* GPIOx)
{
	uint32_t exti_port = 0;
	if (GPIOx == GPIOA)
	{
		exti_port =  EXTI_PORTA;
		
	}
	else if (GPIOx == GPIOB)
	{
		exti_port =  EXTI_PORTB;
	}
	else if (GPIOx == GPIOC)
	{
		exti_port =  EXTI_PORTC;
	}
	else if (GPIOx == GPIOD)
	{
		exti_port =  EXTI_PORTD;
	}
	else if (GPIOx == GPIOE)
	{
		exti_port =  EXTI_PORTE;
	}
	else if (GPIOx == GPIOF)
	{
		exti_port =  EXTI_PORTF;
	}
	else if (GPIOx == GPIOG)
	{
		exti_port =  EXTI_PORTG;
	}

	#if STM32F4
	else if (GPIOx == GPIOH)
	{
		exti_port =  EXTI_PORTH;
	}
	else if (GPIOx == GPIOI)
	{
		exti_port =  EXTI_PORTI;
	}
	else if (GPIOx == GPIOJ)
	{
		exti_port =  EXTI_PORTJ;
	}
	else if (GPIOx == GPIOK)
	{
		exti_port =  EXTI_PORTK;
	}
	#endif

	return exti_port;
}



static uint32_t get_gpio_clock_group(GPIO_TypeDef* GPIOx)
{
	uint32_t clock_group = 0;
	if (GPIOx == GPIOA)
	{
		clock_group =  RCC_GROUP_GPIOA;
	}
	else if (GPIOx == GPIOB)
	{
		clock_group =  RCC_GROUP_GPIOB;
	}
	else if (GPIOx == GPIOC)
	{
		clock_group =  RCC_GROUP_GPIOC;
	}
	else if (GPIOx == GPIOD)
	{
		clock_group =  RCC_GROUP_GPIOD;
	}
	else if (GPIOx == GPIOE)
	{
		clock_group =  RCC_GROUP_GPIOE;
	}
	else if (GPIOx == GPIOF)
	{
		clock_group =  RCC_GROUP_GPIOF;
	}
	else if (GPIOx == GPIOG)
	{
		clock_group =  RCC_GROUP_GPIOG;
	}

	#if STM32F4
	
	else if (GPIOx == GPIOH)
	{
		clock_group =  RCC_GROUP_GPIOH;
	}
	else if (GPIOx == GPIOI)
	{
		clock_group =  RCC_GROUP_GPIOI;
	}
	else if (GPIOx == GPIOJ)
	{
		clock_group =  RCC_GROUP_GPIOJ;
	}
	else if (GPIOx == GPIOK)
	{
		clock_group =  RCC_GROUP_GPIOK;
	}
	#endif
	
	return clock_group;
}


static EXTITrigger_TypeDef get_exti_trigger(enum GPIO_MODE gpio_mode)
{
	EXTITrigger_TypeDef trigger_mode =(EXTITrigger_TypeDef) 0; 

	/*
	switch(gpio_mode)
	{
		case GPIO_MODE_EXTI_Both:
			trigger_mode = EXTI_Trigger_Rising_Falling;
			break;
		case GPIO_MODE_EXTI_Falling:
			trigger_mode = EXTI_Trigger_Falling;
			break;
		case GPIO_MODE_EXTI_Rising:
			trigger_mode = EXTI_Trigger_Rising;
		default:
			break;
	}*/

	if(gpio_mode & GPIO_MODE_EXTI_Both)
	{
		trigger_mode = EXTI_Trigger_Rising_Falling;
	}
	else if (gpio_mode & GPIO_MODE_EXTI_Rising)
	{
		trigger_mode = EXTI_Trigger_Rising;
	}
	else if (gpio_mode & GPIO_MODE_EXTI_Falling)
	{
		trigger_mode = EXTI_Trigger_Falling;
	}

	return trigger_mode;
}

static GPIOMode_TypeDef get_stm32gpiomode(enum GPIO_MODE gpio_mode)
{
	#if STM32F4
	GPIOMode_TypeDef  mode = GPIO_Mode_AF ;

	if ( (gpio_mode & GPIO_MODE_IN_FLOATING) || 
		 (gpio_mode & GPIO_MODE_IN_PULLUP) || 
		 (gpio_mode & GPIO_MODE_IN_PULLDOWN) || 
		 (gpio_mode & GPIO_MODE_EXTI_Both) ||
		 (gpio_mode & GPIO_MODE_EXTI_Rising) ||
		 (gpio_mode & GPIO_MODE_EXTI_Falling))
	{
		mode = GPIO_Mode_IN;
	}
	else if (gpio_mode & GPIO_MODE_IN_ANALOG)
	{
		mode = GPIO_Mode_AN;
	}
	else if ((gpio_mode & GPIO_MODE_OUT_OD) || 
			 (gpio_mode & GPIO_MODE_OUT_PP))
	{
		mode = GPIO_Mode_OUT;
	}


	/*
	switch (gpio_mode)
	{
		case GPIO_MODE_IN_FLOATING:   // 输入悬空
		case GPIO_MODE_IN_PULLUP:  //带上拉的输入
		case GPIO_MODE_IN_PULLDOWN: //带下拉的输入

		case GPIO_MODE_EXTI_Both:
		case GPIO_MODE_EXTI_Rising:
		case GPIO_MODE_EXTI_Falling:
			mode = GPIO_Mode_IN;
			break;
		case GPIO_MODE_IN_ANALOG:  //模拟输入
			mode = GPIO_Mode_AN;
			break;
		case GPIO_MODE_OUT_OD:
		case GPIO_MODE_OUT_PP:
			mode = GPIO_Mode_OUT;
			break;
		default:
			break;
	}
	*/

	#elif STM32F1

	GPIOMode_TypeDef  mode ;

	if ( (gpio_mode & GPIO_MODE_IN_FLOATING))
	{
		mode = GPIO_Mode_IN_FLOATING;
	}
	else if (gpio_mode & GPIO_MODE_IN_ANALOG)
	{
		mode = GPIO_Mode_AN;
	}
	else if ((gpio_mode & GPIO_MODE_IN_PULLUP) )
	{
		mode = GPIO_Mode_IPU;
	}
	else if ((gpio_mode & GPIO_MODE_IN_PULLDOWN) )
	{
		mode = GPIO_Mode_IPD;
	}
	else if ((gpio_mode & GPIO_MODE_OUT_PP) )
	{
		mode = GPIO_Mode_Out_PP;
	}
	else if ((gpio_mode & GPIO_MODE_OUT_OD) )
	{
		mode = GPIO_Mode_Out_OD;
	}


	/*
	switch (gpio_mode)
	{
		case GPIO_MODE_IN_FLOATING:
			mode = GPIO_Mode_IN_FLOATING;
			break;
		case GPIO_MODE_IN_PULLUP:
			mode = GPIO_Mode_IPU;
			break;
		case GPIO_MODE_IN_PULLDOWN:
			mode = GPIO_Mode_IPD;
			break;

								 //GPIO口配置成EXTI时，in floating
		case GPIO_MODE_EXTI_Both:
		case GPIO_MODE_EXTI_Rising:
		case GPIO_MODE_EXTI_Falling:
			//mode = GPIO_Mode_IN_FLOATING;
			mode = GPIO_Mode_IPU;
			break;

		case GPIO_MODE_IN_ANALOG:  //模拟输入
			mode = GPIO_Mode_AIN;
			break;
		case GPIO_MODE_OUT_OD:
			mode = GPIO_Mode_Out_OD;
			break;
		case GPIO_MODE_OUT_PP:
			mode = GPIO_Mode_Out_PP;
			break;	

		default:
			break;
	}
	*/

	#endif


	return mode;

}

#if STM32F4
static GPIOPuPd_TypeDef get_stm32gpioPuPd(enum GPIO_MODE gpio_mode)
{
	GPIOPuPd_TypeDef  mode = GPIO_PuPd_NOPULL ;

	if (gpio_mode & GPIO_MODE_IN_PULLUP)
	{
		//带上拉的输入
		mode = GPIO_PuPd_UP;
	}
	else if (gpio_mode & GPIO_MODE_IN_PULLDOWN)
	{
		//带下拉的输入
		mode = GPIO_PuPd_DOWN;
	}


	/*
	switch (gpio_mode)
	{
		//case GPIO_MODE_IN_FLOATING:   // 输入悬空
		case GPIO_MODE_IN_PULLUP:  //带上拉的输入
			mode = GPIO_PuPd_UP;
			break;
		case GPIO_MODE_IN_PULLDOWN: //带下拉的输入
			mode = GPIO_PuPd_DOWN;
			break;
	
		default:
			break;
	}
	*/
	return mode;

}
#endif

/*
	Gec_GPIO_Init: 初始化一个GPIO引脚
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	@gpio_mode: 指定GPIO的模式

	返回值:
		成功返回0
		失败返回-1

*/
void Gec_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, enum GPIO_MODE gpio_mode)
{
//	int ret = 0;
	uint32_t clock_group = get_gpio_clock_group(GPIOx);
	//GPIO_Init
	GPIO_InitTypeDef p;



	//使能GPIO分组时钟
	#if STM32F4	
	RCC_AHB1PeriphClockCmd(clock_group, ENABLE);
	#elif STM32F1
	RCC_APB2PeriphClockCmd(clock_group, ENABLE);
	#endif


	#if STM32F4	
	p.GPIO_Mode = get_stm32gpiomode(gpio_mode) ;
	p.GPIO_OType = (gpio_mode & GPIO_MODE_OUT_OD) ? GPIO_OType_OD : GPIO_OType_PP;
	p.GPIO_Pin = GPIO_Pin;
	p.GPIO_PuPd = get_stm32gpioPuPd(gpio_mode);
	p.GPIO_Speed = GPIO_Speed_50MHz;

	#elif STM32F1

	p.GPIO_Mode = get_stm32gpiomode(gpio_mode);
	p.GPIO_Pin = GPIO_Pin;
	p.GPIO_Speed = GPIO_Speed_50MHz;	

	#endif


	GPIO_Init(GPIOx, &p);


	#if STM32F1
	//
	if ( (GPIOx == GPIOA  && (GPIO_Pin == GPIO_Pin_13 ||
							  GPIO_Pin == GPIO_Pin_14 ||
							  GPIO_Pin == GPIO_Pin_15) ) ||
		((GPIOx == GPIOB  && (GPIO_Pin == GPIO_Pin_3 ||
							  GPIO_Pin == GPIO_Pin_4) ))
		)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	}

	#endif




	//配置外部中断，if EXTI
	if (IS_GEC_EXTI_MODE(gpio_mode))
	{
		EXTI_InitTypeDef e;
		// 1.2 SYSCFG控制器
		#if STM32F4
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
		SYSCFG_EXTILineConfig( get_gpio_exti_port(GPIOx), get_gpio_exti_pin(GPIO_Pin)); // PA0 -> EXTI0
		#elif STM32F1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_EXTILineConfig(get_gpio_exti_port(GPIOx), get_gpio_exti_pin(GPIO_Pin));
		
		#endif
		
		// 1.3 EXTI外部中断控制器的配置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 0 ~3

		
		//EXTI_Line0
		e.EXTI_Line = GPIO_Pin; //GPIO_Pin_0 <->  EXTI_Line0
		e.EXTI_LineCmd = ENABLE;
		e.EXTI_Mode = EXTI_Mode_Interrupt;
		e.EXTI_Trigger = get_exti_trigger(gpio_mode);
		EXTI_Init(&e);
	}

}


/*
	Gec_GPIO_Get_Value: 获取某个GPIO的值
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	返回值:
		0  -> 低电平
		1  -> 高电平
*/
int Gec_GPIO_Get_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

	return GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin);

}


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

void Gec_GPIO_Set_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t value)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (value == 0)? Bit_RESET : Bit_SET);

}


/*
	Gec_GPIO_Toggle_Value:翻转某个GPIO引脚的值
					“翻转”： 0 -> 1
							1 -> 0
	@GPIOx: 指定GPIO分组，如：GPIOA, GPIOB,...
	@GPIO_Pin: 指定GPIO引脚编号，如：GPIO_Pin_0, GPIO_Pin_1,...
	返回值:
		无。

*/
void Gec_GPIO_Toggle_Value(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	#if STM32F4
	GPIO_ToggleBits(GPIOx, GPIO_Pin);

	#elif STM32F1

	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

	GPIOx->ODR ^= GPIO_Pin;

	#endif
}

/*
	Gec_GPIO_Get_ITFlag:获取GPIO外部中断的标记
	@EXTI_Line:
		相应GPIO外部中断的中断编号，如:
			EXTI_Line0, EXTI_Line1, ...
	返回值:
		1 -> 相应的GPIO引脚产生了中断
		0 -> 相应的GPIO引脚没产生中断
*/
int Gec_GPIO_Get_ITFlag(uint32_t EXTI_Line)
{
	return EXTI_GetITStatus(EXTI_Line);
}

/*
	Gec_GPIO_Clear_ITFlag: 清掉GPIO外部中断的标记
	@EXTI_Line:
		相应GPIO外部中断的中断编号，如:
			EXTI_Line0, EXTI_Line1, ...
	返回值:
		无
*/
void Gec_GPIO_Clear_ITFlag(uint32_t EXTI_Line)
{
	EXTI_ClearITPendingBit(EXTI_Line);
}

