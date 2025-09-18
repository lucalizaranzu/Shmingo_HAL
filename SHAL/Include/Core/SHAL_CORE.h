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

//Overall init function for SHAL --------------------------

void SHAL_init();

//---------------------------------------------------------




//Universal structs and defines ---------------------------

typedef bool (*condition_fn_t)(void);

#define SHAL_WAIT_FOR_CONDITION_US(cond, timeout_us) \
    SHAL_wait_for_condition_us([&](){ return (cond); }, (timeout_us))

#define SHAL_WAIT_FOR_CONDITION_MS(cond, timeout_ms) \
    SHAL_wait_for_condition_ms([&](){ return (cond); }, (timeout_ms))



//Currently configures systick to count down in microseconds
void systick_init();

//Max of 16ms, use SHAL_delay_ms for longer delay
void SHAL_delay_us(uint32_t us);

void SHAL_delay_ms(uint32_t ms);

template<typename Condition>
bool SHAL_wait_for_condition_us(Condition cond, uint32_t timeout_us) {
    while (timeout_us--) {
        if (cond()) {
            return true; // success
        }
        SHAL_delay_us(1);
    }
    return false; // timeout
}

template<typename Condition>
bool SHAL_wait_for_condition_ms(Condition cond, uint32_t timeout_ms) {
    while (timeout_ms--) {
        if (cond()) {
            return true; // success
        }
        SHAL_delay_ms(1);
    }
    return false; // timeout
}

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
