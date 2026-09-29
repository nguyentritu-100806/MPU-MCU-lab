/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Exercise 8 - Digital Clock
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* TIM2 interrupt every 5 ms */
#define TIMER_CYCLE 5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

int hour = 15;
int minute = 8;
int second = 50;

int led_buffer[4] = {0, 0, 0, 0};
int index_led = 0;

/* Software timer 1: clock */
volatile int timer_counter = 0;
volatile int timer_flag = 0;

/* Software timer 2: 7SEG scanning */
volatile int timer2_counter = 0;
volatile int timer2_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

void display7SEG(int num);

void updateClockBuffer(void);
void update7SEG(void);

void setTimer(int duration);
void setTimer2(int duration);
void timerRun(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* =========================================================
 * SOFTWARE TIMER 1
 * Used for updating second every 1000 ms
 * ========================================================= */
void setTimer(int duration)
{
    timer_counter = duration / TIMER_CYCLE;
    timer_flag = 0;
}


/* =========================================================
 * SOFTWARE TIMER 2
 * Used for scanning 7SEG every 5 ms
 * ========================================================= */
void setTimer2(int duration)
{
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}


/* =========================================================
 * SOFTWARE TIMER RUN
 * Called every 5 ms by TIM2 interrupt
 * ========================================================= */
void timerRun(void)
{
    /* Timer 1 */
    if (timer_counter > 0)
    {
        timer_counter--;

        if (timer_counter == 0)
        {
            timer_flag = 1;
        }
    }


    /* Timer 2 */
    if (timer2_counter > 0)
    {
        timer2_counter--;

        if (timer2_counter == 0)
        {
            timer2_flag = 1;
        }
    }
}


/* =========================================================
 * UPDATE CLOCK BUFFER
 *
 * Example:
 * hour   = 15
 * minute = 8
 *
 * led_buffer = {1, 5, 0, 8}
 * ========================================================= */
void updateClockBuffer(void)
{
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;

    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}


/* =========================================================
 * UPDATE 7 SEGMENT
 *
 * Exercise 8:
 * This function is called from MAIN, NOT interrupt.
 * ========================================================= */
void update7SEG(void)
{
    /* Turn all 4 digits OFF first */
    HAL_GPIO_WritePin(GPIOA, EN0_Pin, OFF);
    HAL_GPIO_WritePin(GPIOA, EN1_Pin, OFF);
    HAL_GPIO_WritePin(GPIOA, EN2_Pin, OFF);
    HAL_GPIO_WritePin(GPIOA, EN3_Pin, OFF);


    switch (index_led)
    {
        case 0:
            display7SEG(led_buffer[0]);

            HAL_GPIO_WritePin(GPIOA, EN0_Pin, ON);
            break;


        case 1:
            display7SEG(led_buffer[1]);

            HAL_GPIO_WritePin(GPIOA, EN1_Pin, ON);
            break;


        case 2:
            display7SEG(led_buffer[2]);

            HAL_GPIO_WritePin(GPIOA, EN2_Pin, ON);
            break;


        case 3:
            display7SEG(led_buffer[3]);

            HAL_GPIO_WritePin(GPIOA, EN3_Pin, ON);
            break;
    }


    index_led++;


    if (index_led >= 4)
    {
        index_led = 0;
    }
}


/* =========================================================
 * DISPLAY ONE NUMBER ON 7SEG
 * Common Anode
 * ========================================================= */
void display7SEG(int num)
{
    if (num < 0 || num > 9)
    {
        return;
    }


    switch (num)
    {
        case 0:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_SET);

            break;


        case 1:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_SET);

            break;


        case 2:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 3:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 4:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 5:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 6:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 7:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_SET);

            break;


        case 8:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;


        case 9:

            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET);

            break;
    }
}

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_TIM2_Init();


    /* USER CODE BEGIN 2 */


    /*
     * Initial display:
     *
     * hour   = 15
     * minute = 08
     *
     * -> 15:08
     */
    updateClockBuffer();


    /*
     * Software timer 1:
     * 1000 ms for clock
     */
    setTimer(1000);


    /*
     * Software timer 2:
     * 5 ms for 7SEG scanning
     */
    setTimer2(5);


    /*
     * Start TIM2 interrupt
     */
    HAL_TIM_Base_Start_IT(&htim2);


    /* USER CODE END 2 */


    /* Infinite loop */

    /* USER CODE BEGIN WHILE */

    while (1)
    {

        /* =====================================================
         * SOFTWARE TIMER 1
         *
         * Every 1000 ms:
         * update second/minute/hour
         * blink DOT
         * ===================================================== */

        if (timer_flag == 1)
        {

            timer_flag = 0;


            second++;


            if (second >= 60)
            {
                second = 0;

                minute++;
            }


            if (minute >= 60)
            {
                minute = 0;

                hour++;
            }


            if (hour >= 24)
            {
                hour = 0;
            }


            /*
             * Update 4 digits
             */
            updateClockBuffer();


            /*
             * Exercise 7:
             * DOT processing is moved to MAIN
             */
            HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);


            /*
             * Restart 1-second software timer
             */
            setTimer(1000);
        }



        /* =====================================================
         * SOFTWARE TIMER 2
         *
         * Every 5 ms:
         * update one 7SEG digit
         * ===================================================== */

        if (timer2_flag == 1)
        {

            timer2_flag = 0;


            /*
             * Exercise 8:
             * update7SEG is now in MAIN
             */
            update7SEG();


            /*
             * Restart 7SEG software timer
             */
            setTimer2(5);
        }
    }

    /* USER CODE END WHILE */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    /* HSI = 8 MHz */

    RCC_OscInitStruct.OscillatorType =
            RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
            RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
            RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
            RCC_PLL_NONE;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }


    RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource =
            RCC_SYSCLKSOURCE_HSI;


    RCC_ClkInitStruct.AHBCLKDivider =
            RCC_SYSCLK_DIV1;


    RCC_ClkInitStruct.APB1CLKDivider =
            RCC_HCLK_DIV1;


    RCC_ClkInitStruct.APB2CLKDivider =
            RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief TIM2 Initialization Function
  * @retval None
  */
