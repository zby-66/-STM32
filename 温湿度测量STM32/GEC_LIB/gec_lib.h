
#ifndef __GEC_LIB_H__
#define __GEC_LIB_H__


 


#if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_XL) || defined (STM32F10X_CL) 

#define STM32F1  1

#include "stm32f10x.h"
#include "core_cm3.h"

//时钟分组 stm32f10x

#define RCC_GROUP_GPIOA   RCC_APB2Periph_GPIOA
#define RCC_GROUP_GPIOB   RCC_APB2Periph_GPIOB
#define RCC_GROUP_GPIOC   RCC_APB2Periph_GPIOC
#define RCC_GROUP_GPIOD   RCC_APB2Periph_GPIOD
#define RCC_GROUP_GPIOE   RCC_APB2Periph_GPIOE
#define RCC_GROUP_GPIOF   RCC_APB2Periph_GPIOF
#define RCC_GROUP_GPIOG   RCC_APB2Periph_GPIOG

#define RCC_GROUP_AFIO	  RCC_APB2Periph_AFIO



// EXTI  PORTx
#define EXTI_PORTA	GPIO_PortSourceGPIOA
#define EXTI_PORTB	GPIO_PortSourceGPIOB
#define EXTI_PORTC	GPIO_PortSourceGPIOC
#define EXTI_PORTD	GPIO_PortSourceGPIOD
#define EXTI_PORTE	GPIO_PortSourceGPIOE
#define EXTI_PORTF	GPIO_PortSourceGPIOF
#define EXTI_PORTG	GPIO_PortSourceGPIOG


//EXTI  PINx
#define EXTI_PIN0	GPIO_PinSource0
#define EXTI_PIN1	GPIO_PinSource1
#define EXTI_PIN2	GPIO_PinSource2
#define EXTI_PIN3	GPIO_PinSource3
#define EXTI_PIN4	GPIO_PinSource4
#define EXTI_PIN5	GPIO_PinSource5
#define EXTI_PIN6	GPIO_PinSource6
#define EXTI_PIN7	GPIO_PinSource7
#define EXTI_PIN8	GPIO_PinSource8
#define EXTI_PIN9	GPIO_PinSource9
#define EXTI_PIN10	GPIO_PinSource10
#define EXTI_PIN11	GPIO_PinSource11
#define EXTI_PIN12	GPIO_PinSource12
#define EXTI_PIN13	GPIO_PinSource13
#define EXTI_PIN14	GPIO_PinSource14
#define EXTI_PIN15	GPIO_PinSource15


#elif defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F410xx) || defined(STM32F411xE) || defined(STM32F412xG) || defined(STM32F413_423xx) ||defined(STM32F446xx) || defined(STM32F469_479xx)


#define STM32F4 1
#include "stm32f4xx.h"
#include "core_cm4.h"


//时钟分组 stm32f40x
#define RCC_GROUP_GPIOA   RCC_AHB1Periph_GPIOA
#define RCC_GROUP_GPIOB   RCC_AHB1Periph_GPIOB
#define RCC_GROUP_GPIOC   RCC_AHB1Periph_GPIOC
#define RCC_GROUP_GPIOD   RCC_AHB1Periph_GPIOD
#define RCC_GROUP_GPIOE   RCC_AHB1Periph_GPIOE
#define RCC_GROUP_GPIOF   RCC_AHB1Periph_GPIOF
#define RCC_GROUP_GPIOG   RCC_AHB1Periph_GPIOG
#define RCC_GROUP_GPIOH   RCC_AHB1Periph_GPIOH
#define RCC_GROUP_GPIOI   RCC_AHB1Periph_GPIOI
#define RCC_GROUP_GPIOJ   RCC_AHB1Periph_GPIOJ
#define RCC_GROUP_GPIOK   RCC_AHB1Periph_GPIOK


// EXTI  PORTx
#define EXTI_PORTA	EXTI_PortSourceGPIOA
#define EXTI_PORTB	EXTI_PortSourceGPIOB
#define EXTI_PORTC	EXTI_PortSourceGPIOC
#define EXTI_PORTD	EXTI_PortSourceGPIOD
#define EXTI_PORTE	EXTI_PortSourceGPIOE
#define EXTI_PORTF	EXTI_PortSourceGPIOF
#define EXTI_PORTG	EXTI_PortSourceGPIOG
#define EXTI_PORTH	EXTI_PortSourceGPIOH
#define EXTI_PORTI	EXTI_PortSourceGPIOI
#define EXTI_PORTJ	EXTI_PortSourceGPIOJ
#define EXTI_PORTK	EXTI_PortSourceGPIOK


//EXTI  PINx
#define EXTI_PIN0	EXTI_PinSource0
#define EXTI_PIN1	EXTI_PinSource1
#define EXTI_PIN2	EXTI_PinSource2
#define EXTI_PIN3	EXTI_PinSource3
#define EXTI_PIN4	EXTI_PinSource4
#define EXTI_PIN5	EXTI_PinSource5
#define EXTI_PIN6	EXTI_PinSource6
#define EXTI_PIN7	EXTI_PinSource7
#define EXTI_PIN8	EXTI_PinSource8
#define EXTI_PIN9	EXTI_PinSource9
#define EXTI_PIN10	EXTI_PinSource10
#define EXTI_PIN11	EXTI_PinSource11
#define EXTI_PIN12	EXTI_PinSource12
#define EXTI_PIN13	EXTI_PinSource13
#define EXTI_PIN14	EXTI_PinSource14
#define EXTI_PIN15	EXTI_PinSource15



#endif







#endif

