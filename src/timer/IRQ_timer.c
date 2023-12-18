/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../TouchPanel/TouchPanel.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern void DrawRectangle(int x,int y, uint16_t color);
extern int aniBlack[40];
extern int aniYellow[40];
extern void ClearHeart(int number);
extern void ClearCherry(int number);
extern void DrawSatiety(int number);
extern void DrawHappiness(int number);
extern int status;
extern void draw_select(int number);
extern void ClearMenu(void);
extern void DrawBird(int offset,int offset2);
extern void ClearBird(int offset,int offset2);
extern void ClearSnack(void);
extern void AnimationOut(void);
extern void DrawHeartCuddles(void);
extern void ClearHeartCuddles(void);
extern void DrawHeartCuddles2(void);
extern void openEyes(void);
extern void DrawSnack(void);
extern void DrawMeal(void);

volatile int heart1=3;
volatile int cherry=3;
volatile int Animation=0;

extern int volume;
extern int SoundMealSnack;
extern int SoundGameOver;
extern int SoundCuddles;

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	static int seconds=0;
	static int minutes=0;
	static int hours=0;
	char time[50]="";
	
	/* Age */
	seconds++;
	if(seconds==60){
		seconds=0;
		minutes++;
	}
	if(minutes==60){
		minutes=0;
		hours++;
	}
	sprintf(time, "Age: %02d:%02d:%02d",hours,minutes,seconds);
	GUI_Text(65, 10, (uint8_t *) time, Black, White);
	
	if(seconds%5==0){
		heart1--;
		cherry--;
		ClearHeart(3-heart1);
		ClearCherry(3-cherry);
		if(heart1==0 || cherry==0){
			SoundGameOver=1;
			GUI_Text(80, 70, (uint8_t *) "GAME OVER", Black, White);
			if(status!=0 && status!=1){
				if(status==2 || status==4){
					ClearSnack();
				}
				else{
					ClearSnack();
					ClearBird(-40,10);
					DrawBird(10,10);
				}
				if(Animation!=0){
					ClearBird(10,0);
					DrawBird(10,10);
					ClearHeartCuddles();
				}
			}
			ClearHeartCuddles();
			openEyes();
			status=5;
			disable_timer(0);
			ClearMenu();
			seconds=0;
			minutes=0;
			hours=0;
			heart1=3;
			cherry=3;
			AnimationOut();
			GUI_Text(95, 290, (uint8_t *) "RESET", Black, White);
			draw_select(2);
			
			LPC_TIM0->IR = 1;			/* clear interrupt flag */
			return;
		}
	}
	/* Animation */
	if(status==0){
		int i=0;
		for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-10,Black);
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-20,Black);
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-25,White);
		}
		for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]-10,aniYellow[i+1]-10,DarkYellow);
		}
		status=1;
	}
	else if(status==1){
		int i=0;
		for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-10,White);
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-25,Black);
			DrawRectangle(aniBlack[i]-10,aniBlack[i+1]-15,Black);
		}
		for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]-10,aniYellow[i+1]-15,DarkYellow);
		}
		status=0;
	}
	/* Animation Eat */
	else if((status==2 || status==4) && Animation==0){
		int i=0;
		if(status==2){
			DrawSnack();
			status=7;
		}
		if(status==4){
			DrawMeal();
			status=8;
		}
		ClearBird(10,10);
		DrawBird(-40,10);
		for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-10,Black);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-20,Black);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-25,White);
		}
		for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]+40,aniYellow[i+1]-10,DarkYellow);
		}
		if(status==7){
			heart1++;
			DrawHappiness(heart1);
		}
		if(status==8){
			cherry++;
			DrawSatiety(cherry);
		}
		status=3;
	}
	else if(status==3){
		int i=0;
		for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-10,White);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-25,Black);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-15,Black);
		}
		for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]+40,aniYellow[i+1]-15,DarkYellow);
		}
		SoundMealSnack=1;
		status=15;
	}
	else if(status==15){
		int i=0;
		for(i=0;i<10;i=i+2){
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-10,Black);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-20,Black);
			DrawRectangle(aniBlack[i]+40,aniBlack[i+1]-25,White);
		}
		for(i=0;i<8;i=i+2){
			DrawRectangle(aniYellow[i]+40,aniYellow[i+1]-10,DarkYellow);
		}
		ClearSnack();
		ClearBird(-40,10);
		DrawBird(10,10);
		status=0;
	}
	/* Animation touch */
	if(Animation==1){
		openEyes();
		DrawHeartCuddles2();
		Animation=2;
	}
	else if(Animation==2){
		ClearBird(10,10);
		DrawBird(10,0);
		Animation=3;
	}
	else if(Animation==3){
		ClearBird(10,0);
		DrawBird(10,10);
		ClearHeartCuddles();
		heart1++;
		Animation=0;
		DrawHappiness(heart1);
	}
	
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
	
void TIMER1_IRQHandler (void)
{
	static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks]*volume/7;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
	
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}



void TIMER2_IRQHandler (void){
  if((status==0 || status==1) && Animation==0){
		if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
			if(display.x>60 && display.y>130 && display.x<180 &&  display.y<220){
				SoundCuddles=1;
				DrawHeartCuddles();
				Animation=1;
			}
		}
	}
	
	LPC_TIM2->IR = 1;
	return;
}

void TIMER3_IRQHandler (void){
	
	disable_timer(1);
	
	LPC_TIM3->IR = 1;
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
