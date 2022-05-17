/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @author         : JZimnol
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "chess.h"
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

/* USER CODE BEGIN PV */
volatile uint16_t ADC_results[8];
volatile uint8_t ADC_counter = 0;
volatile int buttonDebounce = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  GameInit();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  MX_DMA_Init(); /* NOTE: this function needs to be executed before MX_ADC1_Init(),
                          otherwise ADC DMA won't work properly and no ADC values
                          will be read
                 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  /* start SPI2 timer */
  HAL_TIM_Base_Start_IT(&htim10);

  /* simple welcome sequence */
  for( uint8_t i=0; i<8; i++ ) {
    for( uint8_t j=0; j<8; j++ ) {
      Chessboard[i][j] = P1_bishop;
      Chessboard[7-i][7-j] = P1_bishop;
      HAL_Delay(100);
      Chessboard[7-i][7-j] = emptyField;
      Chessboard[i][j] = emptyField;
    }
  }
  HAL_Delay(1000);

  for( uint8_t i=0; i<8; i++ ) {
    for( uint8_t j=0; j<8; j++ ) {
      Chessboard[i][j] = emptyField;
    }
  }

  /* start ADC timer */
  HAL_TIM_Base_Start_IT(&htim11);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /*
     * EMPTY!!!!!
     */
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/* USER CODE BEGIN 4 */

/**-----------------------------------------------------------------------------
 * @brief     TIM10 & TIM11 ISR
 * @param[in] instance of a timer
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /* interrupt every 695 us */
    if( htim->Instance == TIM10 ) {
        buttonDebounce++;
        if( Game.mode == Normal ) {
            uint8_t dataToSend = 0;
            if( Game.SPI2_x%2 == 0 ) {
                SPI2_Buffer[Game.SPI2_x][Game.SPI2_y] =
                        Chessboard[Game.SPI2_x][Game.SPI2_y] != emptyField ? WHITE_LIGHT : NO_LIGHT;
                SPI2_Buffer[Game.SPI2_x][Game.SPI2_y+1] =
                        Chessboard[Game.SPI2_x][Game.SPI2_y+1] != emptyField ? WHITE_LIGHT : NO_LIGHT;

                dataToSend = ~((SPI2_Buffer[Game.SPI2_x][Game.SPI2_y]<<4) |
                                SPI2_Buffer[Game.SPI2_x][Game.SPI2_y+1]);
            }
            else {
                SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y] =
                        Chessboard[Game.SPI2_x][7-Game.SPI2_y] != emptyField ? WHITE_LIGHT : NO_LIGHT;
                SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y-1] =
                        Chessboard[Game.SPI2_x][7-Game.SPI2_y-1] != emptyField ? WHITE_LIGHT : NO_LIGHT;

                dataToSend = ~((SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y]<<4) |
                                SPI2_Buffer[Game.SPI2_x][7-Game.SPI2_y-1]);
            }

            /* blocking transmission, 7.11 us */
            HAL_SPI_Transmit(&hspi2, &dataToSend, 1, 100000);

            Game.SPI2_y += 2;
            if( Game.SPI2_y == 8 ) {
                Game.SPI2_y = 0;
                Game.SPI2_x++;
                if( Game.SPI2_x == 8 ) {
                    HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, GPIO_PIN_RESET);
                    Game.SPI2_x = 0;
                }
            }
        }

    }
    /* interrupt every 5.55 ms */
    else if( htim->Instance == TIM11 ) {
        ADC_counter = ADC_counter > 7 ? 0 : ADC_counter;
        SetMuxOutputs(ADC_counter);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_results, 8);
    }
}

