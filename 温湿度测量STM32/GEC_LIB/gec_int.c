
//#include "stm32f4xx.h"
//#include "core_cm4.h"
#include "gec_int.h"

//用户级中断向量表
isr_t Gec_IntVectTbl[BSP_INT_ID_NUM];


static  void  Gec_IntHandler_Dummy (void)
{

}

//总中断函数

void Gec_IntHandler(unsigned int int_id)
{
	isr_t isr;
	if (int_id < BSP_INT_ID_NUM) 
	{
		isr = Gec_IntVectTbl[int_id];
		if (isr != (isr_t)Gec_IntHandler_Dummy) 
		{
		    isr();
		}
	}
}





void Gec_Int_Register(unsigned int int_id, isr_t isr)
{
	if (int_id < BSP_INT_ID_NUM)
	{
		Gec_IntVectTbl[int_id] = isr;
	}
}

void Gec_Int_Unregister(unsigned int int_id)
{
	if (int_id < BSP_INT_ID_NUM)
	{
		Gec_IntVectTbl[int_id] = Gec_IntHandler_Dummy;
	}
}


void Gec_Int_Enable(unsigned int int_id)
{
	//NVIC_SetPriority((IRQn_Type)int_id, 15); //中断优先级最低

	NVIC_EnableIRQ((IRQn_Type)int_id);
}

void Gec_Int_Disable(unsigned int int_id)
{
	NVIC_DisableIRQ((IRQn_Type)int_id);
}


//该函数，应该在启动代码中或C_main中调用。
void Gec_Int_Init(void)
{
	unsigned int  int_id;
	unsigned int  r;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	for (int_id = 0; int_id < BSP_INT_ID_NUM; int_id++) 
	{
		Gec_Int_Register(int_id, Gec_IntHandler_Dummy);
		Gec_Int_Disable(int_id);
		NVIC_SetPriority((IRQn_Type)int_id, 15); //中断优先级最低
	}

	// 设置SysTick中断的优先级为 0(最高)
	// SysTick中断优先级在　寄存器　0xE000ED23 设置　　8bits
	// 4字节对齐，小端模式，故从地址0xE000ED20处开始读4字节
	// 0xE000ED23在最高字节
	r =  *((volatile unsigned int*)0xE000ED20);
	r &=  (1UL << 24) - 1; //把r的最高8bits清零，其他位不变
	*((volatile unsigned int*)0xE000ED20) = r;


	
}



void	   WWDG_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_WWDG); } // Window WatchDog										  
void	   PVD_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_PVD); } // PVD through EXTI Line detection						  
void	   TAMP_STAMP_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_TAMP_STAMP); } // Tamper and TimeStamps through the EXTI line			  
void	   RTC_WKUP_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_RTC_WKUP); } // RTC Wakeup through the EXTI line 					  
void	   FLASH_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_FLASH); } // FLASH										   
void	   RCC_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_RCC); } // RCC											   
void	   EXTI0_IRQHandler() 				 
{

	Gec_IntHandler(BSP_INT_ID_EXTI0);
} // EXTI Line0	�жϴ������������֣����Ը�											 
void	   EXTI1_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_EXTI1); } // EXTI Line1											  
void	   EXTI2_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_EXTI2); } // EXTI Line2											  
void	   EXTI3_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_EXTI3); } // EXTI Line3											  
void	   EXTI4_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_EXTI4); } // EXTI Line4											  
void	   DMA1_Stream0_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH0); } // DMA1 Stream 0								   
void	   DMA1_Stream1_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH1); } // DMA1 Stream 1								   
void	   DMA1_Stream2_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH2); } // DMA1 Stream 2								   
void	   DMA1_Stream3_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH3); } // DMA1 Stream 3								   
void	   DMA1_Stream4_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH4); } // DMA1 Stream 4								   
void	   DMA1_Stream5_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH5); } // DMA1 Stream 5								   
void	   DMA1_Stream6_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_CH6); } // DMA1 Stream 6								   
void	   ADC_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_ADC); } // ADC1, ADC2 and ADC3s 						   
void	   CAN1_TX_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN1_TX); } // CAN1 TX												  
void	   CAN1_RX0_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN1_RX0); } // CAN1 RX0 											  
void	   CAN1_RX1_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN1_RX1); } // CAN1 RX1 											  
void	   CAN1_SCE_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN1_SCE); } // CAN1 SCE 											  
void	   EXTI9_5_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_EXTI9_5); } // External Line[9:5]s									  
void	   TIM1_BRK_TIM9_IRQHandler() 		 {  Gec_IntHandler(BSP_INT_ID_TIM1_BRK_TIM9); } // TIM1 Break and TIM9					 
void	   TIM1_UP_TIM10_IRQHandler() 		 {  Gec_IntHandler(BSP_INT_ID_TIM1_UP_TIM10); } // TIM1 Update and TIM10				 
void	   TIM1_TRG_COM_TIM11_IRQHandler()	 {  Gec_IntHandler(BSP_INT_ID_TIM1_TRG_COM_TIM11); } // TIM1 Trigger and Commutation and TIM11
void	   TIM1_CC_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_TIM1_CC); } // TIM1 Capture Compare 								  
void	   TIM2_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_TIM2); } // TIM2 										   
void	   TIM3_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_TIM3); } // TIM3 										   
void	   TIM4_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_TIM4); } // TIM4 										   
void	   I2C1_EV_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C1_EV); } // I2C1 Event											  
void	   I2C1_ER_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C1_ER); } // I2C1 Error											  
void	   I2C2_EV_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C2_EV); } // I2C2 Event											  
void	   I2C2_ER_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C2_ER); } // I2C2 Error												
void	   SPI1_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_SPI1); } // SPI1 										   
void	   SPI2_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_SPI2); } // SPI2 										   
void	   USART1_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_USART1); } // USART1										   
void	   USART2_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_USART2); } // USART2										   
void	   USART3_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_USART3); } // USART3										   
void	   EXTI15_10_IRQHandler() 			 {  Gec_IntHandler(BSP_INT_ID_EXTI15_10); } // External Line[15:10]s								  
void	   RTC_Alarm_IRQHandler() 			 {  Gec_IntHandler(BSP_INT_ID_RTC_ALARM); } // RTC Alarm (A and B) through EXTI Line				  
void	   OTG_FS_WKUP_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_OTG_FS_WKUP); } // USB OTG FS Wakeup through EXTI line						  
void	   TIM8_BRK_TIM12_IRQHandler()		 {  Gec_IntHandler(BSP_INT_ID_TIM8_BRK_TIM12); } // TIM8 Break and TIM12 				 
void	   TIM8_UP_TIM13_IRQHandler() 		 {  Gec_IntHandler(BSP_INT_ID_TIM8_UP_TIM13); } // TIM8 Update and TIM13				 
void	   TIM8_TRG_COM_TIM14_IRQHandler()	 {  Gec_IntHandler(BSP_INT_ID_TIM8_TRG_COM_TIM14); } // TIM8 Trigger and Commutation and TIM14
void	   TIM8_CC_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_TIM8_CC); } // TIM8 Capture Compare 								  
void	   DMA1_Stream7_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA1_STREAM7); } // DMA1 Stream7 										  
void	   FSMC_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_FSMC); } // FSMC 										   
void	   SDIO_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_SDIO); } // SDIO 										   
void	   TIM5_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_TIM5); } // TIM5 										   
void	   SPI3_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_SPI3); } // SPI3 										   
void	   UART4_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_USART4); } // UART4										   
void	   UART5_IRQHandler() 				 {  Gec_IntHandler(BSP_INT_ID_USART5); } // UART5										   
void	   TIM6_DAC_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_TIM6_DAC); } // TIM6 and DAC1&2 underrun errors					 

