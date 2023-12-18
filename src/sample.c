/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "led/led.h"
#include "adc/adc.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

volatile int status=0;
extern int Animation;

void DrawRectangle(int x,int y, uint16_t color){
	LCD_DrawLine(x,y,x+5,y,color);
	LCD_DrawLine(x,y+1,x+5,y+1,color);
	LCD_DrawLine(x,y+2,x+5,y+2,color);
	LCD_DrawLine(x,y+3,x+5,y+3,color);
	LCD_DrawLine(x,y+4,x+5,y+4,color);
}

void DrawRectangleSmall(int x,int y, uint16_t color){
	LCD_DrawLine(x,y,x+3,y,color);
	LCD_DrawLine(x,y+1,x+3,y+1,color);
	LCD_DrawLine(x,y+2,x+3,y+2,color);
}



int blue[8]={145,165,150,165,145,170,150,170};
int black[72]={120,150,125,150,130,150,135,150,140,150,145,150,
									110,155,115,155,140,155,150,155,
									105,160,135,160,155,160,
									95,165,100,165,105,165,110,165,135,165,160,165,
									90,170,115,170,135,170,160,170,
									90,175,120,175,140,175,160,175,
									90,180,120,180,145,180,150,180,155,180,160,180,165,180};
int black2[54]={95,185,115,185,140,185,170,185,
								100,190,105,190,110,190,135,190,145,190,150,190,155,190,160,190,165,190,
								110,195,140,195,165,195,
								115,200,120,200,145,200,150,200,155,200,160,200,165,200,
								125,205,130,205,135,205,140,205};
int yellow[20]={145,185,150,185,155,185,160,185,165,185,
								140,190,
								145,195,150,195,155,195,160,195};
int yellow2[16]={130,170,125,175,130,180,125,185,
								95,170,100,175,95,180,100,185};
int red[56]={120,155,125,155,130,155,135,155,
							110,160,115,160,120,160,125,160,130,160,
							115,165,120,165,125,165,130,165,
							100,170,105,170,110,170,120,170,125,170,
							95,175,105,175,110,175,115,175,
							100,180,105,180,110,180,115,180,
							105,185,110,185};
int red2[42]={130,175,135,175,
							125,180,135,180,140,180,
							120,185,130,185,135,185,
							115,190,120,190,125,190,130,190,
							115,195,120,195,125,195,130,195,135,195,
							125,200,130,200,135,200,140,200};

int white[22]={145,155,
							140,160,145,160,150,160,
							140,165,155,165,
							140,170,155,170,
							145,175,150,175,155,175};
int aniBlack[40]={145,205,150,205,155,205,160,205,165,205};
int aniYellow[40]={145,200,150,200,155,200,160,200};
int heart[54]={20,35,23,35,29,35,32,35,
								17,38,20,38,23,38,26,38,29,38,32,38,35,38,
								17,41,20,41,23,41,26,41,29,41,32,41,35,41,
								20,44,23,44,26,44,29,44,32,44,
								23,47,26,47,29,47,
								26,50};
int cherryb[6]={150,35,150,38,150,41};
int cherryg[10]={153,32,156,32,159,32,153,35,156,35};
int cherryr[34]={144,41,147,41,153,41,156,41,
									144,44,147,44,150,44,153,44,156,44,
									144,47,147,47,150,47,153,47,156,47,
									147,50,150,50,153,50};
int snackr[36]={210,190,213,190,216,190,222,190,225,190,228,190,
								210,193,213,193,216,193,222,193,225,193,228,193,
								210,196,213,196,216,196,222,196,225,196,228,196};
int snackg[20]={213,187,213,184,216,184,216,181,219,181,219,178,222,181,222,184,225,184,225,187};
int mealr[72]={213,187,216,187,222,187,225,187,
								210,190,213,190,216,190,219,190,222,190,225,190,228,190,
								210,193,213,193,216,193,219,193,222,193,225,193,228,193,
								210,196,213,196,216,196,219,196,222,196,225,196,228,196,
								210,199,213,199,216,199,219,199,222,199,225,199,228,199,
								213,202,216,202,222,202,225,202};
int mealg[8]={219,187,219,184,219,181,222,181};
int vol[44]={29,6,
						26,9,29,9,
						23,12,26,12,29,12,
						17,15,20,15,23,15,26,15,29,15,
						17,18,20,18,23,18,26,18,29,18,
						23,21,26,21,29,21,
						26,24,29,24,
						29,27,	
};
int eyes[30]={145,155,
							140,160,145,160,150,160,
							140,165,145,165,150,165,155,165,
							140,170,145,170,150,170,155,170,
							145,175,150,175,155,175};

