
#include <stdio.h>
#include <stdint.h>

#include "gec_time.h"
#include "gec_int.h"

#include "stm32f4xx.h"
#include "core_cm4.h"


#define rSysTickCTL   *((volatile unsigned long*)0xE000E010)
#define rSysTickRELOAD  *((volatile unsigned long*)0xE000E014)
#define rSysTickCURVAL  *((volatile unsigned long*)0xE000E018) 

#define  rDEMCtl    (*(volatile unsigned long *)0xE000EDFC)
#define  rDWTCtl    (*(volatile unsigned long *)0xE0001000)
#define  rCYCCnt  (*(volatile unsigned long *)0xE0001004)


typedef struct gec_timer
{
	int  timer_id; //定时器id,用来唯一标识一个定时器
	unsigned long expires; //超时时间
						   //毫秒为单位
	void (*timeout)(unsigned long) ; //超时执行的函数

	unsigned long data; //超时函数参数

	unsigned long auto_reload; //是否周期性加载

	// ...
} gec_timer_t;

//NONE_ONE_OFF_BIT  "非一次性"bit标志
#define  NONE_ONE_OFF_BIT	  16
#define GEC_TIMER_MAX_ID  65535
//系统同时允许的定时器数量
#define GEC_TIMER_MAX_NUM 20

static int cur_tid = 0;
static gec_timer_t  gec_timers[GEC_TIMER_MAX_NUM];


typedef struct gec_timer_list
{
	//gec_timer_t *first; //指向定时器链表中的第一个结点
	uint32_t lists[GEC_TIMER_MAX_NUM];

	int last; //最末尾定时器在lists中的下标。
			 // last == -1表示lists为空。
}gec_timer_list;

static gec_timer_list  gec_tlist;



static unsigned long cnt = 0;



static void inline Hardware_Timer_Disable(void);
static void inline Hardware_Timer_Enable(void);
static void Add_Timer(int timer_index);



static uint32_t cpu_freq_hz ;
/*
	设置一个计时监测点
*/
void Gec_Timing_Start(void)
{
	//先获取cpu的时钟频率
	RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
	cpu_freq_hz = rcc_clocks.HCLK_Frequency;
	//printf("cpu freq = %d hz\n", rcc_clocks.HCLK_Frequency);


	rDEMCtl |= 1 << 24;

	rCYCCnt = 0u;

	rDWTCtl |= 1 << 0;
}


/*
	获取离最近计时监测点经历的cpu时钟周期数
*/
uint32_t Gec_Get_CPU_TS(void)
{
	uint32_t cpu_ts;

	cpu_ts = rCYCCnt;
	return cpu_ts;
}


/*
	把CPU时钟周期数转换为微秒数
*/
uint32_t Gec_CPUTS_To_us(uint32_t ts)
{
	if (cpu_freq_hz >= 1000000)
	{
		return ts /(cpu_freq_hz / 1000000);
	}
	else if (cpu_freq_hz >= 1000)
	{
		return ts /(cpu_freq_hz / 1000) * 1000;
	}
	else
	{
		return ts / cpu_freq_hz * 1000000;
	}
}




/*
	mdelay:毫秒级别的延时函数

*/
void mdelay(uint32_t d)
{
	cnt = d;
	
	SysTick_Config(SystemCoreClock/1000);
	NVIC_SetPriority(SysTick_IRQn, 0); //设置SysTick中断的优先级


	while (cnt != 0 );

	rSysTickCTL &=	~(1 << 0) ; // bit0 -> 0  disable SysTick

}

/*
	udelay:微秒级别的延时函数
*/
void udelay(uint32_t u)
{
	cnt = u;

	SysTick_Config(SystemCoreClock/1000000);
	NVIC_SetPriority(SysTick_IRQn, 1); //设置SysTick中断的优先级

	
	
	while (cnt != 0);

	rSysTickCTL &=  ~(1 << 0) ; // bit0 -> 0  disable SysTick
}

//SysTick定时器中断的处理函数
void SysTick_Handler(void)
{
	
	if(cnt > 0)
	{
		cnt--;
	}

}

