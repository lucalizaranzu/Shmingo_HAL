//
// Created by Luca on 9/21/2025.
//

#include "SHAL_ADC.h"
#include "SHAL_GPIO.h"
#include "SHAL_UART.h"
#include <cstdio>
//Can hard code registers on F0 because all F0 devices have only one ADC, and use only one clock
SHAL_Result SHAL_ADC::init(ADC_Key key) {

    m_ADCKey = key;

    if(!isValid()){
        SHAL_UART2.sendString("Not valid\r\n");
        return SHAL_Result::ERROR;
    }

    SHAL_UART2.sendString("Init called\r\n");
    PIN(B4).toggle();
    SHAL_delay_ms(100);
    PIN(B4).toggle();

    SHAL_ADC_RCC_Enable_Reg clock_reg = getADCRCCEnableRegister(m_ADCKey); //Clock enable

    SHAL_apply_bitmask(clock_reg.reg,clock_reg.mask);

    auto clock_select_register = getADCClockSelectRegister();

    SHAL_set_bits(clock_select_register.reg, 2, static_cast<uint32_t>(ADC_Clock_Source::SHAL_SYSCLK),clock_select_register.offset); //Set ADC clock

    wakeFromDeepSleep();

    if(calibrate() != SHAL_Result::OKAY){ //Calibrate
        SHAL_UART2.sendString("Calibration failed");
        return SHAL_Result::ERROR;
    }

    if(enable() != SHAL_Result::OKAY){
        SHAL_UART2.sendString("Could not enable from init\r\n");
        return SHAL_Result::ERROR;
    }

    configureAlignment(SHAL_ADC_Alignment::RIGHT);
    configureResolution(SHAL_ADC_Resolution::B12);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::calibrate() {
    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey);

    if(disable() != SHAL_Result::OKAY){
        return SHAL_Result::ERROR;
    }

    SHAL_delay_us(1000);

    if ((*control_reg.reg & (control_reg.enable_mask | control_reg.disable_mask)) != 0) {
        return SHAL_Result::ERROR;
    }

    SHAL_clear_bitmask(control_reg.reg, control_reg.differential_mode_mask);

    SHAL_apply_bitmask(control_reg.reg, control_reg.calibration_mask);

    if ((*control_reg.reg & control_reg.calibration_mask) == 0) {
        return SHAL_Result::ERROR;
    }

    if (!SHAL_WAIT_FOR_CONDITION_US(((*control_reg.reg & control_reg.calibration_mask) != 0),500)) { //Wait for conversion
        return SHAL_Result::ERROR; //Failed sequence
    }

    SHAL_UART2.sendString("Calibration OK\r\n");
    return SHAL_Result::OKAY;
}

uint16_t SHAL_ADC::singleConvertSingle(SHAL_ADC_Channel channel, SHAL_ADC_SampleTime time) {

    auto data_reg = getADCDataReg(m_ADCKey); //Where our output will be stored

    auto sampleTimeReg = getADCChannelSamplingTimeRegister(m_ADCKey,channel);

    SHAL_set_bits(sampleTimeReg.reg,3,static_cast<uint8_t>(time),sampleTimeReg.channel_offset); //Set sample time register TODO un-hardcode bit width?

    addADCChannelToSequence(channel,0); //Use index 0 to convert channel
    if(setADCSequenceAmount(1) == SHAL_Result::ERROR){return 0;} //Since we're using single convert, convert 1 channel

    if(enable() != SHAL_Result::OKAY){
        return 0;
    }

    startConversion(); //Start ADC conversion

    auto ISR_reg = getADCISRReg(m_ADCKey);

    if(!SHAL_WAIT_FOR_CONDITION_US(((*ISR_reg.reg & ISR_reg.end_of_sequence_mask) != 0),500)){ //Wait for conversion
        return 0; //Failed
    }

    return *data_reg.reg;
}

