//
// Created by Luca on 9/7/2025.
//

#ifndef SHMINGO_HAL_SHAL_TIM_REG_H
#define SHMINGO_HAL_SHAL_TIM_REG_H

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
#include "SHAL_TIM_REG_F072xB.h"
#elif defined(STM32F078xx)
#include "stm32f078xx.h"
#elif defined(STM32F091xC)
  #include "stm32f091xc.h"
#elif defined(STM32F098xx)
  #include "stm32f098xx.h"
#elif defined(STM32F030xC)
  #include "stm32f030xc.h"
#elif defined(STM32F030xC)
#include "stm32f030xc.h"
#elif defined(STM32L412xx)
#include "stm32l412xx.h"
#elif defined(STM32L422xx)
#include "stm32l422xx.h"
#elif defined(STM32L431xx)
#include "stm32l431xx.h"
#elif defined(STM32L432xx)
#include "stm32l432xx.h"
#elif defined(STM32L433xx)
#include "stm32l433xx.h"
#elif defined(STM32L442xx)
  #include "stm32l442xx.h"
#elif defined(STM32L443xx)
  #include "stm32l443xx.h"
#elif defined(STM32L451xx)
  #include "stm32l451xx.h"
#elif defined(STM32L452xx)
  #include "stm32l452xx.h"
#elif defined(STM32L462xx)
  #include "stm32l462xx.h"
#elif defined(STM32L471xx)
  #include "stm32l471xx.h"
#elif defined(STM32L475xx)
  #include "stm32l475xx.h"
#elif defined(STM32L476xx)
  #include "stm32l476xx.h"
#elif defined(STM32L485xx)
  #include "stm32l485xx.h"
#elif defined(STM32L486xx)
  #include "stm32l486xx.h"
#elif defined(STM32L496xx)
  #include "stm32l496xx.h"
#elif defined(STM32L4A6xx)
  #include "stm32l4a6xx.h"
#elif defined(STM32L4P5xx)
  #include "stm32l4p5xx.h"
#elif defined(STM32L4Q5xx)
  #include "stm32l4q5xx.h"
#elif defined(STM32L4R5xx)
  #include "stm32l4r5xx.h"
#elif defined(STM32L4R7xx)
  #include "stm32l4r7xx.h"
#elif defined(STM32L4R9xx)
  #include "stm32l4r9xx.h"
#elif defined(STM32L4S5xx)
  #include "stm32l4s5xx.h"
#elif defined(STM32L4S7xx)
  #include "stm32l4s7xx.h"
#elif defined(STM32L4S9xx)
#else
 #error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif

#endif //SHMINGO_HAL_SHAL_TIM_REG_H
