/**
  ******************************************************************************
  * @file    timer.c 
  * @brief   HAL time base based on the hardware TIM.
  *    
  *          This file override the native HAL time base functions (defined as weak)
  *          the TIM time base:
  *           + Intializes the TIM peripheral generate a Period elapsed Event each 1ms
  *           + HAL_IncTick is called inside HAL_TIM_PeriodElapsedCallback ie each 1ms
  * 
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

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL_TimeBase_TIM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef        Tim6Handle;
TIM_HandleTypeDef        Tim15Handle;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the TIM6 as a time base source. 
  *         The time source is configured  to have 1ms time base with a dedicated 
  *         Tick interrupt priority. 
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig(). 
  * @param  TickPriority Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef TIMER_VAQ_Config (void)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency;
  
  /* Enable TIM6 clock */
  __HAL_RCC_TIM6_CLK_ENABLE();
  
  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;
  
  /* Compute TIM6 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2U*HAL_RCC_GetPCLK1Freq();
  }
  
  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
  
  /* Initialize TIM6 */
  Tim6Handle.Instance = TIM6;
  
      /*Configure the TIM6 IRQ priority */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 4 ,0U);
  
  /* Enable the TIM6 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  
  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM6CLK/1000U) - 1]. to have a (1U/1000U) s time base.
  + Prescaler = (uwTimclock/1000000U - 1U) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  Tim6Handle.Init.Period = (1000000U / 4000U) - 1U;
  Tim6Handle.Init.Prescaler = uwPrescalerValue;
  Tim6Handle.Init.ClockDivision = 0U;
  Tim6Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  Tim6Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&Tim6Handle) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&Tim6Handle);
  }
  
  /* Return function status */
  return HAL_ERROR;
}

/**
  * @brief  This function configures the TIM15 as a time base source. 
  *         The time source is configured  to have 1ms time base with a dedicated 
  *         Tick interrupt priority. 
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig(). 
  * @param  TickPriority Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef TIMER_VAL_Config (void)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency;
  
  /* Enable TIM15 clock */
  __HAL_RCC_TIM15_CLK_ENABLE();
  
  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;
  
  /* Compute TIM15 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2U*HAL_RCC_GetPCLK1Freq();
  }
  
  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
  
  /* Initialize TIM15 */
  Tim15Handle.Instance = TIM15;
  
  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM15CLK/1000U) - 1]. to have a (1U/1000U) s time base.
  + Prescaler = (uwTimclock/1000000U - 1U) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  Tim15Handle.Init.Period = (1000000U / 200U) - 1U;
  Tim15Handle.Init.Prescaler = uwPrescalerValue;
  Tim15Handle.Init.ClockDivision = 0U;
  Tim15Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  Tim15Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  /*Configure the TIM15 IRQ priority */
  HAL_NVIC_SetPriority(TIM15_IRQn, 4 ,1U);
  
  /* Enable the TIM15 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM15_IRQn);


  if(HAL_TIM_Base_Init(&Tim15Handle) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&Tim15Handle);
  }
  
  /* Return function status */
  return HAL_ERROR;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void TIMER_SuspendTick(void)
{
  /* Disable TIM6 update Interrupt */
  __HAL_TIM_DISABLE_IT(&Tim6Handle, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void TIMER_ResumeTick(void)
{
  /* Enable TIM6 Update interrupt */
  __HAL_TIM_ENABLE_IT(&Tim6Handle, TIM_IT_UPDATE);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // debug_print("HAL_TIM_PeriodElapsedCallback");
	
	if( htim->Instance == TIM15){
		//BSP_LED_Toggle(LED4);
	}
	
	if( htim->Instance == TIM6){
		BSP_LED_Toggle(LED4);
	}
		
}



/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
