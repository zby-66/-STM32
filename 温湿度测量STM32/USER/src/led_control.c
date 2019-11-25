#include "led_control.h"
#include "gec_gpio.h"

//LED0 PF9
//LED1 PF10
//LED2 PE13
//LED3 PE14
//初始化LED灯对应的GPIO口
void led_init(void)
{
    //LED1对应的GPIO口的初始化
    Gec_GPIO_Init(GPIOF, GPIO_Pin_9, GPIO_MODE_OUT_PP);
    //LED2对应的GPIO口的初始化
    Gec_GPIO_Init(GPIOF, GPIO_Pin_10, GPIO_MODE_OUT_PP);
    //LED3对应的GPIO口的初始化
    Gec_GPIO_Init(GPIOE, GPIO_Pin_13, GPIO_MODE_OUT_PP);
    //LED4对应的GPIO口的初始化
    Gec_GPIO_Init(GPIOE, GPIO_Pin_14, GPIO_MODE_OUT_PP);  

    //控制LED灯默认为灭
    Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,1);
    Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,1);
    Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_13,1);
    Gec_GPIO_Set_Value(GPIOE,GPIO_Pin_14,1);
}

//实现控制某一盏LED灯亮或者灭
//led_no:某一盏灯
//cmd:命令,控制亮或者灭
void led_control(int led_num,int cmd)//定义
{
    if(led_num == LED1)
    {
        //用LED_ON(0)代表灯亮
        if(cmd == LED_ON)
        {
            Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_9, 0);
        }
        else //用LED_OFF(1)代表灯灭
        {
            Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_9, 1);
        }
    }
    else if(led_num == LED2)
    {
        //用LED_ON(0)代表灯亮
        if(cmd == LED_ON)
        {
            Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_10, 0);
        }
        else //用LED_OFF(1)代表灯灭
        {
            Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_10, 1);
        }

    }
    else if(led_num == LED3)
    {
        //用LED_ON(0)代表灯亮
        if(cmd == LED_ON)
        {
            Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_13, 0);
        }
        else //用LED_OFF(1)代表灯灭
        {
            Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_13, 1);
        }

    }
    else if(led_num == LED4)
    {
        //用LED_ON(0)代表灯亮
        if(cmd == LED_ON)
        {
            Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_14, 0);
        }
        else //用LED_OFF(1)代表灯灭
        {
            Gec_GPIO_Set_Value(GPIOE, GPIO_Pin_14, 1);
        }

    }
}


