//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM.h"

Timer::Timer(Timer_Key t) : timer(t), timer_reg(getTimerRegister(t)){
    RCC_Peripheral rcc = getTimerRCC(timer);
    *rcc.reg |= rcc.bitmask;
}

void Timer::start() {
    timer_reg->CR1 |= TIM_CR1_CEN;
}

void Timer::stop() {
    timer_reg->CR1 &= ~TIM_CR1_CEN;
}

void Timer::setPrescaler(uint16_t presc) {
    timer_reg->PSC = presc;
}

void Timer::setARR(uint16_t arr) {
    timer_reg->ARR = arr;
}

void Timer::enableInterrupt() {
    timer_reg->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(getIRQn(timer));
}


