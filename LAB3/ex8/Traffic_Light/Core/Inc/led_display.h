/*
 * led_display.h
 *
 *  Created on: Oct 27, 2026
 *      Author: hochu
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_
#include "main.h"
extern int led_buffer[4];

void display_mode_and_leds(void);
void scan_7seg(void);


#endif /* INC_LED_DISPLAY_H_ */
