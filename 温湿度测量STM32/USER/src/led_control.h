#ifndef __LED_CONTROL_H__//防止重复定义
#define __LED_CONTROL_H__

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4

#define LED_ON  0
#define LED_OFF 1

void led_init(void);//声明
void led_control(int led_num,int cmd);


#endif