/*
	Del_Timeout_Timer:删除已经超时的定时器
	@timer_index: 要删除的定时器在“活动定时器列表”中
				的下标。
*/
static void Del_Timeout_Timer(int timer_index)
{
	int i = gec_tlist.lists[timer_index]; //原始定时器数组中的下标
	int k;
	gec_timer_t *p = &gec_timers[i];
	

	for (k = timer_index; k < gec_tlist.last; k++)
	{
		gec_tlist.lists[k] = gec_tlist.lists[k+1];
	}

	gec_tlist.last--;


	if (p->auto_reload)
	{
		//周期性定时器
		p->expires= p->auto_reload + 1;
		Add_Timer(i);
		
	}
	else
	{
		p->timer_id = -1; //删除它
	}


	if (gec_tlist.last == -1)
	{
		Hardware_Timer_Disable();
	}
}


//硬件定时器中断处理函数
// 让它1ms中断一次
void Hardware_Timer_IRQHandler(void)
{
	int i;
	int t_index;
	timer_func_t pFunc ;

	//Gec_GPIO_Set_Value(GPIOF, GPIO_Pin_9, 0);
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		for (i = 0; i <= gec_tlist.last ;i++)
		{
			t_index = gec_tlist.lists[i];
			gec_timers[t_index].expires -- ;
			//printf("t_index = %d, gec_timers[t_index].expires = %d\n",
			//		t_index, gec_timers[t_index].expires);
			if (gec_timers[t_index].expires == 0)
			{
				pFunc = gec_timers[t_index].timeout;
				(*pFunc)(gec_timers[t_index].data);

				//gec_timers[t_index].timer_id = -1;
				Del_Timeout_Timer(i);
				i--;
			}
		}

		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}


static void inline Hardware_Timer_Enable(void)
{
	//Gec_Int_Enable(BSP_INT_ID_TIM7);
	TIM_Cmd(TIM7, ENABLE);
}

static void inline Hardware_Timer_Disable(void)
{
	//Gec_Int_Disable(BSP_INT_ID_TIM7);
	TIM_Cmd(TIM7, DISABLE);
}

 void Hardware_Timer_Init(void)
{
	//1. 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	//NVIC_InitTypeDef n;
	//n.NVIC_IRQChannel = TIM7_IRQn;
	//n.NVIC_IRQChannelCmd = ENABLE;
	//n.NVIC_IRQChannelPreemptionPriority = 3;
	//n.NVIC_IRQChannelSubPriority = 3;
	//NVIC_Init(&n);

	Gec_Int_Register(BSP_INT_ID_TIM7, Hardware_Timer_IRQHandler);
	Gec_Int_Enable(BSP_INT_ID_TIM7);
	
	TIM_TimeBaseInitTypeDef  t;

	t.TIM_CounterMode = TIM_CounterMode_Up;
	t.TIM_Period = 1000;
	t.TIM_Prescaler = 83;
	t.TIM_ClockDivision = 0;
	
	TIM_TimeBaseInit(TIM7, &t);

	/* Prescaler configuration */
	//TIM_PrescalerConfig(TIM3, 83, TIM_PSCReloadMode_Immediate);


	//2. 
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	//TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	//Gec_Int_Register(BSP_INT_ID_TIM7, Hardware_Timer_IRQHandler);
	//Gec_Int_Enable(BSP_INT_ID_TIM7);



	//3.
	TIM_ARRPreloadConfig(TIM7, ENABLE);
	//TIM_Cmd(TIM7, ENABLE);
	
}

