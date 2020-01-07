/**************************************************************
The initial and control for 320×240 16Bpp TFT LCD----LCD_LTV350QV-F04
**************************************************************/
#ifndef __LCDD_H__
#define __LCDD_H__


#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 240320
#define LCD_XSIZE_TFT_240320 	(240)	
#define LCD_YSIZE_TFT_240320 	(320)

#define SCR_XSIZE_TFT_240320 	(240)
#define SCR_YSIZE_TFT_240320 	(320)


//#define  SCR_XSIZE_TFT_240320 	(LCD_XSIZE_TFT_240320*2)
//#define  SCR_YSIZE_TFT_240320 	(LCD_YSIZE_TFT_240320*2)

//TFT240320
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)

//Timing parameter for NEC3.5"
#define VBPD_240320		(1)		//垂直同步信号的后肩
#define VFPD_240320		(5)		//垂直同步信号的前肩
#define VSPW_240320		(1)		//垂直同步信号的脉宽

#define HBPD_240320		(36)		//水平同步信号的后肩
#define HFPD_240320		(19)		//水平同步信号的前肩
#define HSPW_240320		(5)		//水平同步信号的脉宽

#define CLKVAL_TFT_240320	(4) 	
//FCLK=180MHz,HCLK=90MHz,VCLK=6.5MHz

// GPB1/TOUT1 for Backlight control(PWM)
#define GPB1_TO_OUT()   (rGPBUP &= 0xfffd, rGPBCON &= 0xfffffff3, rGPBCON |= 0x00000004)
#define GPB1_TO_1()     (rGPBDAT |= 0x0002)
#define GPB1_TO_0()     (rGPBDAT &= 0xfffd)

extern void Uart_Printf(char *f, ...) ;
extern unsigned char flower1_240_320[];

static void Lcd_PowerEnable(int invpwren,int pwren);

//volatile static unsigned short LCD_BUFFER[SCR_YSIZE_TFT_240320][SCR_XSIZE_TFT_240320];
#define  LCD_BUFFER   0x32000000
#endif