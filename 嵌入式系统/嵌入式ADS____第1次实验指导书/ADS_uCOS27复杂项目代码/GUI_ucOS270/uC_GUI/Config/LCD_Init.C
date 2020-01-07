/*
*********************************************************************************************************
* 文件: LCD_Init.C
* 描述: LCD 控制器服务文件.
* 编写: 深思 (001-12345@sohu.com).
*********************************************************************************************************
*/
#define   Global_Def 
#include  "Includes.H"
/*
// LCD control 1 register
#define  LC1_CLKVAL      (2)            // VCLK = HCLK / [(CLKVAL+1) * 2] typ: 25.175 MHz.
#define  LC1_MMODE       (0)            // Determine the toggle rate of the VM.
#define  LC1_PNRMODE     (3)            // 11 = TFT LCD panel
#define  LC1_BPPMODE     (12)           // 1100 = 16 bpp for TFT

// LCD control 2 register
#define  LC2_VBPD        (33-1)         // TFT: Vertical back porch is the number of inactive lines at the start
                                        // of a frame, after vertical synchronization period.
#define  LC2_LINEVAL     (LCD_YSIZE-1)  // TFT/STN: These bits determine the vertical size of LCD panel.
#define  LC2_VFPD        (10-1)         // TFT: Vertical front porch is the number of inactive lines at the end
                                        // of a frame, before vertical synchronization period.
#define  LC2_VSPW        (2-1)          // TFT: Vertical sync pulse width determines the VSYNC pulse's high
                                        // level width by counting the number of inactive lines.

// LCD control 3 register
#define  LC3_HBPD        (48-1)         // TFT: Horizontal back porch is the number of VCLK periods between
                                        // the falling edge of HSYNC and the start of active data.
#define  LC3_HOZVAL      (LCD_XSIZE-1)  // TFT/STN: These bits determine the horizontal size of LCD panel.
#define  LC3_HFPD        (10-1)         // TFT: Horizontal front porch is the number of VCLK periods between
                                        // the end of active data and the rising edge of HSYNC.

// LCD control 4 register
#define  LC4_MVAL        (0)
#define  LC4_HSPW        (96-1)         // TFT: Horizontal sync pulse width determines the HSYNC pulse's
                                        // high level width by counting the number of the VCLK.

// LCD control 5 register
#define  LC5_BPP24BL     (0)            // TFT: This bit determines the order of 24 bpp video memory.
                                        // 0 = LSB valid 1 = MSB Valid
#define  LC5_FRM565      (1)            // 0 = 5:5:5:1 Format 1 = 5:6:5 Format
#define  LC5_INVVCLK     (0)            // 0 = The video data is fetched at VCLK falling edge
                                        // 1 = The video data is fetched at VCLK rising edge
#define  LC5_INVVLINE    (1)            // This bit indicates the VLINE/HSYNC pulse polarity.
#define  LC5_INVVFRAME   (1)            // 0 = Normal 1 = Inverted
#define  LC5_INVVD       (0)            // 0 = Normal 1 = VD is inverted.
#define  LC5_INVVDEN     (0)            // TFT: This bit indicates the VDEN signal polarity.
#define  LC5_INVPWREN    (0)            // 0 = Normal 1 = Inverted
#define  LC5_INVLEND     (0)            // TFT: This bit indicates the LEND signal polarity.
#define  LC5_PWREN       (0)            // LCD_PWREN output signal enable/disable.
#define  LC5_ENLEND      (0)            // LEND output signal enable/disable.
#define  LC5_BSWP        (0)            // Byte swap control bit.
#define  LC5_HWSWP       (1)            // Half-Word swap control bit.

// Frame buffer start address 1 register
#define  LCDBANK         ((INT32U)LCD_Buffer>>22)
#define  LCDBASEU        (((INT32U)LCD_Buffer>>1)&0x1fffff)

// Frame buffer start address 2 register
#define  LCDBASEL        (LCDBASEU+((PAGEWIDTH+OFFSIZE)*(LC2_LINEVAL+1)))

// Virtual screen address set
#define  OFFSIZE         (0)
#define  PAGEWIDTH       (LCD_XSIZE*16/16)

// 开/关 LCD 及背光.
#define  LCD_ON()        (rLCDCON1|= 1);(rGPGDAT|= (1<<4))
#define  LCD_OFF()       (rLCDCON1&=~1);(rGPGDAT&=~(1<<4))
*/
/*
********************************************************************************************************
* 函数: void s3c2410_LCD_Init (void).
* 描述: 初始化 LCD 控制器.
********************************************************************************************************
*/


void s3c2410_LCD_Init (void)
{

/*
 IsInitialized = 0;//MemDev
 _IsInited = 0;//Win
 WM__CreateFlags = 0;
 GUI_CURSOR_pfTempHide = NULL;
 GUI_Context.hDevData = 0;
   WM__hCapture=0;
 WM__hWinFocus=0;
   WM_pfPollPID = 0;
 WM_pfHandlePID = 0;
   GUI_pfTimerExec = 0;
   _KeyMsgCnt = 0;

    /*LCD_OFF();
    rLCDCON1 = ((LC1_CLKVAL<<8)+(LC1_MMODE<<7)+(LC1_PNRMODE<<5)+(LC1_BPPMODE<<1)+(0<<0));
    rLCDCON2 = ((LC2_VBPD<<24)+(LC2_LINEVAL<<14)+(LC2_VFPD<<6)+(LC2_VSPW<<0));
    rLCDCON3 = ((LC3_HBPD<<19)+(LC3_HOZVAL<<8)+(LC3_HFPD<<0));
    rLCDCON4 = ((LC4_MVAL<<8)+(LC4_HSPW<<0));

    rLCDCON5 =  (LC5_BPP24BL<<12)+(LC5_FRM565<<11)+ \
                (LC5_INVVCLK<<10)+(LC5_INVVLINE<<9)+(LC5_INVVFRAME<<8)+(LC5_INVVD<<7)+ \
                (LC5_INVVDEN<<6)+(LC5_INVPWREN<<5)+(LC5_INVLEND<<4)+(LC5_PWREN<<3)+ \
                (LC5_ENLEND<<2)+(LC5_BSWP<<1)+(LC5_HWSWP<<0);

    rLCDSADDR1 = ((LCDBANK<<21)+(LCDBASEU<<0));
    rLCDSADDR2 = (LCDBASEL<<0);
    rLCDSADDR3 = ((OFFSIZE<<11)+(PAGEWIDTH<<0));

    rLCDINTMSK|= (3);
    rLPCSEL    = 0;
    rTPAL      = 0;

    LCD_ON();
   */
}

/*
********************************************************************************************************
* End.
********************************************************************************************************
*/
