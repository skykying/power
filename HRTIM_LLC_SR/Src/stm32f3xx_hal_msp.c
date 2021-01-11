/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/Src/stm32f3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup HRTIM_LLC_HalfBridge
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
* @brief  HAL_HRTIM_MspInit
* @param  None
* @retval None
*/
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef * hhrtim)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM input: FAULT1 (PA12) */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);
  
  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* Configure and enable HRTIM TIMERA interrupt channel in NVIC */
  HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);

  /* Configure and enable HRTIM TIMERD interrupt channel in NVIC */
  HAL_NVIC_SetPriority(HRTIM1_TIMC_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_TIMC_IRQn);
}


/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_ADC12_CONFIG(RCC_ADC12PLLCLK_DIV1);
  /* ADC2 Periph clock enable */
  __HAL_RCC_ADC12_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure PA2 as analog input */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure PA5 as analog input */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for DMA transfer complete interrupt (not used here) */
  //  HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
  //  HAL_NVIC_EnableIRQ(ADCx_IRQn);

  HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 1);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

/**
  * @brief DAC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* DAC Periph clock enable */
  __HAL_RCC_DAC1_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* DAC Channel1 GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;
  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance                 = USARTx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  /* Configure the DMA handler for reception process */
  hdma_rx.Instance                 = USARTx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);
    
  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (USART3_TX) */
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);
    
  /* NVIC configuration for DMA transfer complete interrupt (USART3_RX) */
  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);
  
  /* NVIC for USART, to catch the TX complete */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure USARTx Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure USARTx Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
   
  /*##-3- Disable the DMA #####################################################*/
  /* De-Initialize the DMA channel associated to reception process */
  if(huart->hdmarx != 0)
  {
    HAL_DMA_DeInit(huart->hdmarx);
  }
  /* De-Initialize the DMA channel associated to transmission process */
  if(huart->hdmatx != 0)
  {
    HAL_DMA_DeInit(huart->hdmatx);
  }  
  
  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(USARTx_DMA_TX_IRQn);
  HAL_NVIC_DisableIRQ(USARTx_DMA_RX_IRQn);
}

/**
  * @brief COMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef      GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* COMP2 Non-inverting input: PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*##-3- Configure the NVIC for COMPx ########################################*/
  /* NVIC for COMPx */
//  HAL_NVIC_SetPriority(COMPx_IRQn, 0, 1);
//  HAL_NVIC_EnableIRQ(COMPx_IRQn);
}

#ifdef SMBUS_ENABLE
/**
  * @brief SMBUS MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for DMA interrupt request enable
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspInit(SMBUS_HandleTypeDef *hsmbus)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

  /*##-1- Configure the SMBUS clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = SMBUSx_RCC_PERIPHCLK;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = SMBUSx_CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SMBUSx_SCL_GPIO_CLK_ENABLE();
  SMBUSx_SDA_GPIO_CLK_ENABLE();
  /* Enable SMBUSx clock */
  SMBUSx_CLK_ENABLE(); 

  /*##-3- Configure peripheral GPIO ##########################################*/  
  /* SMBUS TX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SCL_PIN;
  GPIO_InitStruct.Mode             = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull             = GPIO_PULLUP;
  GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_ALERT_AF;
  
  HAL_GPIO_Init(SMBUSx_SCL_GPIO_PORT, &GPIO_InitStruct);
    
  /* SMBUS RX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SDA_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_ALERT_AF;
    
  HAL_GPIO_Init(SMBUSx_SDA_GPIO_PORT, &GPIO_InitStruct);
    
  /* SMBUS ALERT GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_ALERT_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_ALERT_AF;
    
  HAL_GPIO_Init(SMBUSx_ALERT_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-4- Configure the NVIC for SMBUS ########################################*/   
  /* NVIC for SMBUSx */
  HAL_NVIC_SetPriority(SMBUSx_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SMBUSx_ER_IRQn);
  HAL_NVIC_SetPriority(SMBUSx_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SMBUSx_EV_IRQn);
}

/**
  * @brief SMBUS MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspDeInit(SMBUS_HandleTypeDef *hsmbus)
{
  
  /*##-1- Reset peripherals ##################################################*/
  SMBUSx_FORCE_RESET();
  SMBUSx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure SMBUS Tx as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_SCL_GPIO_PORT, SMBUSx_SCL_PIN);
  /* Configure SMBUS Rx as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_SDA_GPIO_PORT, SMBUSx_SDA_PIN);
  /* Configure SMBUS Alert as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_ALERT_GPIO_PORT, SMBUSx_ALERT_PIN);
   
  /*##-4- Disable the NVIC for SMBUS ###########################################*/
  HAL_NVIC_DisableIRQ(SMBUSx_ER_IRQn);
  HAL_NVIC_DisableIRQ(SMBUSx_EV_IRQn);
}
#endif //SMBUS_ENABLE
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
