/*
 * timer.h
 *
 *  Created on: Oct 27, 2026
 *      Author: hochu
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TIMER_CYCLE 10;

extern int timer1_flag;
extern int timer2_flag;

void setTimer1(int duration);
void timerRun(void);



#endif /* INC_TIMER_H_ */
