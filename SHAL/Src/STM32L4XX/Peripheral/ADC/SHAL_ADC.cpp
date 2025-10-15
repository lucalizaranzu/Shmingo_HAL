//
// Created by Luca on 9/21/2025.
//

#include "SHAL_ADC.h"

//Can hard code registers on F0 because all F0 devices have only one ADC, and use only one clock
SHAL_Result SHAL_ADC::init() {

    if(m_ADCKey == ADC_Key::INVALID || m_ADCKey == ADC_Key::NUM_ADC){
        return SHAL_Result::ERROR;
    }

    ADC_TypeDef* ADC_reg = getADCRegister(m_ADCKey);

    SHAL_ADC_RCC_Enable_Reg clock_reg = getADCRCCEnableRegister(m_ADCKey); //Clock enable

    *clock_reg.reg |= clock_reg.mask;

    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey);

    if (*control_reg.reg & control_reg.enable_mask) {
        //request disable: ADEN=1 -> set ADDIS to disable
        *control_reg.reg |= control_reg.disable_mask;
        //wait until ADEN cleared (ISR.ADREADY == 0)
        if(!SHAL_WAIT_FOR_CONDITION_MS((*control_reg.reg & control_reg.enable_mask) == 0, 100)){
            return SHAL_Result::ERROR;
        }
    }

    if(calibrate() != SHAL_Result::OKAY){ //Calibrate
        return SHAL_Result::ERROR;
    }

    configureAlignment(SHAL_ADC_Alignment::RIGHT);
    configureResolution(SHAL_ADC_Resolution::B12);

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::calibrate() {

    if(disable() != SHAL_Result::OKAY){ //Disable the ADC
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey);

    *control_reg.reg |= control_reg.calibration_mask;

    if(!SHAL_WAIT_FOR_CONDITION_US(((*control_reg.reg & control_reg.calibration_mask) == 0),500)){ //Wait for calibration
        return SHAL_Result::ERROR;
    }

    return SHAL_Result::OKAY;
}

uint16_t SHAL_ADC::singleConvertSingle(SHAL_ADC_Channel channel, ADC_SampleTime time) {

    ADC_TypeDef* ADC_reg = getADCRegister(m_ADCKey);


    ADC->CCR |= ADC_CCR_VREFEN | ADC_CCR_TSEN; //Enable VREFINT and Temp sensor in global ADC struct

    ADC_reg->CHSELR = static_cast<uint32_t>(channel); //Enable channel for conversion
    ADC_reg->SMPR |= static_cast<uint32_t>(time); //Set sampling time

    if(!SHAL_WAIT_FOR_CONDITION_US(((ADC_reg->ISR & ADC_ISR_EOC) != 0),500)){ //Wait for conversion
        return 0; //Failed
    }

    uint16_t result = ADC_reg->DR;
    return result;
}

void SHAL_ADC::multiConvertSingle(SHAL_ADC_Channel* channels, const int numChannels, uint16_t* result, ADC_SampleTime time) {
    ADC_TypeDef* ADC_reg = getADCRegister(m_ADCKey);

    ADC->CCR |= ADC_CCR_VREFEN | ADC_CCR_TSEN; //Enable VREFINT and Temp sensor in global ADC struct

    for(int i = 0; i < numChannels; i++){ //Enable all channels
        ADC_reg->CHSELR = static_cast<uint32_t>(channels[i]);
    }

    ADC_reg->SMPR |= static_cast<uint32_t>(time); //Set sampling time


    for(int i = 0; i < numChannels; i++){
        if(!SHAL_WAIT_FOR_CONDITION_US(((ADC_reg->ISR & ADC_ISR_EOC) != 0),500)){ //Wait for conversion
            continue; //Failed
        }

        result[i] = ADC_reg->DR;
    }
}

SHAL_Result SHAL_ADC::disable() {

    if(!isValid()){
        return SHAL_Result::ERROR;
    }

    SHAL_ADC_Control_Reg control_reg = getADCControlReg(m_ADCKey);
    if (*control_reg.reg & control_reg.enable_mask) {
        //request disable: ADEN=1 -> set ADDIS to disable
        *control_reg.reg |= control_reg.disable_mask;
        //wait until ADEN cleared (ISR.ADREADY == 0)
        if(!SHAL_WAIT_FOR_CONDITION_MS((*control_reg.reg & control_reg.enable_mask) == 0, 100)){
            return SHAL_Result::ERROR;
        }
    }

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

    *config_reg.reg &= ~(0x1UL << config_reg.alignment_offset); //TODO check if this needs to be abstracted (Do other platforms have >2 resolution possibilities?
    *config_reg.reg |= static_cast<uint8_t>(alignment) << config_reg.alignment_offset;

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