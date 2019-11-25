#ifndef __GEC_TIME_H__
#define __GEC_TIME_H__

#include <stdint.h>



///---------------------定时功能 API---------------------

//定时器"超时函数"原型
typedef void (*timer_func_t)(unsigned long);


/*
	Gec_Timer_Add:注册一个定时器
	@expires:定时器的超时时间，单位为毫秒
	@pFunc: 函数指针，超时执行的函数
	@data: 超时执行的函数的参数
	@one_off: 是否为“一次性”
			 1 定时器只是一次超时
			 0 定时器为周期性
	返回值:
		成功返回定时器的id(>=0, 用来唯一标识你申请的定时器)
		失败返回-1
*/
int Gec_Timer_Add( unsigned long expires, //超时时间，单位为毫秒
						timer_func_t pFunc, //超时函数
						unsigned long data, //超时函数参数
						int one_off  //是否为“一次性”
		);


/*
	Gec_Timer_Mod:修改定时器的超时时间
	@timer_id:定时器id
	@expires: 新的超时时间
	返回值:
		无。
*/
void Gec_Timer_Mod(int timer_id, 
					   unsigned long expires);
/*
	Gec_Timer_Del:用来删除timer_id指定的定时器
	@timer_id: 定时器id,Gec_Timer_Add的返回值

	NOTE:
		“一次性”定时器在超时后，系统会自动将它删除。
		
*/
void Gec_Timer_Del(int timer_id);



/// ----------------延时功能 API----------------------


/*
	mdelay:毫秒级别的延时函数

*/
void mdelay(uint32_t);


/*
	udelay:微秒级别的延时函数
*/
void udelay(uint32_t);




/// ------------------计时功能 API-------------------
/*
	设置一个计时监测点
*/
void Gec_Timing_Start(void);


/*
	获取离最近计时监测点经历的cpu时钟周期数
*/
uint32_t Gec_Get_CPU_TS(void);


/*
	把CPU时钟周期数转换为微秒数
*/
uint32_t Gec_CPUTS_To_us(uint32_t);


#endif