void DrawHappiness(int number){
	int i=0;
	if(number==1){
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i],heart[i+1],Red);
		}
	}
	if(number==2){
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i],heart[i+1],Red);
		}
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i]+30,heart[i+1],Red);
		}
	}
	if(number==3){
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i],heart[i+1],Red);
		}
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i]+30,heart[i+1],Red);
		}
		for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i]+60,heart[i+1],Red);
		}
	}
}




void DrawSatiety(int number){
	int i=0;
	if(number==1){
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i],cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i],cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i],cherryr[i+1],Red);
		}
	}
	if(number==2){
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i],cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i],cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i],cherryr[i+1],Red);
		}
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i]+30,cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i]+30,cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i]+30,cherryr[i+1],Red);
		}
	}
	if(number==3){
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i],cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i],cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i],cherryr[i+1],Red);
		}
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i]+30,cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i]+30,cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i]+30,cherryr[i+1],Red);
		}
		for(i=0;i<6;i=i+2){
			DrawRectangleSmall(cherryb[i]+60,cherryb[i+1],Brown);
		}
		for(i=0;i<10;i=i+2){
			DrawRectangleSmall(cherryg[i]+60,cherryg[i+1],Green);
		}
		for(i=0;i<34;i=i+2){
			DrawRectangleSmall(cherryr[i]+60,cherryr[i+1],Red);
		}
	}
}
							
void DrawBird(int offset,int offset2){
	int i=0;
	DrawRectangle(blue[i]-offset,blue[i+1]-offset2,Black);
	DrawRectangle(blue[i+2]-offset,blue[i+3]-offset2,Black);
	DrawRectangle(blue[i+4]-offset,blue[i+5]-offset2,Black);
	DrawRectangle(blue[i+6]-offset,blue[i+7]-offset2,Cyan);
	for(i=0;i<72;i=i+2){
		DrawRectangle(black[i]-offset,black[i+1]-offset2,Black);
		if(i<56){
			DrawRectangle(red[i]-offset,red[i+1]-offset2,Red);
			if(i<42){
				DrawRectangle(red2[i]-offset,red2[i+1]-offset2,Red);
				if(i<22){
					DrawRectangle(white[i]-offset,white[i+1]-offset2,White);
					if(i<20){
						DrawRectangle(yellow[i]-offset,yellow[i+1]-offset2,DarkYellow);
						if(i<16){
							DrawRectangle(yellow2[i]-offset,yellow2[i+1]-offset2,Yellow);
						
						}
					}
				}
			}
			if(i<54){
				
				DrawRectangle(black2[i]-offset,black2[i+1]-offset2,Black);
			}
		}
	}
	return;
}

void ClearBird(int offset,int offset2){
//	int i=0;
//	for(i=130;i<250;i++){
//		LCD_DrawLine(0,i,205,i,White);
//	}
	int i=0;
	for(i=0;i<72;i=i+2){
		DrawRectangle(black[i]-offset,black[i+1]-offset2,White);
	}
	for(i=0;i<54;i=i+2){
		DrawRectangle(black2[i]-offset,black2[i+1]-offset2,White);
	}
	for(i=0;i<54;i=i+2){
		DrawRectangle(black2[i]-offset,black2[i+1]-offset2,White);
	}
	i=0;
	DrawRectangle(blue[i]-offset,blue[i+1]-offset2,White);
	DrawRectangle(blue[i+2]-offset,blue[i+3]-offset2,White);
	DrawRectangle(blue[i+4]-offset,blue[i+5]-offset2,White);
	DrawRectangle(blue[i+6]-offset,blue[i+7]-offset2,White);
	for(i=0;i<20;i=i+2){
		DrawRectangle(yellow[i]-offset,yellow[i+1]-offset2,White);
	}
	for(i=0;i<56;i=i+2){
		DrawRectangle(red[i]-offset,red[i+1]-offset2,White);
	}
	for(i=0;i<16;i=i+2){
		DrawRectangle(yellow2[i]-offset,yellow2[i+1]-offset2,White);
	}
	for(i=0;i<42;i=i+2){
		DrawRectangle(red2[i]-offset,red2[i+1]-offset2,White);
	}
	for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]-offset,aniBlack[i+1]-offset2,White);
			DrawRectangle(aniBlack[i]-offset,aniBlack[i+1]-(offset2+10),White);
			DrawRectangle(aniBlack[i]-offset,aniBlack[i+1]-(offset2+15),White);
			DrawRectangle(aniBlack[i]-offset,aniBlack[i+1]-(offset2+5),White);
		}
	for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]-offset,aniYellow[i+1]-(offset2),White);
			DrawRectangle(aniYellow[i]-offset,aniYellow[i+1]-(offset2+5),White);
		}
	return;
}


