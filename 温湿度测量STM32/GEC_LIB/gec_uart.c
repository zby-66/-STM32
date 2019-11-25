
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "gec_time.h"
#include "gec_gpio.h"

/*
	Gec_UART_Init:串口初始函数
	@USARTx: 指定要初始化的串口，如: USART1, USART2, USART3 ...
	@bard_rate: 指定串口波特率，如: 115200, 9600
	@parity: 指定串口的校验方式，有如下:
			USART_Parity_No 无校验
			USART_Parity_Odd  奇校验
			USART_Parity_Even　偶校验
	@data_bits: 指定串口一帧数据的bit位数,如:  7, 8, 9
	@stop_bits: 指定串口传输的停止位数，有如下:
			USART_StopBits_0_5    0.5个停止位
			USART_StopBits_1	  1个停止位
			USART_StopBits_1_5    1.5个停止位
			USART_StopBits_2	  2个停止位

*/

void Gec_UART_Init(USART_TypeDef * USARTx,
						uint32_t bard_rate,
						uint32_t parity,
						uint32_t data_bits,
						uint32_t stop_bits
					 	)
{

	//1.GPIO口配置
	GPIO_InitTypeDef p;
	if (USARTx == USART1)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		p.GPIO_Mode = GPIO_Mode_AF;
		p.GPIO_OType = GPIO_OType_PP;
		p.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		p.GPIO_PuPd = GPIO_PuPd_NOPULL;
		p.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &p);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	} 
	else if (USARTx == USART2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		p.GPIO_Mode = GPIO_Mode_AF;
		p.GPIO_OType = GPIO_OType_PP;
		p.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		p.GPIO_PuPd = GPIO_PuPd_NOPULL;
		p.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &p);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	}
	else if (USARTx == USART3)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		p.GPIO_Mode = GPIO_Mode_AF;
		p.GPIO_OType = GPIO_OType_PP;
		p.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		p.GPIO_PuPd = GPIO_PuPd_NOPULL;
		p.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &p);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	}

		//2. USART配置
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
	USART_InitTypeDef u;


	uint16_t word_len;
	if ((data_bits == 8 && parity == USART_Parity_No ) || 
		(data_bits == 7 && parity != USART_Parity_No )  ) 
	{
		word_len = USART_WordLength_8b;
	}
	else if ((data_bits == 8 && parity != USART_Parity_No ) ||
		(data_bits == 9 && parity == USART_Parity_No ))
	{
		word_len = USART_WordLength_9b;
	}
	else 
	{
		//
	}
	
	
		
	u.USART_BaudRate =	bard_rate; // 115200,9600
	u.USART_WordLength = word_len;
	u.USART_StopBits = stop_bits;
	u.USART_Parity = parity;
	u.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx;
	u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USARTx, &u);

	
	//3. USART串口中断配置

	//配置RXNE标志产生USART串口中断
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE); //接收中断
	//USART_ITConfig(pu, USART_IT_TC, ENABLE);
	//NVIC_InitTypeDef n;
	//n.NVIC_IRQChannel = usart_irqn;
	//n.NVIC_IRQChannelCmd = ENABLE;
	//n.NVIC_IRQChannelPreemptionPriority = 2;
	//n.NVIC_IRQChannelSubPriority = 2;
	//NVIC_Init(&n);

	//4. 使能串口
	USART_Cmd(USARTx, ENABLE);
		

}

int Gec_UART_Write(USART_TypeDef * USARTx,
						uint8_t *buf,
						int count)
{
	//int r = 0;
	int cnt = 0;
	int i ;

	for (i = 0; i < count; i++)
	{
		cnt = 0;
		USART_SendData(USARTx, (uint16_t)buf[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET
				&& (++cnt) < 0xff)
		{
			mdelay(1);
		}

		if (cnt >= 0xff)
		{
			break;
		}
	}
	
	return i;
	
}

int Gec_UART_Read(USART_TypeDef * USARTx,
						uint8_t *buf,
						int count)
{
	int i;
	int cnt;
	for (i = 0; i < count; i++)
	{
		cnt = 0;
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET &&
			  ++cnt < 0xff)
		{
			mdelay(1);
		}

		if (cnt >= 0xff)
		{
			break;
		}

		buf[i] = USART_ReceiveData(USARTx);
	}

	return i;
}
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 
int fputc(int c, FILE *stream)
{
	USART_SendData(USART1, (uint16_t)c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return c;
}
