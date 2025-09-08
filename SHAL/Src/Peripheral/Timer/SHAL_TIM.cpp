//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM.h"
#include <cassert>

Timer::Timer(Timer_Key t) : TIMER_KEY(t){
    TIM_RCC_Enable rcc = getTimerRCC(TIMER_KEY);
    *rcc.busEnableReg |= (1 << rcc.offset);
}

Timer::Timer() : TIMER_KEY(Timer_Key::S_TIM_INVALID){

}

void Timer::start() {
    getTimerRegister(TIMER_KEY)->CR1 |= TIM_CR1_CEN;
    getTimerRegister(TIMER_KEY)->EGR |= TIM_EGR_UG; //load prescaler reg and ARR
    enableInterrupt();
}

void Timer::stop() {
    getTimerRegister(TIMER_KEY)->CR1 &= ~TIM_CR1_CEN;
}

void Timer::setPrescaler(uint16_t presc) {
    getTimerRegister(TIMER_KEY)->PSC = presc;
}

void Timer::setARR(uint16_t arr) {
    getTimerRegister(TIMER_KEY)->ARR = arr;
}

void Timer::enableInterrupt() {
    getTimerRegister(TIMER_KEY)->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(getIRQn(TIMER_KEY));
}


Timer &TimerManager::get(Timer_Key timer_key) {

    //Ensure that we don't try to get invalid timers
    assert(timer_key != Timer_Key::S_TIM_INVALID && timer_key != Timer_Key::NUM_TIMERS);

    Timer& selected = timers[static_cast<int>(timer_key)];

    //Timer queried is not initialized yet (defaults to invalid)
    if(selected.TIMER_KEY == Timer_Key::S_TIM_INVALID){
        timers[static_cast<int>(timer_key)] = Timer(timer_key); //Initialize TIMER_KEY
    }

    return timers[static_cast<int>(timer_key)];
}