void ClearHeart(int number){
	int i=0;
	if(number==1){
		for(i=34;i<53;i++)
			LCD_DrawLine(76,i,99,i,White);
	}
	
	if(number==2){
		for(i=34;i<53;i++)
			LCD_DrawLine(45,i,99,i,White);
	}
	
	if(number==3){
		for(i=34;i<53;i++)
			LCD_DrawLine(15,i,99,i,White);
	}
	
}

void ClearCherry(int number){
	int i=0;
	if(number==1){
		for(i=31;i<53;i++)
			LCD_DrawLine(200,i,222,i,White);
	}
	
	if(number==2){
		for(i=31;i<53;i++)
			LCD_DrawLine(170,i,222,i,White);
	}
	
	if(number==3){
		for(i=31;i<53;i++)
			LCD_DrawLine(140,i,222,i,White);
	}
}

void draw_select(int number){
	volatile int i=0;
	if(number==0){
		for(i=50;i<80;i++){
			DrawRectangleSmall(i,305,Black);
			DrawRectangleSmall(115+i,305,White);
		}
	}
	if(number==1){
		for(i=165;i<195;i++){
			DrawRectangleSmall(i,305,Black);
			DrawRectangleSmall(i-115,305,White);
		}
	}
	if(number==2){
		for(i=95;i<135;i++){
			DrawRectangleSmall(i,305,Black);
		}
	}
	return;
}

void DrawSnack(){
	int i=0;
	for(i=0;i<36;i=i+2){
		DrawRectangleSmall(snackr[i],snackr[i+1],Red);
	}
	for(i=0;i<20;i=i+2){
		DrawRectangleSmall(snackg[i],snackg[i+1],Green);
	}
}
void ClearSnack(){
	int i=0;
	for(i=170;i<220;i++){
		LCD_DrawLine(208,i,235,i,White);
	}
}
void DrawMeal(){
	int i=0;
	for(i=0;i<72;i=i+2){
		DrawRectangleSmall(mealr[i],mealr[i+1],Red);
	}
	for(i=0;i<8;i=i+2){
		DrawRectangleSmall(mealg[i],mealg[i+1],Green);
	}
}
void ClearMenu(){
	int i=0;
	for(i=290;i<320;i++){
		LCD_DrawLine(50,i,200,i,White);
	}
}

void Reset(){
	LCD_Clear(White);
	DrawBird(10,10);
	DrawHappiness(3);
	DrawSatiety(3);
	GUI_Text(50, 290, (uint8_t *) "MEAL", Black, White);
	GUI_Text(160, 290, (uint8_t *) "SNACK", Black, White);
	draw_select(0);
	init_timer(0,0,0,3,0x17D7840); 						  /* 1s */
	enable_timer(0);
	status=0;
	Animation=0;
}

void AnimationOut(){
	int i=0,j=0;
	for(i=-5;i>-160;i=i-15){
		DrawBird(i,10);
		for(j=-5;j<10;j=j+5){
			DrawRectangle(110+j-i,140,White);
			DrawRectangle(100+j-i,145,White);
			DrawRectangle(95+j-i,150,White);
			DrawRectangle(85+j-i,155,White);
			DrawRectangle(80+j-i,160,White);
			DrawRectangle(80+j-i,165,White);
			DrawRectangle(80+j-i,170,White);
			DrawRectangle(85+j-i,175,White);
			DrawRectangle(90+j-i,180,White);
			DrawRectangle(100+j-i,185,White);
			DrawRectangle(105+j-i,190,White);
			DrawRectangle(115+j-i,195,White);
		}
	}
}


void DrawHeartCuddles(){
	int i=0;
	for(i=0;i<54;i=i+2){
		if(i<30){
			DrawRectangle(eyes[i]-10,eyes[i+1]-10,Red);
		}
			DrawRectangleSmall(heart[i]+50,heart[i+1]+100,Red);
			DrawRectangleSmall(heart[i]+140,heart[i+1]+100,Red);
		}
	
}

void DrawHeartCuddles2(){
	int i=0;
	for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i]+95,heart[i+1]+70,Red);
	}
}

