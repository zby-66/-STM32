#include "gec_uart.h"
#include "gec_int.h"
#include "led.h"
#include "gec_gpio.h"

#define OUTPUT 1
#define INPUT  0

#define uchar unsigned char 
#define uint unsigned int

int get_data(void)
{
	//返回串口的值
	return Gec_GPIO_Get_Value(GPIOB,GPIO_Pin_7);    
}

void set_data(unsigned short sta)
{
	//根据所传参数，判断串口的开关
	Gec_GPIO_Set_Value(GPIOB,GPIO_Pin_7,sta);
}

void data_init(unsigned char cmd)
{
	//对每次数值的更新
	cmd ? Gec_GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_MODE_OUT_PP) : Gec_GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_MODE_IN_FLOATING); 
}

uint flag;
uint t;

unsigned short over_time_counter;
uchar RH_data_H,RH_data_L,TEM_data_H,TEM_data_L,check_data;
uchar TEM_data_H_temp,TEM_data_L_temp,RH_data_H_temp,RH_data_L_temp,check_data_temp;
uchar byte_value;



void Read_Byte(void)
{
	uchar i;
	uchar bit_value;
	for(i = 0;i < 8;i++)
	{
		//越过50us的低电平时隙
		over_time_counter = 2;
		while(!get_data() && over_time_counter++)
			;
		//拉高后过去30us判断总线是否还为高电平,如果为高,此数据位为1,如果为低,此数据位为0
		udelay(30);
		if(get_data())//还为高电平则值为1，否则值为0
		{
			bit_value = 1;
		}
		else
		{
			bit_value = 0;
		}
		//等待下一位开始
		over_time_counter = 2;
		while(get_data() && over_time_counter++);
		//错误处理,如果数据传输出错,LED_ERROR灯亮,LED_RIGHT灯灭
		
		if(over_time_counter == 1)
		{
			Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,0);  //好的
			Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,1); //灭了
			break;
		}
		//将获取到的位数进行处理,先位移,再或上最后一位
		byte_value = byte_value << 1;
		byte_value = byte_value | bit_value;	
	}
}

unsigned char RH[3] = {0};
unsigned char TEM[3] = {0};
 
void del_data(void)
{
	//求出湿度数值的个十百位，并与字符相结合转换为字符，便于输出。功能为将16进制转换为10进制(并没有真的转换)
	//不得不说龙哥牛逼，温度同理。
	RH[0] = RH_data_H / 100 + '0';
	RH[1] = RH_data_H / 10 % 10 + '0';
	RH[2] = RH_data_H % 10 + '0';
	TEM[0] = TEM_data_H / 100 + '0';
	TEM[1] = TEM_data_H /10 % 10 + '0';
	TEM[2] = TEM_data_H % 10 + '0';
}

void set_warning(void)
{
	//判断湿度是否超过90，温度是否超过30（可根据实际情况改）
	if(RH_data_H / 10 % 10 >= 9   || TEM_data_H /10 % 10 >= 3)
	{
		//超过则蜂鸣器响起，报警
		Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_8,1);
	}
	else
	{
		//正常时，报告现在没警告，并将蜂鸣器一直保持为零。诺蜂鸣器开始报警，那当降到正常值的时候，关闭蜂鸣器。
		Gec_UART_Write(USART1,"NOW IS OK",9);
		Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_8,0);
	}
}


void set_DATA(void)
{
	//添加提示信息：“这是湿度”
	Gec_UART_Write(USART1,"This is RH",10);
	//将湿度数据写入到屏幕上
	Gec_UART_Write(USART1,RH,3);
	//添加提示信息：“这是温度”
	Gec_UART_Write(USART1,"This is TEM",11);
	//将温度数据写入到屏幕上
	Gec_UART_Write(USART1,TEM,3);
}

void read_dht11(void)
{
	uchar calc_check_data; 
	data_init(OUTPUT);//总线切换成输出模式
	set_data(1);//将其设置为高电平
	set_data(0);//将总线拉低,发送开始信号
	mdelay(20);
	//等待40us后,读取DHT11的响应信号,同时将总线拉高	
	set_data(1);//将总线拉高,结束开始信号
	udelay(40);
	data_init(INPUT);//将总线切换为输入模式,总线由从机接管

	//等待DTH11把总线拉低
	if(!get_data())
	{
		//如果进入此if语句,说明DHT11已经响应开始信号,将总线拉低
		//此时一直等待DHT11再次将总线拉高,同时设置over_time_counter来避免等待超时
		//over_time_counter为unsigned char,最大1111 1111(255),当再次加1时,为0,即跳出while
		over_time_counter = 2;
		while(!get_data() && over_time_counter++)
			;
		//等待总线电位降低,意味着DHT11开始发送数据,等待50us低电平时隙过去
		over_time_counter = 2;
		while(get_data() && over_time_counter++)
			;
		//按顺序读取值
		Read_Byte();
		RH_data_H_temp = byte_value;//湿度高八位
		Read_Byte();
		RH_data_L_temp = byte_value;//湿度低八位
		Read_Byte();
		TEM_data_H_temp = byte_value;//温度高八位
		Read_Byte();
		TEM_data_L_temp = byte_value;//温度低八位
		Read_Byte();
		check_data_temp = byte_value;//校验和

		//设置为输出模式
		data_init(OUTPUT);
		//读取完后将总线拉高
		set_data(1);
		
		//检查读取是否有误,如果无误,LED_ERROR灯灭,LED_RIGHT灯亮
		calc_check_data = (RH_data_H_temp + RH_data_L_temp + TEM_data_H_temp + TEM_data_L_temp);
		if(calc_check_data == check_data_temp)
		{
			RH_data_H = RH_data_H_temp;
			RH_data_L = RH_data_L_temp;
			TEM_data_H = TEM_data_H_temp;
			TEM_data_L = TEM_data_L_temp;
			check_data = check_data_temp;
			del_data();			//对所读数据进行计算转换
			set_warning();		//判断是否超过所设计的值
			set_DATA();			//将提示以及所读数据传输到屏幕
			Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_9,1);
			Gec_GPIO_Set_Value(GPIOF,GPIO_Pin_10,0);
		}
	}	
}

void uart1_init(void)
{	
    //初始化串口2
    Gec_UART_Init(USART1,9600,USART_Parity_No,8,USART_StopBits_1);
    //使能串口2
    Gec_Int_Enable(BSP_INT_ID_USART1);
}