static void MX_TIM2_Init(void)
{

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};

    TIM_MasterConfigTypeDef sMasterConfig = {0};


    htim2.Instance = TIM2;


    /*
     * TIM2 clock = 8 MHz
     *
     * PSC = 7999
     *
     * 8 MHz / 8000
     * = 1000 Hz
     *
     * 1 count = 1 ms
     *
     *
     * ARR = 4
     *
     * count:
     *
     * 0
     * 1
     * 2
     * 3
     * 4
     *
     * = 5 counts
     *
     * interrupt every 5 ms
     */


    htim2.Init.Prescaler = 7999;


    htim2.Init.CounterMode =
            TIM_COUNTERMODE_UP;


    htim2.Init.Period = 4;


    htim2.Init.ClockDivision =
            TIM_CLOCKDIVISION_DIV1;


    htim2.Init.AutoReloadPreload =
            TIM_AUTORELOAD_PRELOAD_DISABLE;


    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }


    sClockSourceConfig.ClockSource =
            TIM_CLOCKSOURCE_INTERNAL;


    if (HAL_TIM_ConfigClockSource(
            &htim2,
            &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }


    sMasterConfig.MasterOutputTrigger =
            TIM_TRGO_RESET;


    sMasterConfig.MasterSlaveMode =
            TIM_MASTERSLAVEMODE_DISABLE;


    if (HAL_TIMEx_MasterConfigSynchronization(
            &htim2,
            &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* GPIO Ports Clock Enable */

    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();


    /* Initial output GPIOA */

    HAL_GPIO_WritePin(
            GPIOA,
            DOT_Pin
            | LED_RED_Pin
            | EN0_Pin
            | EN1_Pin
            | EN2_Pin
            | EN3_Pin,
            GPIO_PIN_RESET);


    /* Initial output GPIOB */

    HAL_GPIO_WritePin(
            GPIOB,
            SEG0_Pin
            | SEG1_Pin
            | SEG2_Pin
            | SEG3_Pin
            | SEG4_Pin
            | SEG5_Pin
            | SEG6_Pin,
            GPIO_PIN_RESET);


    /* GPIOA outputs */

    GPIO_InitStruct.Pin =
            DOT_Pin
            | LED_RED_Pin
            | EN0_Pin
            | EN1_Pin
            | EN2_Pin
            | EN3_Pin;


    GPIO_InitStruct.Mode =
            GPIO_MODE_OUTPUT_PP;


    GPIO_InitStruct.Pull =
            GPIO_NOPULL;


    GPIO_InitStruct.Speed =
            GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    /* GPIOB outputs */

    GPIO_InitStruct.Pin =
            SEG0_Pin
            | SEG1_Pin
            | SEG2_Pin
            | SEG3_Pin
            | SEG4_Pin
            | SEG5_Pin
            | SEG6_Pin;


    GPIO_InitStruct.Mode =
            GPIO_MODE_OUTPUT_PP;


    GPIO_InitStruct.Pull =
            GPIO_NOPULL;


    GPIO_InitStruct.Speed =
            GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/* USER CODE BEGIN 4 */


/* =========================================================
 * TIMER INTERRUPT
 *
 * Exercise 8 requirement:
 *
 * Interrupt ONLY handles software timers.
 *
 * NO:
 * display7SEG()
 * GPIO processing
 * update7SEG()
 * clock update
 *
 * inside ISR.
 * ========================================================= */

void HAL_TIM_PeriodElapsedCallback(
        TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)
    {

        timerRun();

    }
}


/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

    __disable_irq();


    while (1)
    {

    }
}


#ifdef USE_FULL_ASSERT

void assert_failed(
        uint8_t *file,
        uint32_t line)
{

}

#endif