void	   TIM7_IRQHandler(void)					 
{  


	Gec_IntHandler(BSP_INT_ID_TIM7); 

} // TIM7 				  
void	   DMA2_Stream0_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH0); } // DMA2 Stream 0								   
void	   DMA2_Stream1_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH1); } // DMA2 Stream 1								   
void	   DMA2_Stream2_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH2); } // DMA2 Stream 2								   
void	   DMA2_Stream3_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH3); } // DMA2 Stream 3								   
void	   DMA2_Stream4_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH4); } // DMA2 Stream 4								   
void	   ETH_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_ETH); } // Ethernet 									   
void	   ETH_WKUP_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_ETH_WKUP); } // Ethernet Wakeup through EXTI line					  
void	   CAN2_TX_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN2_TX); } // CAN2 TX												  
void	   CAN2_RX0_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN2_RX0); } // CAN2 RX0 											  
void	   CAN2_RX1_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN2_RX1); } // CAN2 RX1 											  
void	   CAN2_SCE_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_CAN2_SCE); } // CAN2 SCE 											  
void	   OTG_FS_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_OTG_FS); } // USB OTG FS									   
void	   DMA2_Stream5_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH5); } // DMA2 Stream 5								   
void	   DMA2_Stream6_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH6); } // DMA2 Stream 6								   
void	   DMA2_Stream7_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_DMA2_CH7); } // DMA2 Stream 7								   
void	   USART6_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_USART6); } // USART6											
void	   I2C3_EV_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C3_EV); } // I2C3 event											  
void	   I2C3_ER_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_I2C3_ER); } // I2C3 error											  
void	   OTG_HS_EP1_OUT_IRQHandler()		 {  Gec_IntHandler(BSP_INT_ID_OTG_HS_EP1_OUT); } // USB OTG HS End Point 1 Out					   
void	   OTG_HS_EP1_IN_IRQHandler() 		 {  Gec_IntHandler(BSP_INT_ID_OTG_HS_EP1_IN); } // USB OTG HS End Point 1 In					   
void	   OTG_HS_WKUP_IRQHandler()			 {  Gec_IntHandler(BSP_INT_ID_OTG_HS_WKUP); } // USB OTG HS Wakeup through EXTI						  
void	   OTG_HS_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_OTG_HS); } // USB OTG HS									   
void	   DCMI_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_DCMI); } // DCMI 										   
void	   CRYP_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_CRYP); } // CRYP crypto									   
void	   HASH_RNG_IRQHandler()				 {  Gec_IntHandler(BSP_INT_ID_HASH_RNG); } // Hash and Rng
void	   FPU_IRQHandler()					 {  Gec_IntHandler(BSP_INT_ID_FPU); } // FPU

