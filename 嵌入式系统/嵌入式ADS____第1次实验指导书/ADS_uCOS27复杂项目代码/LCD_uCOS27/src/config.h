/****************************************Copyright (c)**************************************************

**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define uint8    INT8U                    /* Unsigned  8 bit quantity                           */
#define int8     INT8S                    /* Signed    8 bit quantity                           */
#define uint16   INT16U                   /* Unsigned 16 bit quantity                           */
#define int16    INT16S                   /* Signed   16 bit quantity                           */
#define uint32   INT32U                   /* Unsigned 32 bit quantity                           */
#define int32    INT32S                   /* Signed   32 bit quantity                           */


/********************************/
/*      uC/OS-II的特殊代码      */
/********************************/

#include "Includes.h"


/********************************/
/*      ARM的特殊代码           */
/********************************/

#include    "..\arm\LPC2124.h"


/********************************/
/*     应用程序配置             */
/********************************/

#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>


/********************************/
/*     本例子的配置             */
/********************************/
/* 系统设置 */
#define Fosc            11059200                    //晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍


#include    "target.h"

// LPC21000 misc uart0 definitions
#define UART0_PCB_PINSEL_CFG     (INT32U)0x00000005
#define UART0_INT_BIT            (INT32U)0x0040
#define LCR_DISABLE_LATCH_ACCESS (INT32U)0x00000000
#define LCR_ENABLE_LATCH_ACCESS  (INT32U)0x00000080
#define LCR_DISABLE_BREAK_TRANS  (INT32U)0x00000000
#define LCR_ODD_PARITY           (INT32U)0x00000000
#define LCR_ENABLE_PARITY        (INT32U)0x00000008
#define LCR_1_STOP_BIT           (INT32U)0x00000000
#define LCR_CHAR_LENGTH_8        (INT32U)0x00000003 
#define LSR_THR_EMPTY            (INT32U)0x00000020

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
