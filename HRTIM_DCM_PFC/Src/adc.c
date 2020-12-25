
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

ADC_HandleTypeDef AdcHandle;

/**
 * @brief  calculate the adc float value
 * @param  hadc: ADC unit32_t value
 */
float32_t calculate_adc_f32 ( uint32_t adc ) {
    float32_t result = adc;
	
    result = result * 3.3f;
    result = result / 4096;

    return result;
}

/**
  * @brief  Injected conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint32_t Vin1Conversion=1, Vin2Conversion =2,Vin3Conversion =3, Vin4Conversion=4;
	char buf[128];
	float32_t v1, v2, v3, v4;
	
  if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS))
  {   
    //__HAL_HRTIM_TIMER_CLEAR_IT(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, HRTIM_TIM_IT_CMP2);

    /* Get ADC converted data */
    Vin1Conversion = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
    Vin2Conversion = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
    Vin3Conversion = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_3);
    Vin4Conversion = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_4);
		
    v1 = calculate_adc_f32(Vin1Conversion);
    v2 = calculate_adc_f32(Vin2Conversion);
    v3 = calculate_adc_f32(Vin3Conversion);
    v4 = calculate_adc_f32(Vin4Conversion);
		
		//sprintf(buf, "%f %f %f %f \n\r", v1,v2,v3,v4);
		//debug_print(buf);
  }

}


/**
* @brief  Configure ADC1 for being used with HRTIM
* @param  None
* @retval None
*/
void ADC_Config(void)
{
  ADC_MultiModeTypeDef MultiModeConfig;
  ADC_InjectionConfTypeDef InjectionConfig;

  AdcHandle.Instance = ADC1;

  /* ADC2 is working independently */
  MultiModeConfig.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode = ADC_MODE_INDEPENDENT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &MultiModeConfig);

  /* ADC1 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode = ENABLE;
  AdcHandle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  AdcHandle.Init.LowPowerAutoWait = DISABLE;
  AdcHandle.Init.ContinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfConversion = 1;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 1;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle);

  /* Discontinuous injected mode: 1st injected conversion for Vin on Ch2 */
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectionConfig.InjectedChannel = ADC_CHANNEL_1;
  InjectionConfig.InjectedDiscontinuousConvMode = DISABLE;
  InjectionConfig.InjectedNbrOfConversion = 4;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.QueueInjectedContext = DISABLE;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure the 2nd injected conversion for Vout on Ch4 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure the 3nd injected conversion for Vout on Ch4 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_3;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_3;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);


  /* Configure the 4nd injected conversion for Vout on Ch4 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_4;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_4;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);


  /* Configure and enable ADC1_2_IRQHandler interrupt channel in NVIC */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

  /* Start ADC1 Injected Conversions */
  HAL_ADCEx_InjectedStart_IT(&AdcHandle);
}
