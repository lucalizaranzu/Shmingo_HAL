//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM.h"
#include <cassert>

Timer::Timer(Timer_Key t) : m_key(t){

}

Timer::Timer() : m_key(Timer_Key::S_TIM_INVALID){

}

void Timer::start() {

    auto control_reg = getTimerControlRegister1(m_key);
    auto event_generation_reg = getTimerEventGenerationRegister(m_key);
    auto status_reg = getTimerStatusRegister(m_key);
    auto break_time_dead_reg = getTimerBreakDeadTimeRegister(m_key);

    auto rcc_reg = getTimerRCC(m_key);

    SHAL_apply_bitmask(control_reg.reg, control_reg.counter_enable_mask); //Enable counter
    SHAL_apply_bitmask(control_reg.reg, control_reg.auto_reload_preload_enable_mask); //Preload enable (buffer)
    SHAL_apply_bitmask(event_generation_reg.reg, event_generation_reg.update_generation_mask);

    SHAL_clear_bitmask(status_reg.reg,status_reg.update_interrupt_flag_mask);

    SHAL_apply_bitmask(rcc_reg.reg,rcc_reg.enable_mask);
    SHAL_apply_bitmask(break_time_dead_reg.reg,break_time_dead_reg.main_output_enable_mask);

    enableInterrupt();
}

void Timer::stop() {
    auto rcc_reg = getTimerRCC(m_key);

    SHAL_clear_bitmask(rcc_reg.reg,rcc_reg.enable_mask);
}

void Timer::setPrescaler(const uint16_t presc) const {
    auto prescalerReg = getTimerPrescalerRegister(m_key);

    SHAL_set_bits(prescalerReg.reg,16,presc,0);
}

void Timer::setARR(const uint16_t arr) const {
    auto autoReloadReg = getTimerAutoReloadRegister(m_key);

    SHAL_set_bits(autoReloadReg.reg,16,arr,0);}

void Timer::enableInterrupt() {
    getTimerRegister(m_key)->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(getIRQn(m_key));
}

void Timer::init(uint16_t prescaler, uint16_t autoReload) {
    SHAL_TIM_RCC_Register rcc = getTimerRCC(m_key);

    SHAL_apply_bitmask(rcc.reg,rcc.enable_mask);

    setPrescaler(prescaler);
    setARR(autoReload);
}

void Timer::configurePWM(SHAL_Timer_Channel channel, uint16_t prescaler, uint16_t autoReload, uint16_t captureCompareThreshold) {

    setPrescaler(prescaler);
    setARR(autoReload);

    setOutputCompareMode(channel, SHAL_TIM_Output_Compare_Mode::PWMMode1);
    enableChannel(channel,SHAL_Timer_Channel_Main_Output_Mode::Polarity_Normal,SHAL_Timer_Channel_Complimentary_Output_Mode::Disabled);

    setCaptureCompareValue(channel, captureCompareThreshold);
}

void Timer::configureOneshot(SHAL_Timer_Channel channel, uint16_t prescaler, uint16_t autoReload, uint16_t captureCompareThreshold) {

    setPrescaler(prescaler);
    setARR(autoReload);

    setOutputCompareMode(channel, SHAL_TIM_Output_Compare_Mode::Toggle);
    enableChannel(channel,SHAL_Timer_Channel_Main_Output_Mode::Polarity_Normal,SHAL_Timer_Channel_Complimentary_Output_Mode::Disabled);

    setCaptureCompareValue(channel, captureCompareThreshold);
}

void Timer::setOutputCompareMode(SHAL_Timer_Channel channel, SHAL_TIM_Output_Compare_Mode outputCompareMode) {

    auto channelNum = static_cast<uint8_t>(channel);

    auto CCMR = getTimerOutputCaptureCompareModeRegister(m_key, channel);

    uint32_t OCMR_Offset = channelNum % 2 == 1 ? CCMR.output_compare_1_mode_offset : CCMR.output_compare_2_mode_offset;

    SHAL_set_bits(CCMR.reg,3,static_cast<uint8_t>(outputCompareMode),OCMR_Offset);
}

void Timer::enableChannel(SHAL_Timer_Channel channel, SHAL_Timer_Channel_Main_Output_Mode mainOutputMode,
                          SHAL_Timer_Channel_Complimentary_Output_Mode complimentaryOutputMode) {

    SHAL_TIM_Capture_Compare_Enable_Register captureCompareEnableReg = getTimerCaptureCompareEnableRegister(m_key, channel);

    uint16_t setValue = 0; //Value to set the register as
    auto channelNum = static_cast<uint8_t>(channel);

    uint8_t channelStride = 4; //4 bits per field

    setValue |= (static_cast<uint8_t>(mainOutputMode) << ((channelNum - 1) * channelStride)); //xxBB shifted by c - 1
    setValue |= (static_cast<uint8_t>(complimentaryOutputMode) << (((channelNum - 1) * channelStride) + 2)); //BBxx shifted by c - 1

    SHAL_set_bits(captureCompareEnableReg.reg,16,setValue,0);
}

void Timer::setCaptureCompareValue(SHAL_Timer_Channel channel, uint16_t value) {
    auto captureCompareReg = getTimerCaptureCompareRegister(m_key,channel);

    SHAL_set_bits(captureCompareReg.reg,16,value,0);
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