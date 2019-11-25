
#include <stdint.h>
#include "stm32f4xx.h"
#include "gec_time.h"



//返回1事件产生
//返回0超时。
static int inline WAIT_EVx_TIMEOUT(I2C_TypeDef* I2Cx, uint32_t  EVx,  uint32_t t)
{
	while ((I2C_CheckEvent(I2Cx, EVx) == ERROR) && --t) 
	{
		mdelay(1); 
	}

	return (t == 0) ? 0 : 1; 
}


/*
	Gec_I2C_Init:初始化I2Cx控制器
	@I2Cx:
	@i2c_clock_freq:
	返回值:
		无。

	
*/
void Gec_I2C_Init(I2C_TypeDef* I2Cx, uint32_t i2c_clock_freq)
{
	// 1. 初始化I2C的引脚SDA, SCL -> pb8/pb9复用
	//
	if (I2Cx == I2C1)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitTypeDef p;
		p.GPIO_Mode = GPIO_Mode_AF;
		p.GPIO_OType = GPIO_OType_OD; //外部已经接了一个上拉电阻了。
		p.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		p.GPIO_PuPd = GPIO_PuPd_NOPULL; //外部已经接了一个上拉电阻了。
		p.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &p);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,GPIO_AF_I2C1);
	}



	//2. 初始化I2C控制器
	if (I2Cx == I2C1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	} 
	else if (I2Cx == I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	}
	else if (I2Cx == I2C3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE);
	}
	
	
	I2C_InitTypeDef i;
	i.I2C_Ack = I2C_Ack_Enable;
	i.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i.I2C_ClockSpeed = i2c_clock_freq; //得看　从设备的频率
	i.I2C_DutyCycle = I2C_DutyCycle_16_9;
	i.I2C_Mode = I2C_Mode_I2C;
	i.I2C_OwnAddress1 = 0x77; //随便写,主机当从设备时，需要用到
	I2C_Init(I2Cx, &i);


	//3. 配置I2C的其他一些功能:
	//	如：中断配置，ACK配置...
	//I2C_AcknowledgeConfig(I2C1,DISABLE);


	//4. ENABLE I2C
	I2C_Cmd(I2Cx, ENABLE);


}

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
			)
{
	int i;
	int time ;//等待时长，单位为 ms

	// wait I2C BUS  not BUSY!!!!
	time = 1000;
	while ( (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET) && --time )
	{
		mdelay(1);
	}
	if (time == 0)
	{
		return -1; //超时
	}

	// Send START SIGNAL
	I2C_GenerateSTART(I2Cx, ENABLE);

	// wait ev5
	if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, 250) == 0)
	{
		return -1; //超时。
	}


	// Send I2C Slave Addr
	I2C_Send7bitAddress(I2Cx, i2c_dev_addr, I2C_Direction_Transmitter);

	// Wait EV6
	if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,300) == 0)
	{
		return -1; //超时。
	}


	//Send DATAs
	for (i = 0; i < count; i++)
	{
		I2C_SendData(I2Cx, data[i]);
		if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, 100) == 0)
		{
			break;
		}
	}

	//Send STOP 
	I2C_GenerateSTOP(I2Cx, ENABLE);


	//if (i > 0)
	//{
		
	//}
	
	return i;

}


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
					  int count)
{


	int i;
	int time ;//等待时长，单位为 ms

	// wait I2C BUS  not BUSY!!!!
	time = 1000;
	while ( (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET) && --time )
	{
		mdelay(1);
	}
	if (time == 0)
	{
		return -1; //超时
	}

	// Send START SIGNAL
	I2C_GenerateSTART(I2Cx, ENABLE);

	// wait ev5
	if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, 250) == 0)
	{
		return -1; //超时。
	}


	// Send I2C Slave Addr
	I2C_Send7bitAddress(I2Cx, i2c_dev_addr, I2C_Direction_Receiver);

	// Wait EV6
	if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, 300) == 0)
	{
		return -1; //超时。
	}


	//read DATAs
	for (i = 0; i < count - 1; i++)
	{
		if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, 200) == 0)
		{
			break;
		}
		data[i] = I2C_ReceiveData(I2Cx);
	}


	I2C_AcknowledgeConfig(I2Cx, DISABLE); //最后一个字节，不回复ACK
	
	if (WAIT_EVx_TIMEOUT(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, 200) == 1)
	{
		data[i++] = I2C_ReceiveData(I2Cx);
	}

	
	I2C_AcknowledgeConfig(I2Cx, ENABLE); //恢复成默认值　ACK enable

	

	//Send STOP 
	I2C_GenerateSTOP(I2Cx, ENABLE);


	//if (i > 0)
	//{
		
	//}
	
	return i;

}
