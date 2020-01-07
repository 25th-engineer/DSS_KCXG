/**************************************************************
4*4 Key Scan
**************************************************************/

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

/******************************************************************************
	4X4 矩阵键盘
四个输入引脚：	EINT0 -----( GPF0  )----INPUT
				EINT2 -----( GPF2  )----INPUT
				EINT11-----( GPG3  )----INPUT
				EINT19-----( GPG11 )----INPUT
				
四个输出引脚：			   ( GPB6 )----OUTPUT
				           ( GPB7 )----OUTPUT
				
******************************************************************************/
U8 Key_Scan( void )
{
	Delay( 80 ) ;
	//rGPGDAT = rGPGDAT & (~((1<<6)|(1<<2))) | (1<<6) | (0<<2) ;		//GPG6,2 output 0
//	rGPBDAT &=~(1<<6);
//	rGPBDAT |=1<<7;
	//Delay(10);
	//rGPEDAT = rGPEDAT & (~((1<<13)|(1<<11))) | (1<<13) | (1<<11) ;		//GPE13,11 output 0
	if(      (rGPFDAT&(1<< 0)) == 0 )	
		return 1 ;
	else if( (rGPFDAT&(1<< 2)) == 0 )
		return 2;
	else if( (rGPGDAT&(1<< 3)) == 0 )
		return 3 ;
	else if( (rGPGDAT&(1<<11)) == 0 )
		return 4 ;
	else
		return 0xff;

	//rGPGDAT = rGPGDAT & (~((1<<6)|(1<<2))) | (0<<6) | (1<<2) ;		//GPG6,2 output 0
	//rGPEDAT = rGPEDAT & (~((1<<13)|(1<<11))) | (1<<13) | (1<<11) ;		//GPE13,11 output 0
//	rGPBDAT &=~(1<<7);
//	rGPBDAT |=1<<6;
//	if(      (rGPFDAT&(1<< 0)) == 0 )		return 2 ;
//	else if( (rGPFDAT&(1<< 2)) == 0 )		return 4 ;
//	else if( (rGPGDAT&(1<< 3)) == 0 )		return 6 ;
//	else if( (rGPGDAT&(1<<11)) == 0 )		return 8 ;
//	else return 0xff ;
	
}

static void __irq Key_ISR(void)
{
	U8 key;
	U32 r;

	EnterCritical(&r);
	if(rINTPND==BIT_EINT8_23) {
		ClearPending(BIT_EINT8_23);
		if(rEINTPEND&(1<<11)) {
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 11;
		}
		if(rEINTPEND&(1<<19)) {
		//	Uart_Printf("eint19\n");		
			rEINTPEND |= 1<< 19;
		}
	}
	if(rINTPND==BIT_EINT0) {
		//Uart_Printf("eint0\n");
		ClearPending(BIT_EINT0);
	}
	if(rINTPND==BIT_EINT2) {
		//Uart_Printf("eint2\n");
		ClearPending(BIT_EINT2);
	}
	key=Key_Scan();
	if( key == 0xff )
		Uart_Printf( "Interrupt occur... Key is released!\n") ;
	else
		Uart_Printf( "Interrupt occur... K%d is pressed!\n", key) ;

//	rGPBCON &=~((3<<12)|(3<<14));
//	rGPBCON |=((1<<12)|(1<<14));
//	rGPBUP &=~(3<<6);
//	rGPBDAT &=~(3<<6);
	ExitCritical(&r);
}

void KeyScan_Test(void)
{
	Uart_Printf("\nKey Scan Test, press ESC key to exit !\n");	

	//rGPGCON &= ~((3<<12)|(3<<4));
	//rGPGCON |= (1<<12)|(1<<4);		//GPG6,2 output 0
	//rGPGDAT &= ~((1<<6)|(1<<2));
	
	//rGPECON &= ~((3<<26)|(3<<22));
	//rGPECON |= (1<<26)|(1<<22);		//GPE13,11 output 0
	//rGPEDAT &= ~((1<<13)|(1<<11));
	
	rGPGCON = rGPGCON & (~((3<<22)|(3<<6))) | ((2<<22)|(2<<6)) ;		//GPG11,3 set EINT
	rGPFCON = rGPFCON & (~((3<<4)|(3<<0))) | ((2<<4)|(2<<0)) ;		//GPF2,0 set EINT
	
	rEXTINT0 &= ~(7|(7<<8));	
	rEXTINT0 |= (0|(0<<8));	//set eint0,2 falling edge int
	rEXTINT1 &= ~(7<<12);
	rEXTINT1 |= (0<<12);	//set eint11 falling edge int
	rEXTINT2 &= ~(0xf<<12);
	rEXTINT2 |= (0<<12);	//set eint19 falling edge int
	
//	rGPBCON &=~((3<<12)|(3<<14));
//	rGPBCON |=((1<<12)|(1<<14));
//	rGPBUP |=(3<<6);
//	rGPBDAT &=~(3<<6);

	rEINTPEND |= (1<<11)|(1<<19);		//clear eint 11,19
	rEINTMASK &= ~((1<<11)|(1<<19));	//enable eint11,19
	ClearPending(BIT_EINT0|BIT_EINT2|BIT_EINT8_23);
	pISR_EINT0 = pISR_EINT2 = pISR_EINT8_23 = (U32)Key_ISR;
	EnableIrq(BIT_EINT0|BIT_EINT2|BIT_EINT8_23);	

	//rGPGCON &= ~(3<<20);
	
	//rGPGUP  &= ~(1<<10);
	 while( Uart_GetKey() != ESC_KEY ) ;
	 DisableIrq(BIT_EINT0|BIT_EINT2|BIT_EINT8_23);	
}