void openEyes(){
	int i=0;
	for(i=20;i>=0;i=i-2){
			DrawRectangle(white[i]-10,white[i+1]-10,White);
	}
	i=0;
	DrawRectangle(blue[i]-10,blue[i+1]-10,Black);
	DrawRectangle(blue[i+2]-10,blue[i+3]-10,Black);
	DrawRectangle(blue[i+4]-10,blue[i+5]-10,Black);
	DrawRectangle(blue[i+6]-10,blue[i+7]-10,Cyan);
}

void ClearHeartCuddles(){
	int i=0;
	for(i=0;i<54;i=i+2){
			DrawRectangleSmall(heart[i]+50,heart[i+1]+100,White);
			DrawRectangleSmall(heart[i]+140,heart[i+1]+100,White);
			DrawRectangleSmall(heart[i]+95,heart[i+1]+70,White);
		}
}

extern int volume;
void DrawVolume(offset){
	int i=0;
	for(i=0;i<44;i=i+2){
		DrawRectangleSmall(vol[i]-offset,vol[i+1],Black);
	}
	if(volume==0){
		DrawRectangleSmall(35-10,15,White);
		DrawRectangleSmall(35-10,18,White);
		DrawRectangleSmall(41-10,12,White);
		DrawRectangleSmall(41-10,15,White);
		DrawRectangleSmall(41-10,18,White);
		DrawRectangleSmall(41-10,21,White);
		DrawRectangleSmall(47-10,9,White);
		DrawRectangleSmall(47-10,12,White);
		DrawRectangleSmall(47-10,15,White);
		DrawRectangleSmall(47-10,18,White);
		DrawRectangleSmall(47-10,21,White);
		DrawRectangleSmall(47-10,24,White);
	}
	else if(volume<=2){
		DrawRectangleSmall(35-10,15,Black);
		DrawRectangleSmall(35-10,18,Black);
		DrawRectangleSmall(41-10,12,White);
		DrawRectangleSmall(41-10,15,White);
		DrawRectangleSmall(41-10,18,White);
		DrawRectangleSmall(41-10,21,White);
		DrawRectangleSmall(47-10,9,White);
		DrawRectangleSmall(47-10,12,White);
		DrawRectangleSmall(47-10,15,White);
		DrawRectangleSmall(47-10,18,White);
		DrawRectangleSmall(47-10,21,White);
		DrawRectangleSmall(47-10,24,White);
	}
	else if(volume<=4){
		DrawRectangleSmall(35-10,15,Black);
		DrawRectangleSmall(35-10,18,Black);
		DrawRectangleSmall(41-10,12,Black);
		DrawRectangleSmall(41-10,15,Black);
		DrawRectangleSmall(41-10,18,Black);
		DrawRectangleSmall(41-10,21,Black);
		DrawRectangleSmall(47-10,9,White);
		DrawRectangleSmall(47-10,12,White);
		DrawRectangleSmall(47-10,15,White);
		DrawRectangleSmall(47-10,18,White);
		DrawRectangleSmall(47-10,21,White);
		DrawRectangleSmall(47-10,24,White);
	}
	else{
		DrawRectangleSmall(35-10,15,Black);
		DrawRectangleSmall(35-10,18,Black);
		DrawRectangleSmall(41-10,12,Black);
		DrawRectangleSmall(41-10,15,Black);
		DrawRectangleSmall(41-10,18,Black);
		DrawRectangleSmall(41-10,21,Black);
		DrawRectangleSmall(47-10,9,Black);
		DrawRectangleSmall(47-10,12,Black);
		DrawRectangleSmall(47-10,15,Black);
		DrawRectangleSmall(47-10,18,Black);
		DrawRectangleSmall(47-10,21,Black);
		DrawRectangleSmall(47-10,24,Black);
	}
}
int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	joystick_init();											/* Joystick Initialization            */
	LCD_Initialization();
	TP_Init();
	TouchPanel_Calibrate();
	
	
	LCD_Clear(White);
	DrawBird(10,10);
	DrawHappiness(3);
	DrawSatiety(3);
	DrawVolume(10);
	GUI_Text(50, 290, (uint8_t *) "MEAL", Black, White);
	GUI_Text(160, 290, (uint8_t *) "SNACK", Black, White);
	draw_select(0);

	
	init_timer(0,0,0,3,0x17D7840); 						  /* 1s */
	enable_timer(0);
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/	
	init_timer(2,0,0,3,0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	enable_timer(2);
	ADC_init();
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
  while (1)	
  {
		//__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
