/**
  ******************************************************************************
  * @file    SHAL_TIM_REG.h
  * @author  Luca Lizaranzu
  * @brief   Defines universal macros and objects used across all STM32 families
  ******************************************************************************
  */

#ifndef SHAL_TIM_REG_H
#define SHAL_TIM_REG_H

#include <cassert>
#include <stm32f072xb.h>

#include "SHAL_CORE.h"
#include "SHAL_TIM_TYPES.h"

enum class Timer_Key : uint8_t { //For STM32F072
    S_TIM1,
    S_TIM2,
    S_TIM3,
    S_TIM6,
    S_TIM7,
    S_TIM14,
    S_TIM15,
    S_TIM16,
    S_TIM17,
    NUM_TIMERS,
    S_TIM_INVALID
};


//Get TIMER_KEY peripheral struct including bus register, enable mask, TIMER_KEY mask
constexpr TIM_RCC_Enable getTimerRCC(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Pos};
        case Timer_Key::S_TIM2:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN_Pos};
        case Timer_Key::S_TIM3:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN_Pos};
        case Timer_Key::S_TIM6:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN_Pos};
        case Timer_Key::S_TIM7:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN_Pos};
        case Timer_Key::S_TIM14: return {&RCC->APB1ENR, RCC_APB1ENR_TIM14EN_Pos};
        case Timer_Key::S_TIM15: return {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN_Pos};
        case Timer_Key::S_TIM16: return {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN_Pos};
        case Timer_Key::S_TIM17: return {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN_Pos};
        case Timer_Key::NUM_TIMERS:
        case Timer_Key::S_TIM_INVALID:
            assert(false);
            return {nullptr, 0};; //Unreachable
    }

    __builtin_unreachable();
}

//Get actual register value based on enum
constexpr volatile TIM_TypeDef* getTimerRegister(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return TIM1;
        case Timer_Key::S_TIM2:  return TIM2;
        case Timer_Key::S_TIM3:  return TIM3;
        case Timer_Key::S_TIM6:  return TIM6;
        case Timer_Key::S_TIM7:  return TIM7;
        case Timer_Key::S_TIM14:  return TIM14;
        case Timer_Key::S_TIM15:  return TIM15;
        case Timer_Key::S_TIM16:  return TIM16;
        case Timer_Key::S_TIM17:  return TIM17;
        case Timer_Key::NUM_TIMERS:
        case Timer_Key::S_TIM_INVALID:
            assert(false);
            return nullptr; //Unreachable
    }
    __builtin_unreachable();
}

constexpr IRQn_Type getIRQn(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return TIM1_BRK_UP_TRG_COM_IRQn;
        case Timer_Key::S_TIM2:  return TIM2_IRQn;
        case Timer_Key::S_TIM3:  return TIM3_IRQn;
        case Timer_Key::S_TIM6:  return TIM6_DAC_IRQn;
        case Timer_Key::S_TIM7:  return TIM7_IRQn;
        case Timer_Key::S_TIM14: return TIM14_IRQn;
        case Timer_Key::S_TIM15: return TIM15_IRQn;
        case Timer_Key::S_TIM16: return TIM16_IRQn;
        case Timer_Key::S_TIM17: return TIM17_IRQn;
        case Timer_Key::NUM_TIMERS:
        case Timer_Key::S_TIM_INVALID:
            assert(false);
            return TIM1_BRK_UP_TRG_COM_IRQn; //Unreachable
    }
    __builtin_unreachable();
}



#endif