/*
 * led_display.c
 *
 *  Created on: Oct 27, 2026
 *      Author: hochu
 */
#include "main.h"
#include "timer.h"
#include "input_processing.h"
#include "led_display.h"

int led_buffer[4] = {0, 0, 0, 1};
int led_index = 0;

void display7SEG(int num) {
    char seg_code[10] = {0xC0,
    		             0xF9,
						 0xA4,
						 0xB0,
						 0x99,
						 0x92,
						 0x82,
						 0xF8,
						 0x80,
						 0x90};
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(GPIOB, A_Pin, (seg_code[num] >> 0) & 0x01);
        HAL_GPIO_WritePin(GPIOB, B_Pin, (seg_code[num] >> 1) & 0x01);
        HAL_GPIO_WritePin(GPIOB, C_Pin, (seg_code[num] >> 2) & 0x01);
        HAL_GPIO_WritePin(GPIOB, D_Pin, (seg_code[num] >> 3) & 0x01);
        HAL_GPIO_WritePin(GPIOB, E_Pin, (seg_code[num] >> 4) & 0x01);
        HAL_GPIO_WritePin(GPIOB, F_Pin, (seg_code[num] >> 5) & 0x01);
        HAL_GPIO_WritePin(GPIOB, G_Pin, (seg_code[num] >> 6) & 0x01);
    }
}

void scan_7seg(void) {
    if (timer2_flag == 1) {
        setTimer2(5);

        HAL_GPIO_WritePin(GPIOB, DIGIT_1_Pin | DIGIT_2_Pin | DIGIT_3_Pin | DIGIT_4_Pin, GPIO_PIN_RESET);
        display7SEG(led_buffer[led_index]);

        if (led_index == 0) HAL_GPIO_WritePin(GPIOB, DIGIT_1_Pin, GPIO_PIN_SET);
        else if (led_index == 1) HAL_GPIO_WritePin(GPIOB, DIGIT_2_Pin, GPIO_PIN_SET);
        else if (led_index == 2) HAL_GPIO_WritePin(GPIOB, DIGIT_3_Pin, GPIO_PIN_SET);
        else if (led_index == 3) HAL_GPIO_WritePin(GPIOB, DIGIT_4_Pin, GPIO_PIN_SET);

        led_index++;
        if (led_index >= 4) led_index = 0;
    }
}

void clear_all_leds(void) {
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin |
    		                 LED_YELLOW1_Pin |
						     LED_GREEN1_Pin |
                             LED_RED2_Pin |
							 LED_YELLOW2_Pin |
							 LED_GREEN2_Pin,
							 GPIO_PIN_RESET);
}

void display_mode_and_leds(void) {
    if (mode == 1) {
        // Mode 1: Chế độ bình thường
        led_buffer[0] = 0;
        led_buffer[1] = 0;
        led_buffer[2] = 0;
        led_buffer[3] = mode;
    } else if (mode >= 2 && mode <= 4) {
        // Mode 2, 3, 4: Cập nhật temp_value lên 2 LED đầu
        led_buffer[0] = temp_value / 10;
        led_buffer[1] = temp_value % 10;
        led_buffer[2] = 0;
        led_buffer[3] = mode;
    }

    // Chớp tắt các đèn LED tương ứng với Mode ở tần số 2Hz (chu kỳ 500ms)
    if (timer1_flag == 1) {
        setTimer1(250);
        if (mode == 2) {
            HAL_GPIO_TogglePin(GPIOA, LED_RED1_Pin | LED_RED2_Pin);
            HAL_GPIO_WritePin(GPIOA, LED_YELLOW1_Pin | LED_YELLOW2_Pin | LED_GREEN1_Pin | LED_GREEN2_Pin, GPIO_PIN_RESET);
        } else if (mode == 3) {
            HAL_GPIO_TogglePin(GPIOA, LED_YELLOW1_Pin | LED_YELLOW2_Pin);
            HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_RED2_Pin | LED_GREEN1_Pin | LED_GREEN2_Pin, GPIO_PIN_RESET);
        } else if (mode == 4) {
            HAL_GPIO_TogglePin(GPIOA, LED_GREEN1_Pin | LED_GREEN2_Pin);
            HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_RED2_Pin | LED_YELLOW1_Pin | LED_YELLOW2_Pin, GPIO_PIN_RESET);
        }
    }
}

