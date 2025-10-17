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
    auto control_reg = getTimerControlRegister1(m_key);
    SHAL_clear_bitmask(control_reg.reg, control_reg.counter_enable_mask); //Enable counter
}

void Timer::setPrescaler(uint16_t presc) {
    auto prescaler_reg = getTimerPrescalerRegister(m_key);
    SHAL_set_bits(prescaler_reg.reg, 16, presc, prescaler_reg.offset);
}

void Timer::setARR(uint16_t arr) {
    auto autoreload_reg = getTimerAutoReloadRegister(m_key);
    SHAL_set_bits(autoreload_reg.reg, 16, arr, autoreload_reg.offset);
}

void Timer::enableInterrupt() {
    auto dma_ier = getTimerDMAInterruptEnableRegister(m_key);
    SHAL_apply_bitmask(dma_ier.reg,dma_ier.update_interrupt_enable_mask);

    NVIC_EnableIRQ(getTimerIRQn(m_key)); //Enable the IRQn in the NVIC
}

void Timer::init(uint32_t prescaler, uint32_t autoReload) {
    SHAL_TIM_RCC_Register rcc = getTimerRCC(m_key);
    SHAL_apply_bitmask(rcc.reg,rcc.enable_mask);

    setPrescaler(prescaler);
    setARR(autoReload);
}


Timer &TimerManager::get(Timer_Key timer_key) {

    //Ensure that we don't try to get invalid timers
    assert(timer_key != Timer_Key::S_TIM_INVALID && timer_key != Timer_Key::NUM_TIMERS);

    Timer& selected = timers[static_cast<int>(timer_key)];

    //Timer queried is not initialized yet (defaults to invalid)
    if(selected.m_key == Timer_Key::S_TIM_INVALID){
        timers[static_cast<int>(timer_key)] = Timer(timer_key); //Initialize TIMER_KEY
    }

    return timers[static_cast<int>(timer_key)];
}