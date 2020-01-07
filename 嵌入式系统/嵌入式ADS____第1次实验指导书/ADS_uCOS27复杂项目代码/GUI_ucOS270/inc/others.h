#ifndef __OTHER_H__
#define __OTHER_H__

typedef struct
  {
	int TouchX;           
 	int TouchY;			

 }TouchPos, *PtTouchPos;
 
typedef struct
  {
	U16 TouchX1;           
 	U16 TouchY1;
	U16 TouchX2;           
	U16 TouchY2;           

 }MenuTouchPos, *PtMenuTouchPos;


extern MenuTouchPos iTouchPos[];

extern S8 match_menu(PtTouchPos pt_pos);
extern void printf_float(float value);
//extern void LCD_dis_num(int x, int y, float value);
//extern float GetValue(U8 num);

volatile extern  float value;

volatile extern float temperature;
volatile extern float kt_temperature;

volatile extern float weight;
volatile extern float weightst;


volatile extern float u_wave_length1; //¸ß¶È
volatile extern float u_wave_length2;
volatile extern float u_wave_length3;

volatile extern float cur_speed_rear;
volatile extern float hightest_speed;
volatile extern float hightest_speed_pingtan ;
volatile extern float hightest_speed_ningling ;
volatile extern float hightest_speed_kanke ;

volatile extern float height  ;
//volatile extern float height_pingtan ;
//volatile extern float height_ningling ;
//volatile extern float height_kanke ;
//extern U8 c_input ;
//extern U8 cpp;
#endif