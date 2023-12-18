/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32
#define UPTICKS 1

extern void draw_select(int number);
extern void Reset(void);
extern int status;

volatile int SoundMealSnack=0;
volatile int SoundGameOver=0;
volatile int SoundCuddles=0;

NOTE select[]={
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma}
};

NOTE mealsnack[]={
	{b2, time_semicroma},
	{a3, time_semicroma},
	{d4, time_semicroma},
	{b4b, time_semicroma},
	{c4, time_semicroma},
	{c5, time_semicroma},
};

NOTE GameOver[]={
	{g3, time_croma*2},
	{pause, time_biscroma},
	{f3, time_croma*2},
	{pause, time_biscroma},
	{e3, time_croma*2},
	{pause, time_biscroma},
	{d3, time_croma*2},
	{pause, time_biscroma},
	{c3, time_minima},
};

NOTE Cuddles[]={
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
};

NOTE song[] = 
{
	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 5
};

void RIT_IRQHandler (void){
	static int currentNote = 0;
	static int ticks = 0;
	static int currentNote1 = 0;
	static int ticks1 = 0;
	static int currentNote2 = 0;
	static int ticks2 = 0;
	static int currentNote3 = 0;
	static int ticks3 = 0;
	static int selected=0;
	static int SoundSelect=0;
	ADC_start_conversion();
	if(SoundSelect==1){
		if(isNotePlaying()!=1)
		{
			++ticks;
			if(ticks == UPTICKS)
			{
				ticks = 0;
				playNote(select[currentNote++]);
			}
		}
		if(currentNote == (sizeof(select) / sizeof(select[0])))
		{
			currentNote=0;
			SoundSelect=0;
		}
	}
	if(SoundMealSnack==1){
		if(isNotePlaying()!=1)
		{
			++ticks1;
			if(ticks1 == UPTICKS)
			{
				ticks1 = 0;
				playNote(mealsnack[currentNote1++]);
			}
		}
		if(currentNote1 == (sizeof(mealsnack) / sizeof(mealsnack[0])))
		{
			currentNote1=0;
			SoundMealSnack=0;
		}
	}
	if(SoundGameOver==1){
		if(isNotePlaying()!=1)
		{
			++ticks2;
			if(ticks2 == UPTICKS)
			{
				ticks2 = 0;
				playNote(GameOver[currentNote2++]);
			}
		}
		if(currentNote2 == (sizeof(GameOver) / sizeof(GameOver[0])))
		{
			currentNote2=0;
			SoundGameOver=0;
		}
	}
	if(SoundCuddles==1){
		if(isNotePlaying()!=1)
		{
			++ticks3;
			if(ticks3 == UPTICKS)
			{
				ticks3 = 0;
				playNote(Cuddles[currentNote3++]);
			}
		}
		if(currentNote3 == (sizeof(Cuddles) / sizeof(Cuddles[0])))
		{
			currentNote3=0;
			SoundCuddles=0;
		}
	}
	if(status==0 || status==1){
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
			draw_select(0);
			selected=0;
		}
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			draw_select(1);
			selected=1;
		}
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
			if(selected==1){
				status=2;
				SoundSelect=1;
			}
			if(selected==0){
				status=4;
				SoundSelect=1;
			}
		}
	}
	else if(status==5){
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
			SoundSelect=1;
			Reset();
		}
	}
	
	disable_RIT();
	reset_RIT();
	enable_RIT();
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
