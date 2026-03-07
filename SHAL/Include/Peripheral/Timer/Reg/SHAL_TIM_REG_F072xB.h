/**
  ******************************************************************************
  * @file    SHAL_TIM_REG.h
  * @author  Luca Lizaranzu
  * @brief   Defines universal macros and objects used across all STM32 families
  ******************************************************************************
  */

#ifndef SHAL_TIM_REG_F072XB_H
#define SHAL_TIM_REG_F072XB_H

#include <cassert>
#include <stm32f072xb.h>

#include "SHAL_CORE.h"
#include "SHAL_TIM_TYPES.h"

enum class Timer_Key : uint8_t { //For STM32F072
    S_TIM1 = 0,
    S_TIM2 = 1,
    S_TIM3 = 2,
    S_TIM6,
    S_TIM7,
    S_TIM14,
    S_TIM15,
    S_TIM16,
    S_TIM17,
    NUM_TIMERS,
    S_TIM_INVALID
};

#define SHAL_TIM1 TimerManager::get(Timer_Key::S_TIM1)
#define SHAL_TIM2 TimerManager::get(Timer_Key::S_TIM2)
#define SHAL_TIM3 TimerManager::get(Timer_Key::S_TIM3)
#define SHAL_TIM6 TimerManager::get(Timer_Key::S_TIM6)
#define SHAL_TIM7 TimerManager::get(Timer_Key::S_TIM7)
#define SHAL_TIM14 TimerManager::get(Timer_Key::S_TIM14)
#define SHAL_TIM15 TimerManager::get(Timer_Key::S_TIM15)
#define SHAL_TIM16 TimerManager::get(Timer_Key::S_TIM16)
#define SHAL_TIM17 TimerManager::get(Timer_Key::S_TIM17)

static SHAL_TIM_Info TIM_INFO_TABLE[9] = {
        {TIM1,TIM1_BRK_UP_TRG_COM_IRQn,4},
        {TIM2,TIM2_IRQn,4},
        {TIM3,TIM3_IRQn,4},
        {TIM6,TIM6_DAC_IRQn,0},
        {TIM7,TIM7_IRQn,0},
        {TIM14,TIM14_IRQn,1},
        {TIM15,TIM15_IRQn,2},
        {TIM16,TIM16_IRQn,1},
        {TIM17,TIM17_IRQn,1},
};

//Get actual register value based on enum
static volatile TIM_TypeDef* getTimerRegister(Timer_Key t) {
    return TIM_INFO_TABLE[static_cast<uint8_t>(t)].timer;
}

static IRQn_Type getIRQn(Timer_Key t) {
    return TIM_INFO_TABLE[static_cast<uint8_t>(t)].IRQn;
}


//Get TIMER_KEY peripheral struct including bus register, enable mask, TIMER_KEY mask
static SHAL_TIM_RCC_Register getTimerRCC(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN};
        case Timer_Key::S_TIM2:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN};
        case Timer_Key::S_TIM3:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN};
        case Timer_Key::S_TIM6:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN};
        case Timer_Key::S_TIM7:  return {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN};
        case Timer_Key::S_TIM14: return {&RCC->APB1ENR, RCC_APB1ENR_TIM14EN};
        case Timer_Key::S_TIM15: return {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN};
        case Timer_Key::S_TIM16: return {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN};
        case Timer_Key::S_TIM17: return {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN};
        case Timer_Key::NUM_TIMERS:
        case Timer_Key::S_TIM_INVALID:
            assert(false);
            return {nullptr, 0};; //Unreachable
    }

    __builtin_unreachable();
}


static inline SHAL_TIM_Status_Register getTimerStatusRegister(Timer_Key key){

    SHAL_TIM_Status_Register res = {nullptr, TIM_SR_UIF};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->SR;
    return res;
}

static inline SHAL_TIM_Control_Register_1 getTimerControlRegister1(Timer_Key key){

    SHAL_TIM_Control_Register_1 res = {nullptr, TIM_CR1_CEN_Msk,
                                       TIM_CR1_UDIS,
                                       TIM_CR1_OPM,
                                       TIM_CR1_CMS_Pos,
                                       TIM_CR1_ARPE};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->CR1;
    return res;
}

static inline SHAL_TIM_DMA_Interrupt_Enable_Register getTimerDMAInterruptEnableRegister(Timer_Key key){

    SHAL_TIM_DMA_Interrupt_Enable_Register res = {nullptr, TIM_DIER_UIE};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->DIER;
    return res;
}

static inline SHAL_TIM_Event_Generation_Register getTimerEventGenerationRegister(Timer_Key key){

    SHAL_TIM_Event_Generation_Register res = {nullptr, TIM_EGR_UG};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->EGR;
    return res;
}

