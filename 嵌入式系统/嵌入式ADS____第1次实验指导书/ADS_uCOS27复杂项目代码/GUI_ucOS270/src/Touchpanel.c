/*****************************************
  NAME: Touchpanel.c
  DESC: ADC & Touch screen test
  HISTORY:
  2003.09.23:Leon YH KIM: draft ver 1.0
 *****************************************/
#include "Includes.h"
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"



//extern OS_EVENT *MboxTouchpanel;





#define REQCNT 30
#define ADCPRS 9	//YH 0627
#define LOOP 1

void __irq AdcTsAuto(void);
void TouchpanelInt(void);

int count=0;
extern volatile int xdata, ydata; 
    
void Test_Touchpanel(void)
{
   
    rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

    Uart_Printf("ADC touch screen test\n");

    rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

    pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

	Uart_Printf("\nType any key to exit!!!\n");
	Uart_Printf("\nStylus Down, please...... \n");
	Uart_Getch();

	rINTSUBMSK|=BIT_SUB_TC;
	rINTMSK|=BIT_ADC;
	Uart_Printf("Touch Screen Test is Finished!!!\n");

}




void __irq AdcTsAuto(void)

{
	int i;
	U32 saveAdcdly;

    if(rADCDAT0&0x8000)
    {
		//Uart_Printf("\nStylus Up!!\n");
		rADCTSC&=0xff;	// Set stylus down interrupt bit
    }
    //else 
		//Uart_Printf("\nStylus Down!!\n");

	rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
	saveAdcdly=rADCDLY;
	rADCDLY=40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms

	rADCCON|=0x1;                   //start ADC

		while(rADCCON & 0x1);		//check if Enable_start is low
		while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!!
		
            while(!(rSRCPND & (BIT_ADC)));  //check if ADC is finished with interrupt bit

            xdata=(rADCDAT0&0x3ff);
            ydata=(rADCDAT1&0x3ff);

	 //YH 0627, To check Stylus Up Interrupt.
	 rSUBSRCPND|=BIT_SUB_TC;
	 ClearPending(BIT_ADC);
	 rINTSUBMSK=~(BIT_SUB_TC);
	 rINTMSK=~(BIT_ADC);
			 
	 rADCTSC =0xd3;    //Waiting for interrupt
	 rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal.

			while(1)		//to check Pen-up state
			{
			 if(rSUBSRCPND & (BIT_SUB_TC))	//check if ADC is finished with interrupt bit
				 {
					//Uart_Printf("Stylus Up Interrupt~!\n");
					break;	//if Stylus is up(1) state
				}
			}	

    Uart_Printf("count=%03d  XP=%04d, YP=%04d\n", count++, xdata, ydata);    //X-position Conversion data            

	rADCDLY=saveAdcdly; 
	rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal.
    rSUBSRCPND|=BIT_SUB_TC;
    rINTSUBMSK=~(BIT_SUB_TC);	// Unmask sub interrupt (TC)     
    ClearPending(BIT_ADC);
}

extern void AdcTsIntHandle(void);

/* 
 * 测试触摸屏，打印触点坐标
 */     

///**************************for ucos************************///

void InitTouchpanel(void) 
{
   
    rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

    //Uart_Printf("ADC touch screen test\n");

    rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

    pISR_ADC = (int)AdcTsIntHandle;
	rINTMSK &= ~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK &= ~(BIT_SUB_TC);

	//Uart_Printf("\nType any key to exit!!!\n");
	//Uart_Printf("\nStylus Down, please...... \n");
	//Uart_Getch();

	//rINTSUBMSK|=BIT_SUB_TC;
	//rINTMSK|=BIT_ADC;
	//Uart_Printf("Touch Screen Test is Finished!!!\n");

}


void TouchpanelInt(void)

{
	//int i;
	//U32 saveAdcdly;
 	//static TouchPos TouchXY;

    if(rADCDAT0&0x8000)
    {
		//Uart_Printf("\nStylus Up!!\n");
		rADCTSC&=0xff;	// Set stylus down interrupt bit
		GUI_TOUCH_StoreState(-1,-1);
    }
   
    else {
		//Uart_Printf("\nStylus Down!!\n");

	rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
	//saveAdcdly=rADCDLY;
	rADCDLY=40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms

	rADCCON|=0x1;                   //start ADC

		while(rADCCON & 0x1);		//check if Enable_start is low
		while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!!
		
            while(!(rSRCPND & (BIT_ADC)));  //check if ADC is finished with interrupt bit

            //xdata=(rADCDAT0&0x3ff);
            //ydata=(rADCDAT1&0x3ff);
			//GUI_TOUCH_Exec();
	 //YH 0627, To check Stylus Up Interrupt.
	 rSUBSRCPND|=BIT_SUB_TC;
	 ClearPending(BIT_ADC);
	 //rINTSUBMSK &= ~(BIT_SUB_TC);
	 //rINTMSK &= ~(BIT_ADC);
			 
	 rADCTSC =0xd3;    //Waiting for interrupt
	 rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal.

			while(1)		//to check Pen-up state
			{
			 if(rSUBSRCPND & (BIT_SUB_TC))	
				 {
					//Uart_Printf("Stylus Up Interrupt~!\n");
					break;	//if Stylus is up(1) state
				}
			}	
			//xdata = ydata =0;
			//GUI_TOUCH_Exec();
			
			
	//TouchXY.TouchX = xdata;
    //TouchXY.TouchY = ydata;
	//OSMboxPost(MboxTouchpanel,(void *)&TouchXY);// 给触摸屏任务发送消息
    //Uart_Printf("XP=%04d,YP=%04d\n", xdata, ydata);    //X-position Conversion data            
	
	 }
	//rADCDLY=saveAdcdly; 
	rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal.
    rSUBSRCPND|=BIT_SUB_TC;
    //rINTSUBMSK &= ~(BIT_SUB_TC);	// Unmask sub interrupt (TC)     
    ClearPending(BIT_ADC);
}


