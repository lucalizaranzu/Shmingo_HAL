/**
  ******************************************************************************
  * @file    SHAL_CORE.h
  * @author  Luca Lizaranzu
  * @brief   Defines universal macros and objects used across all STM32 families
  * Includes for ST Microelectronics pre-made C headers for STM32 device families
  ******************************************************************************
  */
#ifndef SHMINGO_HAL_SHAL_CORE_H
#define SHMINGO_HAL_SHAL_CORE_H

#include <cstdint>

//Universal structs and defines ---------------------------



//---------------------------------------------------------


#if defined(STM32F030x6)
#include "stm32f030x6.h"
#elif defined(STM32F030x8)
#include "stm32f030x8.h"
#elif defined(STM32F031x6)
#include "stm32f031x6.h"
#elif defined(STM32F038xx)
#include "stm32f038xx.h"
#elif defined(STM32F042x6)
#include "stm32f042x6.h"
#elif defined(STM32F048xx)
#include "stm32f048xx.h"
#elif defined(STM32F051x8)
#include "stm32f051x8.h"
#elif defined(STM32F058xx)
#include "stm32f058xx.h"
#elif defined(STM32F070x6)
#include "stm32f070x6.h"
#elif defined(STM32F070xB)
#include "stm32f070xb.h"
#elif defined(STM32F071xB)
#include "stm32f071xb.h"
#elif defined(STM32F072xB)
#include "stm32f072xb.h"
#elif defined(STM32F078xx)
#include "stm32f078xx.h"
#elif defined(STM32F091xC)
  #include "stm32f091xc.h"
#elif defined(STM32F098xx)
  #include "stm32f098xx.h"
#elif defined(STM32F030xC)
  #include "stm32f030xc.h"
#else
 #error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif


#endif //SHMINGO_HAL_SHAL_CORE_H
