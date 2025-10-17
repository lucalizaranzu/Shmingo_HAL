/**
  ******************************************************************************
  * @file    SHAL_TIM_REG.h
  * @author  Luca Lizaranzu
  * @brief   Defines universal macros and objects used across all STM32 families
  ******************************************************************************
  */

#ifndef SHAL_TIM_REG_L432KC_H
#define SHAL_TIM_REG_L432KC_H

#include <cassert>
#include <stm32l432xx.h>

#include "SHAL_CORE.h"
#include "SHAL_TIM_TYPES.h"

enum class Timer_Key : uint8_t { //For STM32L432
    S_TIM1,
    S_TIM2,
    S_TIM6,
    S_TIM7,
    S_TIM15,
    S_TIM16,
    NUM_TIMERS,
    S_TIM_INVALID
};

//Lookup table for timer typedefs
static volatile TIM_TypeDef* TIM_TABLE[6] = {
        TIM1,
        TIM2,
        TIM6,
        TIM7,
        TIM15,
        TIM16,
};

static IRQn_Type IRQN_TABLE[6] = {
    TIM1_TRG_COM_IRQn,
    TIM2_IRQn,
    TIM6_DAC_IRQn,
    TIM7_IRQn,
    TIM1_BRK_TIM15_IRQn,
    TIM1_UP_TIM16_IRQn
};

#define SHAL_TIM1 TimerManager::get(Timer_Key::S_TIM1)
#define SHAL_TIM2 TimerManager::get(Timer_Key::S_TIM2)
#define SHAL_TIM6 TimerManager::get(Timer_Key::S_TIM6)
#define SHAL_TIM7 TimerManager::get(Timer_Key::S_TIM7)
#define SHAL_TIM15 TimerManager::get(Timer_Key::S_TIM15)
#define SHAL_TIM16 TimerManager::get(Timer_Key::S_TIM16)



static inline SHAL_TIM_Status_Register getTimerStatusRegister(Timer_Key key){

    SHAL_TIM_Status_Register res = {nullptr, TIM_SR_UIF};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->SR;
    return res;
}

static inline SHAL_TIM_Control_Register_1 getTimerControlRegister1(Timer_Key key){

    SHAL_TIM_Control_Register_1 res = {nullptr, TIM_CR1_CEN_Msk, TIM_CR1_UDIS, TIM_CR1_OPM, TIM_CR1_CMS_Pos};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->CR1;
    return res;
}

static inline SHAL_TIM_DMA_Interrupt_Enable_Register getTimerDMAInterruptEnableRegister(Timer_Key key){

    SHAL_TIM_DMA_Interrupt_Enable_Register res = {nullptr, TIM_DIER_UIE};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->DIER;
    return res;
}

static inline SHAL_TIM_Event_Generation_Register getTimerEventGenerationRegister(Timer_Key key){

    SHAL_TIM_Event_Generation_Register res = {nullptr, TIM_EGR_UG};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->EGR;
    return res;
}

static inline SHAL_TIM_Prescaler_Register getTimerPrescalerRegister(Timer_Key key){

    SHAL_TIM_Prescaler_Register res = {nullptr, 1UL << 15};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->PSC;
    return res;
}

static inline SHAL_TIM_Auto_Reload_Register getTimerAutoReloadRegister(Timer_Key key){

    SHAL_TIM_Auto_Reload_Register res = {nullptr, 1UL << 15};

    volatile TIM_TypeDef* tim = TIM_TABLE[static_cast<uint8_t>(key)];

    res.reg = &tim->ARR;
    return res;
}

//Get TIMER_KEY peripheral struct including bus register, enable mask, TIMER_KEY mask
static inline SHAL_TIM_RCC_Register getTimerRCC(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN};
        case Timer_Key::S_TIM2:  return {&RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN};
        case Timer_Key::S_TIM6:  return {&RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN};
        case Timer_Key::S_TIM7:  return {&RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN};
        case Timer_Key::S_TIM15: return {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN};
        case Timer_Key::S_TIM16: return {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN};
        case Timer_Key::NUM_TIMERS:
        case Timer_Key::S_TIM_INVALID:
            assert(false);
    }

    __builtin_unreachable();
}


//Get timer IRQN from lookup table
static inline IRQn_Type getTimerIRQn(Timer_Key t) {
    return IRQN_TABLE[static_cast<uint8_t>(t)];
}

#endif