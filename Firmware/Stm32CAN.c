/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

union Package1 Dados1 = {0};
union Package2 Dados2 = {0};
union Package3 Dados3 = {0};

union Package1 Dados1_rec = {0};
union Package2 Dados2_rec = {0};
union Package3 Dados3_rec = {0};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define IRMS_MAX (200.0f) // (+/-)
#define VRMS_MAX (400.0f) // (+/-)
#define PQ_MAX (70000.0f) // (+/-)

#define GANHOI (327.0f)  // floor(65535/(IRMS_MAX))
#define GANHOV (163.0f)  // floor(65535/(VRMS_MAX))
#define GANHOPQ (0.468f)  // (65535/(2*PQ_MAX))  // Arredondado para 3 casas decimais
#define OFFSETPQ (32767)


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

float P_Ativa=12345.0f, P_Reativa=-14790.0f;
float Va_RMS=50.0f, Vb_RMS=270.0f, Vc_RMS=350.0f;
float Ia_RMS=52.0f, Ib_RMS=105.0f, Ic_RMS=189.0f;
long int cont_loop_sender=0;

float P_Ativa_rec=0.0f, P_Reativa_rec=0.0f;
float Va_RMS_rec=0.0f, Vb_RMS_rec=0.0f, Vc_RMS_rec=0.0f;
float Ia_RMS_rec=0.0f, Ib_RMS_rec=0.0f, Ic_RMS_rec=0.0f;
long int cont_loop_receiver=0;
long int cont_loop_receiver_CAN=0;

FDCAN_TxHeaderTypeDef   TxHeader;
FDCAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];

uint32_t RxDataLength = 0;
uint8_t i=0;



void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
	  cont_loop_receiver_CAN++;
	  if(cont_loop_receiver_CAN>10000000)cont_loop_receiver_CAN=0;

    /* Retreive Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    	Error_Handler(); /* Reception Error */

    }else{

    	RxDataLength = RxHeader.DataLength;

    	if(RxHeader.Identifier == 0x10){

    		for (i=0; i < RxDataLength ; i++){
    			Dados1_rec.all[i] = RxData[i];
    		}
			P_Ativa_rec   = (Dados1_rec.Potencia.Ativa   - OFFSETPQ) / GANHOPQ;
			P_Reativa_rec = (Dados1_rec.Potencia.Reativa - OFFSETPQ) / GANHOPQ;

    	}else if(RxHeader.Identifier == 0x11){

    		for (i=0; i < RxDataLength ; i++){
    			Dados2_rec.all[i] = RxData[i];
    		}
    		Ia_RMS_rec = (Dados2_rec.Correntes.Ia) / GANHOI;
    		Ib_RMS_rec = (Dados2_rec.Correntes.Ib) / GANHOI;
    		Ic_RMS_rec = (Dados2_rec.Correntes.Ic) / GANHOI;

    	}else if(RxHeader.Identifier == 0x12){

    		for (i=0; i < RxDataLength ; i++){
    			Dados3_rec.all[i] = RxData[i];
    		}
    		Va_RMS_rec = (Dados3_rec.Tensoes.Va) / GANHOV;
    		Vb_RMS_rec = (Dados3_rec.Tensoes.Vb) / GANHOV;
    		Vc_RMS_rec = (Dados3_rec.Tensoes.Vc) / GANHOV;
    	}
    }

    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
      Error_Handler();  /* Notification Error */
    }

  }
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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */

  if(HAL_FDCAN_Start(&hfdcan1)!= HAL_OK){
   	  Error_Handler();
  }

  if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
      Error_Handler();/* Notification Error */
  }

  TxHeader.Identifier = 0x10;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	/////////////////  Pacote 1 ///////////////////////////////////////////////////////
	Dados1.Potencia.Ativa = P_Ativa * GANHOPQ + OFFSETPQ;
	Dados1.Potencia.Reativa = P_Reativa * GANHOPQ + OFFSETPQ;
	TxHeader.Identifier = 0x10;
	TxHeader.DataLength = FDCAN_DLC_BYTES_4;

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, Dados1.all)!= HAL_OK){
		Error_Handler();
	}
	HAL_Delay(500);
	cont_loop_sender++;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);

	/////////////////  Pacote 2 ///////////////////////////////////////////////////////
	Dados2.Correntes.Ia = Ia_RMS * GANHOI;
	Dados2.Correntes.Ib = Ib_RMS * GANHOI;
	Dados2.Correntes.Ic = Ic_RMS * GANHOI;
	TxHeader.Identifier = 0x11;
	TxHeader.DataLength = FDCAN_DLC_BYTES_6;

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, Dados2.all)!= HAL_OK){
		Error_Handler();
	}
	HAL_Delay(500);
	cont_loop_sender++;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);

	TxHeader.Identifier = 0x12;
	TxHeader.DataLength = FDCAN_DLC_BYTES_6;
	Dados3.Tensoes.Va = Va_RMS * GANHOV;
	Dados3.Tensoes.Vb = Vb_RMS * GANHOV;
	Dados3.Tensoes.Vc = Vc_RMS * GANHOV;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, Dados3.all)!= HAL_OK){
		Error_Handler();
	}
	HAL_Delay(500);


	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);

	cont_loop_sender++;
	if(cont_loop_sender>100000000)cont_loop_sender=0;


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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV4;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 5;
  hfdcan1.Init.NominalTimeSeg1 = 14;
  hfdcan1.Init.NominalTimeSeg2 = 5;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 5;
  hfdcan1.Init.DataTimeSeg1 = 14;
  hfdcan1.Init.DataTimeSeg2 = 5;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
