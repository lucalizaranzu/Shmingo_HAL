//
// Created by Luca on 8/28/2025.
//

#include "Core/Include/Timer/SHAL_TIM.h"
#include <cassert>

Timer::Timer(Timer_Key t) : timer(t), timer_reg(getTimerRegister(t)){
    RCC_Peripheral rcc = getTimerRCC(timer);
    *rcc.reg |= rcc.bitmask;
}

Timer::Timer() : timer(Timer_Key::S_TIM_INVALID), timer_reg(nullptr){

}

void Timer::start() {
    timer_reg->CR1 |= TIM_CR1_CEN;
    timer_reg->EGR |= TIM_EGR_UG; //load prescaler reg and ARR
    enableInterrupt();
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


Timer &TimerManager::get(Timer_Key timer_key) {

    //Ensure that we don't try to get invalid timers
    assert(timer_key != Timer_Key::S_TIM_INVALID && timer_key != Timer_Key::NUM_TIMERS);

    Timer& selected = timers[static_cast<int>(timer_key)];

    //Timer queried is not initialized yet (defaults to invalid)
    if(selected.timer == Timer_Key::S_TIM_INVALID){
        timers[static_cast<int>(timer_key)] = Timer(timer_key); //Initialize timer
    }

    return timers[static_cast<int>(timer_key)];
}
