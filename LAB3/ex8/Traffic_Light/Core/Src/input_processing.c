/*
 * input_processing.c
 *
 *  Created on: Oct 27, 2026
 *      Author: hochu
 */

#include "main.h"
#include "input_reading.h"
#include "input_processing.h"

int mode = 1;
int red_time = 5;
int yellow_time = 2;
int temp_value = 1;

enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND };
enum ButtonState buttonState = BUTTON_RELEASED;

enum Button2State { BTN2_RELEASED, BTN2_PRESSED };
static enum Button2State btn2State = BTN2_RELEASED;

enum Button3State { BTN3_RELEASED, BTN3_PRESSED };
static enum Button3State btn3State = BTN3_RELEASED;

void fsm_for_button_2(void) {
    switch (btn2State) {
        case BTN2_RELEASED:
            if (is_button_pressed(1)) {
                btn2State = BTN2_PRESSED;
                if (mode == 2 || mode == 3) {
                    temp_value++;
                    if (temp_value > 99) temp_value = 1;
                }
            }
            break;

        case BTN2_PRESSED:
            if (!is_button_pressed(1)) {
                btn2State = BTN2_RELEASED;
            }
            break;
    }
}

void fsm_for_button_3(void) {
    switch (btn3State) {
        case BTN3_RELEASED:
            if (is_button_pressed(2)) {
                btn3State = BTN3_PRESSED;
                if (mode == 2) {
                    red_time = temp_value;    
                } else if (mode == 3) {
                    yellow_time = temp_value; 
                }
            }
            break;

        case BTN3_PRESSED:
            if (!is_button_pressed(2)) {
                btn3State = BTN3_RELEASED;
            }
            break;
    }
}


void fsm_for_input_processing(void) {
    switch (buttonState) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                buttonState = BUTTON_PRESSED;
                mode++;
                if (mode > 4) mode = 1;

                if (mode == 2) {
                    temp_value = red_time;
                } else if (mode == 3) {
                    temp_value = yellow_time;
                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(0)) {
                    buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            }
            break;
    }

    fsm_for_button_2();
    fsm_for_button_3();
}


