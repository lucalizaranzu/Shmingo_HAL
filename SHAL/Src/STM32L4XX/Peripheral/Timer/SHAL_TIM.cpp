//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM.h"
#include <cassert>

Timer::Timer(Timer_Key key) : m_key(key){

}

Timer::Timer() : m_key(Timer_Key::S_TIM_INVALID){

}

void Timer::start() {

    auto control_reg = getTimerControlRegister1(m_key);
    auto event_generation_reg = getTimerEventGenerationRegister(m_key);

    SHAL_apply_bitmask(control_reg.reg, control_reg.counter_enable_mask); //Enable counter
    SHAL_apply_bitmask(event_generation_reg.reg, event_generation_reg.update_generation_mask);

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
    NVIC_EnableIRQ(getTimerIRQn(TIMER_KEY));
}

void Timer::init(uint32_t prescaler, uint32_t autoReload) {
    TIM_RCC_Enable rcc = getTimerRCC(TIMER_KEY);
    *rcc.busEnableReg |= (1 << rcc.offset);

    setPrescaler(prescaler);
    setARR(autoReload);
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