SHAL_Result SHAL_ADC::multiConvertSingle(SHAL_ADC_Channel* channels, const int numChannels, uint16_t* result, SHAL_ADC_SampleTime time) {
    auto data_reg = getADCDataReg(m_ADCKey); //Where our output will be stored

    setADCSequenceAmount(numChannels); //Convert the correct amount of channels

    for(int i = 0; i < numChannels; i++){
        auto channel = channels[i];

        auto sampleTimeReg = getADCChannelSamplingTimeRegister(m_ADCKey,channel);

        SHAL_set_bits(sampleTimeReg.reg,3,static_cast<uint8_t>(time),sampleTimeReg.channel_offset); //Set sample time register TODO un-hardcode bit width?

        addADCChannelToSequence(channel,i); //Use index 0 to convert channel

        if(enable() != SHAL_Result::OKAY){
            return SHAL_Result::ERROR;
        }
    }

    startConversion(); //Start ADC conversion

    auto ISR_reg = getADCISRReg(m_ADCKey);

    for(int i = 0; i < numChannels; i++) {
        if (!SHAL_WAIT_FOR_CONDITION_US(((*ISR_reg.reg & ISR_reg.end_of_conversion_mask) != 0),500)) { //Wait for conversion
            return SHAL_Result::ERROR; //Failed conversion
        }
        result[i] = *data_reg.reg;
    }

    if (!SHAL_WAIT_FOR_CONDITION_US(((*ISR_reg.reg & ISR_reg.end_of_sequence_mask) != 0),500)) { //Wait for conversion
        return SHAL_Result::ERROR; //Failed sequence
    }

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::enable() {
    if(!isValid()){
        SHAL_UART2.sendString("Enable failed: Invalid \r\n");
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey);
    SHAL_ADC_ISR_Reg ISR_reg = getADCISRReg(m_ADCKey);

    if(!SHAL_WAIT_FOR_CONDITION_MS((*control_reg.reg & control_reg.calibration_mask) == 0, 100)) {
        return SHAL_Result::ERROR;
    }

    if (*control_reg.reg & control_reg.enable_mask) {
        return SHAL_Result::OKAY;  //Not an error
    }

    if (*control_reg.reg & control_reg.disable_mask) {
        return SHAL_Result::ERROR;
    }

    //Clear ADRDY flag by writing 1 to it
    SHAL_apply_bitmask(ISR_reg.reg, ISR_reg.ready_mask);

    //Enable the ADC by setting ADEN
    SHAL_apply_bitmask(control_reg.reg, control_reg.enable_mask);

    if(!SHAL_WAIT_FOR_CONDITION_MS((*ISR_reg.reg & ISR_reg.ready_mask) != 0, 100)) {
        return SHAL_Result::ERROR;
    }

    //Clear ADRDY again
    SHAL_apply_bitmask(ISR_reg.reg, ISR_reg.ready_mask);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::wakeFromDeepSleep() {
    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey); //ADC Control register

    SHAL_clear_bitmask(control_reg.reg,control_reg.deep_power_down_mask); //Wake ADC from sleep

    SHAL_apply_bitmask(control_reg.reg,control_reg.voltage_regulator_mask);

    SHAL_delay_us(50); //Wait for regulator to stabilize

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::disable() {
    if(!isValid()){
        return SHAL_Result::ERROR;
    }

    auto control_reg = getADCControlReg(m_ADCKey);

    //Stop any ongoing conversion
    if (*control_reg.reg & control_reg.start_mask) {
        SHAL_apply_bitmask(control_reg.reg, control_reg.stop_mask);
    }

    //Only disable if ADC is enabled otherwise it hangs
    if (*control_reg.reg & control_reg.enable_mask) {
        SHAL_apply_bitmask(control_reg.reg, control_reg.disable_mask);

        if (!SHAL_WAIT_FOR_CONDITION_MS(((*control_reg.reg & (control_reg.enable_mask | control_reg.disable_mask)) == 0),500)){
            return SHAL_Result::ERROR;
        }
    }

    return SHAL_Result::OKAY;
}


SHAL_Result SHAL_ADC::startConversion() {
    auto control_reg = getADCControlReg(m_ADCKey);

    SHAL_apply_bitmask(control_reg.reg,control_reg.start_mask);

    return SHAL_Result::OKAY;
}

bool SHAL_ADC::isValid() {
    if(m_ADCKey == ADC_Key::INVALID || m_ADCKey == ADC_Key::NUM_ADC){
        return false;
    }
    return true;
}

SHAL_Result SHAL_ADC::configureResolution(SHAL_ADC_Resolution resolution) {
    if(!isValid()){
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Config_Reg config_reg = getADCConfigReg(m_ADCKey);

    SHAL_set_bits(config_reg.reg,2,static_cast<uint8_t>(resolution),config_reg.resolution_offset);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::configureAlignment(SHAL_ADC_Alignment alignment) {
    if(!isValid()){
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Config_Reg config_reg = getADCConfigReg(m_ADCKey);

    //TODO check if this needs to be abstracted (Do other platforms have >2 resolution possibilities?
    SHAL_set_bits(config_reg.reg,1,static_cast<uint8_t>(alignment),config_reg.alignment_offset);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::setADCSequenceAmount(uint32_t amount) {
    if(!isValid()){return SHAL_Result::ERROR;}

    if(amount == 0){
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Sequence_Amount_Reg sequence_amount_reg = getADCSequenceAmountRegister(m_ADCKey);

    SHAL_set_bits(sequence_amount_reg.reg, 4, amount - 1, sequence_amount_reg.offset);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::addADCChannelToSequence(SHAL_ADC_Channel channel, uint32_t index) {

    if(!isValid()){return SHAL_Result::ERROR;}

    auto sequenceRegisters = getADCSequenceRegisters(m_ADCKey);

    auto channelNum = static_cast<uint8_t>(channel);

    uint32_t bitSection = (index + 1) % 5; //Need a new variable since SQR1 has its data bits shifted up by one section to make room for the L section
    uint32_t sequenceRegNumber = (index + 1) / 5;

    volatile uint32_t* sequenceReg = sequenceRegisters.regs[sequenceRegNumber];
    uint32_t bitSectionOffset = sequenceRegisters.offsets[bitSection];

    if(sequenceRegNumber != 0){
        *sequenceReg = 0; //Clear previous conversions
    }
    else{
        *sequenceReg &= 0x0000000F;
    }

    SHAL_set_bits(sequenceReg,5,channelNum,bitSectionOffset);

    return SHAL_Result::OKAY;
}




SHAL_ADC &ADCManager::get(ADC_Key key) {
    return m_ADCs[static_cast<uint8_t>(key)];
}

SHAL_ADC& ADCManager::getByIndex(int index) {

    if(index < static_cast<int>(ADC_Key::NUM_ADC)){
        return m_ADCs[index];
    }
    return m_ADCs[0];
}