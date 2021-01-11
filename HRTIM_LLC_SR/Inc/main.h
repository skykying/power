/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3348_discovery.h"

/* Exported types ------------------------------------------------------------*/
typedef enum { RUN, FAULT, IDLE } DemoModeTypeDef;

/* Exported constants ---------------------------------------------------------*/

/* This defines the minimum LLC operating frequency, here 70.35kHz */
#define LLC_PWM_PERIOD ((uint16_t)0xFFDF)

/* This value is used to position the ADC sampling point before and after the 
commutation on the synchronous rectification FETs */
/* It corresponds to 250ns (0x480/32) x (1/144MHz), slightly above ADC
conversion time */
#define ADC_CONVERSION_TIME     ((uint16_t)0x480)

/* Exported macro ------------------------------------------------------------*/
/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA1_Channel2
#define USARTx_RX_DMA_CHANNEL             DMA1_Channel3


/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Channel2_IRQn
#define USARTx_DMA_RX_IRQn                DMA1_Channel3_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Channel2_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA1_Channel3_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */
/* Exported functions --------------------------------------------------------- */


/* Definition for SMBUSx clock resources */
#define SMBUSx_RCC_PERIPHCLK              RCC_PERIPHCLK_I2C1
#define SMBUSx_CLKSOURCE_SYSCLK           RCC_I2C1CLKSOURCE_SYSCLK
#define SMBUSx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define SMBUSx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SMBUSx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 

#define SMBUSx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define SMBUSx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for SMBUSx Pins */
#define SMBUSx_SCL_PIN                    GPIO_PIN_6
#define SMBUSx_SCL_GPIO_PORT              GPIOF
#define SMBUSx_SDA_PIN                    GPIO_PIN_10
#define SMBUSx_SDA_GPIO_PORT              GPIOA
#define SMBUSx_ALERT_PIN                  GPIO_PIN_8
#define SMBUSx_ALERT_GPIO_PORT            GPIOA
#define SMBUSx_SCL_SDA_ALERT_AF           GPIO_AF4_I2C1

/* Definition for SMBUSx's NVIC */
#define SMBUSx_EV_IRQn                    I2C1_EV_IRQn
#define SMBUSx_ER_IRQn                    I2C1_ER_IRQn
#define SMBUSx_EV_IRQHandler              I2C1_EV_IRQHandler
#define SMBUSx_ER_IRQHandler              I2C1_ER_IRQHandler



extern DemoModeTypeDef DemoState;
extern HRTIM_HandleTypeDef hhrtim;


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
