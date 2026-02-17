/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// --- ROWS (Outputs) ---
// Wired to A0, A1, A2, A3
#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_0  // A0
#define R2_PORT GPIOA
#define R2_PIN GPIO_PIN_1  // A1
#define R3_PORT GPIOA
#define R3_PIN GPIO_PIN_4  // A2
#define R4_PORT GPIOB
#define R4_PIN GPIO_PIN_0  // A3

// --- COLUMNS (Inputs) ---
// Wired to A4, A5, D7, D8
#define C1_PORT GPIOC
#define C1_PIN GPIO_PIN_1  // A4
#define C2_PORT GPIOC
#define C2_PIN GPIO_PIN_0  // A5
#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_8  // D7
#define C4_PORT GPIOA
#define C4_PIN GPIO_PIN_9  // D8

char read_keypad(void)
{
    // 1. Reset all Rows to LOW
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);

    // --- SCAN ROW 1 ---
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return '1';
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return '2';
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return '3';
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return 'A';
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);

    // --- SCAN ROW 2 ---
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return '4';
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return '5';
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return '6';
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return 'B';
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);

    // --- SCAN ROW 3 ---
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return '7';
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return '8';
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return '9';
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return 'C';
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);

    // --- SCAN ROW 4 ---
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return '*';
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return '0';
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return '#';
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return 'D';
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);

    return 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  // --- 1. Initialize LCD ---
  HD44780_Init(2);
  HD44780_Clear();
  HD44780_Backlight();

  // --- 2. Intro Message ---
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("Ready to Type...");
  HAL_Delay(2000); // Show message for 2 seconds
  HD44780_Clear(); // Clear screen for input

  // --- 3. variables ---
  char key;
  char msg[30];
  char lcd_str[2]; // Buffer to hold one char + null terminator

  // Track the cursor position
  int lcd_col = 0;
  int lcd_row = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    key = read_keypad();

    if (key != 0)
    {
        // --- Option A: Debug to PC ---
        sprintf(msg, "Key: %c\r\n", key);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);


        // --- Option B: Display on LCD ---

        // CASE 1: Next Row Button (#)
        if (key == '#')
        {
            lcd_row++;           // Move to next row
            if (lcd_row > 1) {   // If we go past row 1...
                lcd_row = 0;     // ...loop back to top row
            }
            lcd_col = 0;         // Reset to start of line
            HD44780_SetCursor(lcd_col, lcd_row);
        }

        // CASE 2: Clear Screen Button (*)
        else if (key == '*')
        {
            HD44780_Clear();
            lcd_col = 0;
            lcd_row = 0;
        }

        // CASE 3: Normal Character
        else
        {
            // Only print if we haven't reached the end of the line (No Overflow)
            if (lcd_col < 16)
            {
                lcd_str[0] = key;  // Put key in string
                lcd_str[1] = '\0'; // Null terminate
                HD44780_PrintStr(lcd_str);

                lcd_col++; // Move our cursor tracker
            }
        }

        HAL_Delay(200); // Debounce delay
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