/**-----------------------------------------------------------------------------
 * @brief     ADC conversion complete
 * @param[in] instance of an ADC
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    for( uint8_t i=0; i<7; i++) {
        if     ( IsBetween(P1_PAWN_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_pawn;
        else if( IsBetween(P1_ROOK_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_rook;
        else if( IsBetween(P1_KNIGHT_VALUE, ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_knight;
        else if( IsBetween(P1_BISHOP_VALUE, ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_bishop;
        else if( IsBetween(P1_QUEEN_VALUE,  ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_queen;
        else if( IsBetween(P1_KING_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P1_king;
        else if( IsBetween(P2_PAWN_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_pawn;
        else if( IsBetween(P2_ROOK_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_rook;
        else if( IsBetween(P2_KNIGHT_VALUE, ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_knight;
        else if( IsBetween(P2_BISHOP_VALUE, ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_bishop;
        else if( IsBetween(P2_QUEEN_VALUE,  ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_queen;
        else if( IsBetween(P2_KING_VALUE,   ADC_MARGIN, ADC_results[i]) ) Chessboard[i][ADC_counter] = P2_king;
        else   Chessboard[i][ADC_counter] = emptyField;
    }
    ADC_counter++;
}

/**-----------------------------------------------------------------------------
 * @brief     GPIO ISR
 * @param[in] GPIO pin number
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    Game.P1.hasPieces = true;
    Game.P2.hasPieces = true;

    if( GPIO_Pin == P1_changeModePlayer_Pin ) {
        /* falling edge, reset buttonDebounce counter and do nothing */
        if( HAL_GPIO_ReadPin(P1_changeModePlayer_GPIO_Port, P1_changeModePlayer_Pin ) == 0 ) {
            buttonDebounce = 0;
            return;
        }
        /* rising edge */
        else {
            /* short-time press, less than 1 second */
            if( buttonDebounce < 1440 ) {
                if( Game.turn == P1 && Game.mode == Interactive ) {
                    ClearSPI2Buffer();
                    Game.turn = P2;
                    Game.P2.currentField[0] = 0;
                    Game.P2.currentField[1] = 8;
                    FindNextPiece();
                }
            }
            /* long-time press, longer than 1 second */
            else if( buttonDebounce > 280 ) {
                if( Game.mode == Normal ) {
                    ClearSPI2Buffer();
                    Game.mode = Interactive;
                    Game.turn = P1;
                    Game.P1.currentField[0] = 0;
                    Game.P1.currentField[1] = 255;
                    FindNextPiece();
                }
                else if( Game.turn == P1 ) {
                    Game.mode = Normal;
                    Game.SPI2_x = 0;
                    Game.SPI2_y = 0;
                }
            }
            buttonDebounce = 0;
            return;
        }
    }

    if( GPIO_Pin == P2_changeModePlayer_Pin ) {
        /* falling edge, reset buttonDebounce counter and do nothing */
        if( HAL_GPIO_ReadPin(P2_changeModePlayer_GPIO_Port, P2_changeModePlayer_Pin ) == 0 ) {
            buttonDebounce = 0;
            return;
        }
        /* rising edge */
        else {
            /* short-time press, less than 1 second */
            if( buttonDebounce < 1440 ) {
                if( Game.turn == P2 && Game.mode == Interactive ) {
                    ClearSPI2Buffer();
                    Game.turn = P1;
                    Game.P1.currentField[0] = 0;
                    Game.P1.currentField[1] = 255;
                    FindNextPiece();
                }
            }
            /* long-time press, longer than 1 second */
            else if( buttonDebounce > 280 ) {
                if( Game.mode == Normal ) {
                    ClearSPI2Buffer();
                    Game.mode = Interactive;
                    Game.turn = P2;
                    Game.P2.currentField[0] = 0;
                    Game.P2.currentField[1] = 8;
                    FindNextPiece();
                }
                else if( Game.turn == P2 ) {
                    Game.mode = Normal;
                    Game.SPI2_x = 0;
                    Game.SPI2_y = 0;
                }
            }
            buttonDebounce = 0;
            return;
        }
    }

    /* other buttons debounceTimer check */
    if( buttonDebounce < 280 ) {
        return;
    }
    else {
        buttonDebounce = 0;
    }


    if( GPIO_Pin == P1_forward_Pin ) {
        if( Game.mode == Interactive && Game.turn == P1 ) {
            ClearSPI2Buffer();
            FindNextPiece();
        }
    }
    else if( GPIO_Pin == P2_forward_Pin ) {
        if( Game.mode == Interactive && Game.turn == P2 ) {
            ClearSPI2Buffer();
            FindNextPiece();
        }
    }
    else if( GPIO_Pin == P1_backward_Pin ) {
        if( Game.mode == Interactive && Game.turn == P1 ) {
            ClearSPI2Buffer();
            FindPrevPiece();
        }
    }
    else if( GPIO_Pin == P2_backward_Pin ) {
        if( Game.mode == Interactive && Game.turn == P2 ) {
            ClearSPI2Buffer();
            FindPrevPiece();
        }
    }
}

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

#ifdef  USE_FULL_ASSERT
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
