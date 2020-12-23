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


static float32_t v = 2.1f;
static float32_t o1;
static float32_t o2;
float32_t pid_error = 1.1f;
float32_t duty;
	
static arm_pid_instance_f32 pid;

void ALG_PI_regulator( void ) {
    arm_sin_cos_f32(v,&o1,&o2);
      
    duty = arm_pid_f32(&pid, pid_error);
    pid_error = v - duty;
}


void ALG_init(void){

  pid.Kp = 0.55f; 
  pid.Ki = 0.55f;
  pid.Kd = 0;
  arm_pid_init_f32(&pid,pid_error);
}

