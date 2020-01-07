
#include  "config.h"

#define rs (1<<8)
#define rw (1<<9)
#define en (1<<10)
#define busy (1<<7)

#define  TASK_STK_SIZE                  64
OS_STK        TaskStartStk[TASK_STK_SIZE];

void  TaskStart(void *data);
uint8 txt[]={"A LPC2124 Example!"};
/****************************************************************************
* 名称：ChkBusy()
* 功能：检查总线是否忙
****************************************************************************/
void ChkBusy()
{
	PINSEL0=0xffc00000;
	IO0DIR=0x700;
	while(1)
	{
		IO0CLR=rs;
		IO0SET=rw;
		IO0SET=en;
		if(!(IO0PIN & busy))break;
		IO0CLR=en;
	}
	IO0DIR=0x7ff;
}
/****************************************************************************
* 名称：WrOp()
* 功能：写函数
****************************************************************************/
void WrOp(uint8 dat)
{
	
	ChkBusy();
	IO0CLR=rs;		//全部清零
	IO0CLR=rw;
	IO0CLR=0xff;		//先清零
	IO0SET=dat;		//再送数
	IO0SET=en;
	IO0CLR=en;
}
/****************************************************************************
* 名称：WrDat()
* 功能：写数据函数
****************************************************************************/
void WrDat(uint8 dat)	//读数据
{
   
	ChkBusy();
	IO0SET=rs;
	IO0CLR=rw;
	IO0CLR=0xff;		//先清零
	IO0SET=dat;		//再送数
	IO0SET=en;
	IO0CLR=en;
}

/****************************************************************************
* 名称：DisText()
* 功能：显示文本函数
****************************************************************************/

void DisplayText(uint8 addr,uint8 *p)
{
	WrOp(addr);
	while(*p !='\0')WrDat(*(p++));
}
/****************************************************************************
* 名称：main()
* 功能：显示文本
****************************************************************************/

 int main (void)
{
    OSInit();

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 3);
    OSStart();
    return 0;
}
/****************************************************************************
* 名称：TaskStart()
* 功能：任务
****************************************************************************/

void  TaskStart(void *pdata)
{   pdata=pdata;
	TargetInit();
    WrOp(0x0c);
	IO0DIR=0x7ff;		//设置为输出
	IO0CLR=0x7ff;
	DisplayText(0x80,txt);
	while(1);
}
