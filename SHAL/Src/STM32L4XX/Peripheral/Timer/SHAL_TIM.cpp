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
    SHAL_apply_bitmask(control_reg.reg, control_reg.auto_reload_preload_enable_mask); //Preload enable (buffer)
    SHAL_apply_bitmask(event_generation_reg.reg, event_generation_reg.update_generation_mask);

    enableInterrupt();
}

void Timer::stop() {
    auto control_reg = getTimerControlRegister1(m_key);
    SHAL_clear_bitmask(control_reg.reg, control_reg.counter_enable_mask); //Enable counter
}

void Timer::setPrescaler(uint16_t presc) {
    auto prescaler_reg = getTimerPrescalerRegister(m_key);
    SHAL_set_register_value(prescaler_reg.reg,presc);
}

void Timer::setARR(uint16_t arr) {
    auto autoreload_reg = getTimerAutoReloadRegister(m_key);
    SHAL_set_register_value(autoreload_reg.reg,arr);
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

void Timer::setPWMMode(SHAL_Timer_Channel channel, SHAL_TIM_Output_Compare_Mode outputCompareMode, SHAL_Timer_Channel_Main_Output_Mode mainOutputMode,
                       SHAL_Timer_Channel_Complimentary_Output_Mode complimentaryOutputMode) {

    auto ccer = getTimerCaptureCompareEnableRegister(m_key);
    auto ccmr1 = getTimerCaptureCompareModeRegistersOutput(m_key);
    auto bdtr = getTimerBreakDeadTimeRegister(m_key);

    uint8_t fullChannelModeMask = static_cast<uint8_t>(mainOutputMode) | (static_cast<uint8_t>(complimentaryOutputMode) << 2);
    uint8_t channelNum = static_cast<uint8_t>(channel);

    if (channelNum <= 3) {

        uint32_t regNum = channelNum / 2; //TODO change later for support for channels 5 and 6

        if (channelNum % 2 == 1) {
            SHAL_set_bits(ccmr1.regs[regNum], 4, static_cast<uint8_t>(outputCompareMode),
                          ccmr1.output_compare_2_mode_offset);
        } else {
            SHAL_set_bits(ccmr1.regs[regNum], 4, static_cast<uint8_t>(outputCompareMode),
                          ccmr1.output_compare_1_mode_offset);
        }
    }

    uint32_t offset = channelNum * 4;

    if (static_cast<uint8_t>(m_key) > 3) {
        fullChannelModeMask &= (0b0011); //Clear bits for complimentary output since channels 4,5,6 don't support it
    }

    SHAL_set_bits(ccer.reg, 4, fullChannelModeMask, offset);
    SHAL_apply_bitmask(bdtr.reg, bdtr.main_output_enable_mask);
}

void Timer::setPWMDutyCycle(uint32_t dutyCycle) {
    auto reg = getTimerCaptureCompareRegister(m_key);
    SHAL_set_bits(reg.reg,16,dutyCycle,0);
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