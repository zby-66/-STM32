

extern void Gec_Int_Init(void);
extern void Gec_Timer_Init(void);

//该函数需要在汇编或C MAIN函数中首先调用
//初始化Gec_Lib
void Gec_Lib_Init(void)
{

	//初始化 中断管理
	Gec_Int_Init();


	//初始化　定时器
	Gec_Timer_Init();
}