static inline SHAL_TIM_Break_Dead_Time_Register getTimerBreakDeadTimeRegister(Timer_Key key) {

    SHAL_TIM_Break_Dead_Time_Register res = {nullptr,
                                             TIM_BDTR_DTG_Pos,
                                             TIM_BDTR_LOCK_Pos,
                                             TIM_BDTR_OSSI,
                                             TIM_BDTR_OSSR,
                                             TIM_BDTR_BKE,
                                             TIM_BDTR_BKP,
                                             TIM_BDTR_AOE,
                                             TIM_BDTR_MOE};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->BDTR;
    return res;
}

static inline SHAL_TIM_Prescaler_Register getTimerPrescalerRegister(Timer_Key key){

    SHAL_TIM_Prescaler_Register res = {nullptr, 1UL << 15};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->PSC;
    return res;
}

static inline SHAL_TIM_Auto_Reload_Register getTimerAutoReloadRegister(Timer_Key key){

    SHAL_TIM_Auto_Reload_Register res = {nullptr, 1UL << 15};

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->ARR;
    return res;
}

static inline SHAL_TIM_Capture_Compare_Register getTimerCaptureCompareRegister(Timer_Key key, SHAL_Timer_Channel channel){
    auto channel_num = static_cast<uint8_t>(channel);

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    assert(channel_num <= TIM_INFO_TABLE[static_cast<uint8_t>(key)].numChannels);

    switch(channel){
        case SHAL_Timer_Channel::CH1: return {&tim->CCR1,0};
        case SHAL_Timer_Channel::CH2: return {&tim->CCR2,0};
        case SHAL_Timer_Channel::CH3: return {&tim->CCR3,0};
        case SHAL_Timer_Channel::CH4: return {&tim->CCR4,0};
    }
}

static inline SHAL_TIM_Capture_Compare_Enable_Register getTimerCaptureCompareEnableRegister(Timer_Key key, SHAL_Timer_Channel channel){

    uint8_t channel_stride = 3;
    auto channel_num = static_cast<uint8_t>(channel);

    auto output_enable = TIM_CCER_CC1E << (channel_stride * (channel_num - 1));
    auto output_polarity = TIM_CCER_CC1P << (channel_stride * channel_num);
    auto output_complimentary_enable = TIM_CCER_CC1NE << (channel_stride * channel_num);
    auto output_complimentary_polarity = TIM_CCER_CC1NP << (channel_stride * channel_num);

    SHAL_TIM_Capture_Compare_Enable_Register res = {nullptr,
                                                    output_enable,
                                                    output_polarity,
                                                    output_complimentary_enable,
                                                    output_complimentary_polarity,
                                         };

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;

    res.reg = &tim->CCER;
    return res;
}

static inline SHAL_TIM_Output_Capture_Compare_Mode_Register getTimerOutputCaptureCompareModeRegister(Timer_Key key, SHAL_Timer_Channel channel) {
    SHAL_TIM_Output_Capture_Compare_Mode_Register res = {
            nullptr,
            TIM_CCMR1_CC1S_Pos,   //Channel 1 Capture/Compare selection
            TIM_CCMR1_OC1FE,  //Channel 1 Fast enable
            TIM_CCMR1_OC1PE,  //Channel 1 Preload enable
            TIM_CCMR1_OC1M_Pos,   //Channel 1 Mode (OC1M)
            TIM_CCMR1_OC1CE,  //Channel 1 Clear enable
            TIM_CCMR1_CC2S_Pos,   //Channel 2 Capture/Compare selection
            TIM_CCMR1_OC2FE,  //Channel 2 Fast enable
            TIM_CCMR1_OC2PE,  //Channel 2 Preload enable
            TIM_CCMR1_OC2M_Pos,   //Channel 2 Mode (OC2M)
            TIM_CCMR1_OC2CE   //Channel 2 Clear enable
    };

    volatile TIM_TypeDef* tim = TIM_INFO_TABLE[static_cast<uint8_t>(key)].timer;
    uint8_t num_tim_channels = TIM_INFO_TABLE[static_cast<uint8_t>(key)].numChannels;

    volatile uint32_t* reg = nullptr;

    uint8_t channelNum = static_cast<uint32_t>(channel);

    assert(num_tim_channels >= channelNum); //Assert that we don't access undefined memory trying to initialize a non-existent channel

    if(channelNum >= 3){
        reg = &tim->CCMR2;
    }
    else{
        reg = &tim->CCMR1;
    }

    res.reg = reg;

    return res;
}



#endif