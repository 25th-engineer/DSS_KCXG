/*
*********************************************************************************************************
* 文件: Touch_Screen.C

*********************************************************************************************************
*/
#include  "Includes.h"

volatile int xdata, ydata; 
TouchPos  TouchXY;
 
//////////////////////////////////////////////
// ADCCON寄存器
#define PRESCALE_DIS        (0 << 14)
#define PRESCALE_EN         (1 << 14)
#define PRSCVL(x)           ((x) << 6)
#define ADC_INPUT(x)        ((x) << 3)
#define ADC_START           (1 << 0)
#define ADC_ENDCVT          (1 << 15)

// ADCTSC寄存器
#define UD_SEN          (1 << 8)
#define DOWN_INT        (UD_SEN*0)
#define UP_INT          (UD_SEN*1)
#define YM_SEN          (1 << 7)
#define YM_HIZ          (YM_SEN*0)
#define YM_GND          (YM_SEN*1)
#define YP_SEN          (1 << 6)
#define YP_EXTVLT       (YP_SEN*0)
#define YP_AIN          (YP_SEN*1)
#define XM_SEN          (1 << 5)
#define XM_HIZ          (XM_SEN*0)
#define XM_GND          (XM_SEN*1)
#define XP_SEN          (1 << 4)
#define XP_EXTVLT       (XP_SEN*0)
#define XP_AIN          (XP_SEN*1)
#define XP_PULL_UP      (1 << 3)
#define XP_PULL_UP_EN   (XP_PULL_UP*0)
#define XP_PULL_UP_DIS  (XP_PULL_UP*1)
#define AUTO_PST        (1 << 2)
#define CONVERT_MAN     (AUTO_PST*0)
#define CONVERT_AUTO    (AUTO_PST*1)
#define XP_PST(x)       (x << 0)

#define NOP_MODE        0
#define X_AXIS_MODE     1
#define Y_AXIS_MODE     2
#define WAIT_INT_MODE   3


/* 设置进入等待中断模式，XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
 * (1)对于S3C2410，位[8]只能为0，所以只能使用下面的wait_down_int，
 *    它既等待Pen Down中断，也等待Pen Up中断
 * (2)对于S3C2440，位[8]为0、1时分别表示等待Pen Down中断或Pen Up中断
 */
/* 进入"等待中断模式"，等待触摸屏被按下 */
#define wait_down_int() { rADCTSC = DOWN_INT | XP_PULL_UP_EN | \
                          XP_AIN | XM_HIZ | YP_AIN | YM_GND | \
                          XP_PST(WAIT_INT_MODE); }
/* 进入"等待中断模式"，等待触摸屏被松开 */
#define wait_up_int()   { rADCTSC = UP_INT | XP_PULL_UP_EN | XP_AIN | XM_HIZ | \
                          YP_AIN | YM_GND | XP_PST(WAIT_INT_MODE); }

/* 进入自动(连续) X/Y轴坐标转换模式 */
#define mode_auto_xy()  { rADCTSC = CONVERT_AUTO | XP_PULL_UP_DIS | XP_PST(NOP_MODE); }



/* 
 * INT_TC的中断服务程序
 * 当触摸屏被按下时，进入自动(连续) X/Y轴坐标转换模式；
 * 当触摸屏被松开时，进入等待中断模式，再次等待INT_TC中断
 */ 
/////////////////////////////////////
 void Isr_Tc(void)
{
    if (rADCDAT0 & 0x8000)//当触摸屏被松开时
    {
      //  printf("Stylus Up!!\n\r");
        GUI_TOUCH_StoreState(-1,-1);
        xdata=ydata=0;
        wait_down_int();    /* 进入"等待中断模式"，等待触摸屏被按下 */
    }
    else 
    {
      //  printf("Stylus Down: ");//当触摸屏被按下时

        mode_auto_xy();     /* 进入自动(连续) X/Y轴坐标转换模式 */
    
        /* 设置位[0]为1，启动A/D转换
         * 注意：ADCDLY为50000，PCLK = 50MHz，
         *       要经过(1/50MHz)*50000=1ms之后才开始转换X坐标
         *       再经过1ms之后才开始转换Y坐标
         */
        rADCCON |= ADC_START;
    }
    
    // 清INT_TC中断
    rSUBSRCPND |= BIT_SUB_TC;
    rSRCPND    |= BIT_ADC;
    rINTPND    |= BIT_ADC;
}






/* 
 * INT_ADC的中断服务程序
 * A/D转换结束时发生此中断
 * 先读取X、Y坐标值，再进入等待中断模式
 */       
 void Isr_Adc(void)
{
    // 打印X、Y坐标值    
   // printf("xdata = %4d, ydata = %4d\r\n", (int)(rADCDAT0 & 0x3ff), (int)(rADCDAT1 & 0x3ff));
    xdata=(rADCDAT0&0x3ff);
    ydata=(rADCDAT1&0x3ff);
    
    ///////
    TouchXY.TouchX = xdata;
    TouchXY.TouchY = ydata;
    //////
    
	GUI_TOUCH_Exec();
    /* 判断是S3C2410还是S3C2440 */
    if ((rGSTATUS1 == 0x32410000) || (rGSTATUS1 == 0x32410002))
    {   // S3C2410
        wait_down_int();    /* 进入"等待中断模式"，等待触摸屏被松开 */
    }
    else
    {   // S3C2440
        wait_up_int();      /* 进入"等待中断模式"，等待触摸屏被松开 */
    }
	
    // 清INT_ADC中断
    rSUBSRCPND |= BIT_SUB_ADC;
    rSRCPND    |= BIT_ADC;
    rINTPND    |= BIT_ADC;
}



/* 
 * ADC、触摸屏的中断服务程序
 * 对于INT_TC、INT_ADC中断，分别调用它们的处理程序
 */

void AdcTsIntHandle(void)
{
    if (rSUBSRCPND & BIT_SUB_TC)
        Isr_Tc();

    if (rSUBSRCPND & BIT_SUB_ADC)
        Isr_Adc();
}



//初始化触摸屏的中断服务程序

void Init_Ts(void)
{
    pISR_ADC = (int)AdcTsIntHandle;    // 设置ADC中断服务程序
    rINTMSK &= ~BIT_ADC;          // 开启ADC总中断
    rINTSUBMSK &= ~(BIT_SUB_TC);  // 开启INT_TC中断，即触摸屏被按下或松开时产生中断
    rINTSUBMSK &= ~(BIT_SUB_ADC); // 开启INT_ADC中断，即A/D转换结束时产生中断
    
    // 使能预分频功能，设置A/D转换器的时钟 = PCLK/(49+1)
    rADCCON = PRESCALE_EN | PRSCVL(49);

    /* 采样延时时间 = (1/3.6864M)*50000 = 13.56ms
     * 即按下触摸屏后，再过13.56ms才采样
     */
    rADCDLY = 50000;

    wait_down_int();    /* 进入"等待中断模式"，等待触摸屏被按下 */
}







void TOUCH_X_ActivateX(void)
{
}

void TOUCH_X_ActivateY(void)
{
}

void TOUCH_X_Disable(void)   {}

int  TOUCH_X_MeasureX(void)
{
    return xdata;
}

int  TOUCH_X_MeasureY(void)
{
    return ydata;
}

/*
********************************************************************************************************
* End.
********************************************************************************************************
*/
