//====================================================================
// File Name : Adc.c
// Function  : S3C2410 ADC Test 
// Program   : Kang, Weon Tark 
// Date      : May 22, 2002
// Version   : 0.0
// History
//   0.0 : Programming start (March 29,2002) -> KWT
//         ADC Test                          -> May 15, 2002 SOP
//====================================================================

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

#define REQCNT 100              //May 08, 2002 SOP
#define ADC_FREQ 2500000
//#define ADC_FREQ   1250000
//#define LOOP 1
#define LOOP 10000

volatile U32 preScaler;

//==================================================================================		
int ReadAdc(int ch)
{
    int i;
    static int prevCh=-1;

    rADCCON = (1<<14)|(preScaler<<6)|(ch<<3);	//setup channel

    if(prevCh!=ch)
    {
	rADCCON = (1<<14)|(preScaler<<6)|(ch<<3);   //setup channel
	for(i=0;i<LOOP;i++);	//delay to set up the next channel
	prevCh=ch;
    }
    rADCCON|=0x1;   //start ADC

    while(rADCCON & 0x1);	//check if Enable_start is low
    while(!(rADCCON & 0x8000));	//check if EC(End of Conversion) flag is high

    return ( (int)rADCDAT0 & 0x3ff );
}

//==================================================================================
void Test_Adc(void) 
{
    int a0=0; //Initialize variables
    U32 rADCCON_save = rADCCON;
    
   	Uart_Printf( "ADC INPUT Test, press ESC key to exit !\n" ) ;

    preScaler = ADC_FREQ;
    Uart_Printf("ADC conv. freq. = %dHz\n",preScaler);
    preScaler = 50000000/ADC_FREQ -1;               //PCLK:50.7MHz
    
    Uart_Printf("PCLK/ADC_FREQ - 1 = %d\n",preScaler);
    
    while( Uart_GetKey() != ESC_KEY )
    {
	    a0=ReadAdc(0);

	    Uart_Printf( "AIN0: %04d\n", a0 );
		Delay( 500 ) ;
    }
    
    //rADCCON=(0<<14)|(19<<6)|(7<<3)|(1<<2);  //stand by mode to reduce power consumption
    rADCCON = rADCCON_save;
    Uart_Printf("\nrADCCON = 0x%x\n", rADCCON);
}
