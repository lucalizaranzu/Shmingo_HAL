#ifndef SHAL_TIM_REG_H
#define SHAL_TIM_REG_H

#include <cstdint>
#include <stm32f072xb.h>

enum class Bus {
    AHB,
    APB1,
    APB2,
    INVALID
};

struct RCC_Peripheral {
    Bus bus;
    volatile uint32_t* reg;
    uint32_t bitmask;
};

enum class Timer_Key { //For STM32F072
    S_TIM1,
    S_TIM2,
    S_TIM3,
    S_TIM14,
    S_TIM15,
    S_TIM16,
    S_TIM17,
    NUM_TIMERS
};


//Get timer peripheral struct including bus register, enable mask, timer mask
constexpr RCC_Peripheral getTimerRCC(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return {Bus::APB2, &RCC->APB2ENR, RCC_APB2ENR_TIM1EN};
        case Timer_Key::S_TIM2:  return {Bus::APB1, &RCC->APB1ENR, RCC_APB1ENR_TIM2EN};
        case Timer_Key::S_TIM3:  return {Bus::APB1, &RCC->APB1ENR, RCC_APB1ENR_TIM3EN};
        case Timer_Key::S_TIM14: return {Bus::APB1, &RCC->APB1ENR, RCC_APB1ENR_TIM14EN};
        case Timer_Key::S_TIM15: return {Bus::APB2, &RCC->APB2ENR, RCC_APB2ENR_TIM15EN};
        case Timer_Key::S_TIM16: return {Bus::APB2, &RCC->APB2ENR, RCC_APB2ENR_TIM16EN};
        case Timer_Key::S_TIM17: return {Bus::APB2, &RCC->APB2ENR, RCC_APB2ENR_TIM17EN};
        case Timer_Key::NUM_TIMERS: return {Bus::INVALID, nullptr, 0};;
    }

    return {Bus::APB2, &RCC->APB2ENR, RCC_APB2ENR_TIM1EN};
}

//Get actual register value based on enum
constexpr volatile TIM_TypeDef* getTimerRegister(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return TIM1;
        case Timer_Key::S_TIM2:  return TIM2;
        case Timer_Key::S_TIM3:  return TIM3;
        case Timer_Key::S_TIM14:  return TIM14;
        case Timer_Key::S_TIM15:  return TIM15;
        case Timer_Key::S_TIM16:  return TIM16;
        case Timer_Key::S_TIM17:  return TIM17;
        case Timer_Key::NUM_TIMERS: return nullptr;
    }
    return TIM1;
}

constexpr IRQn_Type getIRQn(Timer_Key t) {
    switch(t) {
        case Timer_Key::S_TIM1:  return TIM1_BRK_UP_TRG_COM_IRQn;
        case Timer_Key::S_TIM2:  return TIM2_IRQn;
        case Timer_Key::S_TIM3:  return TIM3_IRQn;
        case Timer_Key::S_TIM14: return TIM14_IRQn;
        case Timer_Key::S_TIM15: return TIM15_IRQn;
        case Timer_Key::S_TIM16: return TIM16_IRQn;
        case Timer_Key::S_TIM17: return TIM17_IRQn;
        case Timer_Key::NUM_TIMERS: return TIM1_BRK_UP_TRG_COM_IRQn;
    }
    return TIM1_BRK_UP_TRG_COM_IRQn;
}



#endif