/*
	Add_Timer:	把　原始定时器数组中的第timer_index个
				定时器　加入到　　活动定时器列表中去
	@timer_inder: 定时器在原始定时器数组gec_timers中的
			下标
	返回值:
		无。
*/
static void Add_Timer(int timer_index)
{
	int i = 0;
	int k;

	for (i = 0; i <= gec_tlist.last; i++)
	{
		if ( gec_timers[gec_tlist.lists[i]].expires > 
			gec_timers[timer_index].expires)
		{
			break;
		}
	}

	for (k = gec_tlist.last; k >= i; k--)
	{
		gec_tlist.lists[k+1] = gec_tlist.lists[k];
	}

	gec_tlist.lists[i] = timer_index;
	gec_tlist.last++;


	//第一个定时器，启动硬件定时器
	//printf("gec_tlist.last = %d\n", gec_tlist.last);
	if (gec_tlist.last == 0)
	{
		Hardware_Timer_Enable();
	}
	
}



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
						void (*pFunc)(unsigned long), //超时函数
						unsigned long data, //超时函数参数
						int one_off  //是否为“一次性”
		)
{
	int tid = -1;

	int i;

	if (expires == 0)
	{
		return -1;
	}


	// 
	Gec_Int_Disable(BSP_INT_ID_TIM7);

	for (i = 0; i < GEC_TIMER_MAX_NUM; i++)
	{
		//printf("i = %d, gec_timers[i].timer_id = %d\n", i,gec_timers[i].timer_id);
		if (gec_timers[i].timer_id == -1)
		{	
					
			tid =  (++cur_tid) % GEC_TIMER_MAX_ID;
			gec_timers[i].timer_id = tid;
			gec_timers[i].expires = expires;
			gec_timers[i].timeout = pFunc;
			gec_timers[i].data = data;
			gec_timers[i].auto_reload = (one_off) ? 0 : expires;
			
			break;
		}
	}

	if (tid != -1)
	{
		Add_Timer(i);
	}

	Gec_Int_Enable(BSP_INT_ID_TIM7);

	return tid;
}


/*
	Gec_Timer_Del:用来删除timer_id指定的定时器
	@timer_id: 定时器id,Gec_Timer_Add的返回值

	NOTE:
		“一次性”定时器在超时后，系统会自动将它删除。
		
*/
void Gec_Timer_Del(int timer_id)
{
	int i;

	if (timer_id == -1)
	{
		return ;
	}

	Gec_Int_Disable(BSP_INT_ID_TIM7);
	
	for (i = 0;  i <= gec_tlist.last; i++)
	{
		int index = gec_tlist.lists[i];
		if (gec_timers[index].timer_id == timer_id)
		{
			gec_timers[index].auto_reload = 0;
			Del_Timeout_Timer(i);
			break;
		}
	}

	Gec_Int_Enable(BSP_INT_ID_TIM7);

	return ;
}

/*
	Gec_Timer_Mod:修改定时器的超时时间
	@timer_id:定时器id
	@expires: 新的超时时间
	返回值:
		无。
*/
void Gec_Timer_Mod(int timer_id, 
					   unsigned long expires)
{
	int i;
	int index;
	int t;
	
	Gec_Int_Disable(BSP_INT_ID_TIM7);


	for (i = 0; i <= gec_tlist.last; i++)
	{
		index = gec_tlist.lists[i];
		if (gec_timers[index].timer_id == timer_id)
		{
			break;
		}
	}

	if (i > gec_tlist.last ||  expires == gec_timers[index].expires)
	{
		goto mod_exit;
	}

	if (expires > gec_timers[index].expires)
	{
		//新超时时间 > 当前的，往后“冒泡”
		gec_timers[index].expires = expires;
		for (i = index; i <= gec_tlist.last; i++)
		{
			if (gec_timers[ gec_tlist.lists[i] ].expires > 
				gec_timers[ gec_tlist.lists[i+1] ].expires)
			{
				t = gec_tlist.lists[i];
				gec_tlist.lists[i] = gec_tlist.lists[i+1];
				gec_tlist.lists[i+1] = t;
			}
			else
			{
				break;
			}
		}
	
	}
	else
	{
		//新超时时间 < 当前的，往前“冒泡”
		gec_timers[index].expires = expires;
		for (i = index; i > 0; i--)
		{
			if (gec_timers[ gec_tlist.lists[i] ].expires <
				gec_timers[ gec_tlist.lists[i-1] ].expires)
			{
				t = gec_tlist.lists[i];
				gec_tlist.lists[i] = gec_tlist.lists[i-1];
				gec_tlist.lists[i-1] = t;
			}
			else
			{
				break;
			}
		}
	}
	
mod_exit:
	Gec_Int_Enable(BSP_INT_ID_TIM7);

	return ;
}


void Gec_Timer_Init(void)
{
	int i ;



	for (i = 0; i < GEC_TIMER_MAX_NUM; i++)
	{
		gec_timers[i].timer_id = -1;
		//gec_tlist.lists[i] = -1; //
	}

	gec_tlist.last = -1;

	Hardware_Timer_Init();
}
