/*
 * timer.c
 *
 *  Created on: Oct 27, 2026
 *      Author: hochu
 */

#include<timer.c>

int timer1_counter = 0;
int timer1_flag = 0;

void setTimer1(int duration){
	timer1_counter = duration / TIMER_CYCLE;
}

void timeRun(void){
	if (timer1_counter > 0){
		timer1_counter--;
		if (timer1_counter <= 0){
			timer1_flag = 1;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		button_reading();
		timerRun();

	}
}



