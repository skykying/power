/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_DCM_PFC/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to control a transition mode PFC with HRTIM
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "main.h"
#include "arm_math.h"
#include "stdio.h"

#define TRANSMITTER_BOARD

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */


#define NPT  512  
#define Fs  1000
#define PI2 6.28318530717959f

static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Error_Handler(void);

static float32_t  testInput_f32[NPT];
static float32_t  testOutput_f32[NPT*2];
static float32_t  testOutput[NPT];
	
static int sindex = 0;

static float32_t maxValue;
static uint32_t refIndex = 213, testIndex = 0;

static float32_t v = 2.1f;
static float32_t o1;
static float32_t o2;
static float32_t pid_error = 1.1f;
static float32_t duty;
	
static arm_rfft_fast_instance_f32 S; 
static uint16_t fftSize = NPT;  
static int i = 0;
static uint8_t ifftFlag = 0; 
static arm_pid_instance_f32 pid;
		
void uart_callback( void ){
	
#ifdef TRANSMITTER_BOARD
/* Wait for User push-button press before starting the Communication.
     In the meantime, LED6 is blinking */
  if(UserButtonStatus == 0) {
      /* Toggle LED6*/
      BSP_LED_Toggle(LED6); 
      HAL_Delay(100);
      arm_sin_cos_f32(v,&o1,&o2);
      
      duty = arm_pid_f32(&pid, pid_error);
      pid_error = v - duty;
      
      for(i=0; i<NPT; i++) 
      { 
        testInput_f32[i] = 1.2f*arm_sin_f32(PI2*i*200/Fs)+1;
      } 

      arm_rfft_fast_f32(&S, testInput_f32, testOutput_f32, ifftFlag); 
      arm_cmplx_mag_f32(testOutput_f32, testOutput, fftSize);
      arm_max_f32(&testOutput[1], fftSize-1, &maxValue, &testIndex);

      sprintf((char*)aTxBuffer, "%f -- %f  \n\r", duty, pid_error);
      HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE);
  } else {
  
		BSP_LED_Off(LED6);

		for( sindex = 0; sindex < NPT; sindex++){
				sprintf((char*)aTxBuffer, "[[[%d = %f]]] \n\r", sindex,testOutput[sindex]);
				HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE);
				HAL_Delay(50);
		}

		sprintf((char*)aTxBuffer, "[[[%f - %d]]] \n\r", maxValue,testIndex);
		HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE);
		/* The board sends the message and expects to receive it back */
		
		/*##-2- Start the transmission process #####################################*/  
		/* While the UART in reception process, user can transmit data through 
			 "aTxBuffer" buffer */
		if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
		{
			//Error_Handler();
		}
		
		/*##-3- Wait for the end of the transfer ###################################*/   
		while (UartReady != SET)
		{
		}
		
		/* Reset transmission flag */
		UartReady = RESET;
		
		/*##-4- Put UART peripheral in reception process ###########################*/  
		if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
		{
			//Error_Handler();
		}

    UserButtonStatus = 0;
	}

#else
  
  /* The board receives the message and sends it back */

  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wait for the end of the transfer ###################################*/   
  /* While waiting for message to come from the other board, LED6 is
     blinking according to the following pattern: a double flash every half-second */  
  while (UartReady != SET)
  {
      BSP_LED_On(LED6); 
      HAL_Delay(100);
      BSP_LED_Off(LED6); 
      HAL_Delay(100);
      BSP_LED_On(LED6); 
      HAL_Delay(100);
      BSP_LED_Off(LED6); 
      HAL_Delay(500); 
  } 
  
  /* Reset transmission flag */
  UartReady = RESET;
  BSP_LED_Off(LED6); 
  
  /*##-4- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    Error_Handler();
  }
  
#endif /* TRANSMITTER_BOARD */

}

void debug_print( char* msg ){
	    sprintf((char*)aTxBuffer, "%s \n\r", msg);
      HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE);
}

void alg_init(void){
  arm_rfft_fast_init_f32(&S, fftSize); 

  pid.Kp = 0.55f; 
  pid.Ki = 0.55f;
  pid.Kd = 0;
  arm_pid_init_f32(&pid,pid_error);
}

void UART_init(void){

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
#endif
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_BUTTON_PIN)
  {  
    UserButtonStatus = 1;
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReady = SET;

  /* Turn LED5 on: Transfer in transmission process is correct */
  BSP_LED_On(LED5); 
  
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReady = SET;
  
  /* Turn LED4 on: Transfer in reception process is correct */
  BSP_LED_On(LED4);
  
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3); 
}


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn on LED4 (ORANGE) */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}
