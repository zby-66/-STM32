#ifndef __GEC_UART_H__
#define __GEC_UART_H__

#include <stdint.h>
#include "stm32f4xx.h"

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
					 	);




/*
	Gec_UART_Write:往一个指定的串口中写数据
	@USARTx: 指定要写哪个串口，如：USART1, USART2,...
	@buf: 指针，指向要写入串口的数据的首地址
	@count: 表示要写多少字节数据
	返回值:
		返回实际写入串口的字节数，
		0表示什么也没写。
*/
int Gec_UART_Write(USART_TypeDef * USARTx,
						uint8_t *buf,
						int count);


/*
	Gec_UART_Read: 从一个指定的串口中读取数据
	@USARTx: 指定要从哪个串口中，读取数据
	@buf: 指针，读取的数据保存到哪里去
	@count:表示要读多少字节数据
	返回值:
		返回实际读到的字节数
		0表示什么也没读到。
*/
int Gec_UART_Read(USART_TypeDef * USARTx,
						uint8_t *buf,
						int count);






#endif
