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

enum class SHAL_Result{
    OKAY,
    ERROR
};


#define SHAL_CALL(func) \
    if(func != SHAL_Result::OKAY){ \
        return SHAL_Result::ERROR; \
    }

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

//Sets bits starting from offset as the LSB
static inline void SHAL_set_bits(volatile uint32_t* reg, uint32_t size, uint32_t bits, uint32_t offset){
    if(reg == nullptr){
        return;
    }
    uint32_t mask = (1 << (size)) - 1;
    *reg &= ~(mask << offset);
    *reg |= bits << offset;
}

//Sets bits starting from offset as the LSB (for uint16_t)
static inline void SHAL_set_bits_16(volatile uint16_t* reg, uint32_t size, uint32_t bits, uint32_t offset){
    uint16_t mask = (1 << (size)) - 1;
    *reg &= ~(mask << offset);
    *reg |= bits << offset;
}

static inline void SHAL_clear_bitmask(volatile uint32_t* reg, uint32_t mask){
    *reg &= ~(mask);
}

static inline void SHAL_apply_bitmask(volatile uint32_t* reg, uint32_t mask){
    SHAL_clear_bitmask(reg,mask);
    *reg |= mask;
}

static inline void SHAL_set_register_value(volatile uint32_t* reg, uint32_t value){
    if(reg == nullptr){
        return;
    }
    *reg = value;
}

static inline void SHAL_set_register_value_16(volatile uint16_t* reg, uint16_t value){
    if(reg == nullptr){
        return;
    }
    *reg = value;
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
#elif defined(STM32F030xC)
  #include "stm32f030xc.h"
#else
 #error "Please select first the target STM32 device used in your application (in stm32f0xx.h file)"
#endif


#endif //SHMINGO_HAL_SHAL_CORE_H
