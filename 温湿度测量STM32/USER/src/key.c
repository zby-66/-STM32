#include "gec_gpio.h"
#include "key.h"


void key_init(void)
{
	Gec_GPIO_Init(GPIOA, GPIO_Pin_0,GPIO_MODE_IN_PULLUP);//KEY 1
	Gec_GPIO_Init(GPIOE, GPIO_Pin_2,GPIO_MODE_IN_PULLUP);//KEY 2
	Gec_GPIO_Init(GPIOE, GPIO_Pin_3,GPIO_MODE_IN_PULLUP);//KEY 3
	Gec_GPIO_Init(GPIOE, GPIO_Pin_4,GPIO_MODE_IN_PULLUP);//KEY 4
	
	Gec_GPIO_Init(GPIOF, GPIO_Pin_8,GPIO_MODE_OUT_PP);//BEEP
	
	Gec_GPIO_Init(GPIOF, GPIO_Pin_9,GPIO_MODE_OUT_PP); //D1
	Gec_GPIO_Init(GPIOF, GPIO_Pin_10,GPIO_MODE_OUT_PP);//D2
	Gec_GPIO_Init(GPIOE, GPIO_Pin_13,GPIO_MODE_OUT_PP);//D3
	Gec_GPIO_Init(GPIOE, GPIO_Pin_14,GPIO_MODE_OUT_PP);//D4

	Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,1);
	Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,1);
	Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_13,1);
	Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_14,1);
	
	int i=0;
	while(1)
	{	
		
		if( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0 ) == KEY_ON )
		{
				Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,0);	
			i++;
		}
				else
		{
							Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,1);	
		}
		
		if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_2 ) == KEY_ON )
		{

				Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,0);

			i++;
		}
		
				else 
		{
							Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,1);	
		}
		
		if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_3 ) == KEY_ON )
		{

				Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_13,0);

			i++;
		}
		
				else 
		{
							Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_13,1);	
		}
		
		if( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_4 ) == KEY_ON )
		{

				Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_14,0);

			i++;
		}
		
				else 
		{
							Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_14,1);	
		}

		if(i >= 2)
		{
			Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_8,1);
		}
	}
}



