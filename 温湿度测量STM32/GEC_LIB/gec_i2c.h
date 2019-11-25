#ifndef __GEC_I2C_H__
#define __GEC_I2C_H__

#include <stdint.h>
#include "stm32f4xx.h"

/*
	Gec_I2C_Init:初始化I2Cx控制器
	@I2Cx:
	@i2c_clock_freq:
	返回值:
		无。

	
*/
void Gec_I2C_Init(I2C_TypeDef* I2Cx, uint32_t i2c_clock_freq);


/*
	Gec_I2C_Write:往I2C总线写数据
	@I2Cx:
	@i2c_dev_addr: I2C从设备地址
			i2c_dev_addr = 	7bits_i2c_addr << 1
	@data: 要写的数据的首地址
	@count:要写的字节数
	返回值:
		成功写到I2C总线上的字节数
		失败返回-1.

	时序:
		MCU  : IIC_START  i2c_dev_addr  data[0]  data[1] ...  data[count-1]  IIC_Stop
		SLAVE:						  A 		A		A   A			   A
*/
int Gec_I2C_Write(I2C_TypeDef* I2Cx,
					  uint8_t  i2c_dev_addr,
					  uint8_t  *data,
					  int count
			);

/*
	Gec_I2C_Write:从I2C总线读数据
	@I2Cx:
	@i2c_dev_addr: I2C从设备地址
			i2c_dev_addr = 	7bits_i2c_addr << 1
	@data: 数据缓冲区首地址，读到哪里去
	@count:要读的字节数
	返回值:
		成功返回从I2C总线上读取到的字节数
		失败返回-1.

	时序:
		MCU  : IIC_START  i2c_dev_addr|1      A	    A        A     NA IIC_Stop
		SLAVE:						    A data  data ...data  data
*/

int Gec_I2C_Read(I2C_TypeDef* I2Cx,
					  uint8_t  i2c_dev_addr,
					  uint8_t  *data,
					  int count);


#endif
