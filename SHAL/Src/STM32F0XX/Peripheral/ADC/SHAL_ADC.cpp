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


    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //Enable clock
    RCC->CR2 |= RCC_CR2_HSI14ON; //Start peripheral oscillator

    if(!SHAL_WAIT_FOR_CONDITION_US(((RCC->CR2 & RCC_CR2_HSI14RDY) != 0),50)){ //Wait for clock OKAY
        return SHAL_Result::ERROR;
    }

    if((ADC_reg->ISR & ADC_ISR_ADRDY) != 0){ //Set ADRDY to 0
        ADC_reg->ISR |= ADC_ISR_ADRDY;
    }

    ADC_reg->CR |= ADC_CR_ADEN; //Enable

    if(!SHAL_WAIT_FOR_CONDITION_US(((ADC_reg->ISR & ADC_ISR_ADRDY) != 0),50)){ //Wait for disable
        return SHAL_Result::ERROR;
    }

    if(calibrate() != SHAL_Result::OKAY){ //Calibrate
        return SHAL_Result::ERROR;
    }

    return SHAL_Result::OKAY;
}

SHAL_Result SHAL_ADC::calibrate() {

    if(m_ADCKey == ADC_Key::INVALID || m_ADCKey == ADC_Key::NUM_ADC){
        return SHAL_Result::ERROR;
    }

    ADC_TypeDef* ADC_reg = getADCRegister(m_ADCKey);

    if((ADC_reg->CR & ADC_CR_ADEN) != 0){ //Clear ADEN (enable)
        ADC_reg->CR |= ADC_CR_ADDIS;
    }

    if(!SHAL_WAIT_FOR_CONDITION_US(((ADC_reg->CR & ADC_CR_ADEN) == 0),50)){ //Wait for disable
        return SHAL_Result::ERROR;
    }

    ADC_reg->CFGR1 &= ~ADC_CFGR1_DMAEN; //Clear DMAEN
    ADC_reg->CR |= ADC_CR_ADCAL; //Launch calibration by setting ADCAL

    if(!SHAL_WAIT_FOR_CONDITION_US(((ADC_reg->CR & ADC_CR_ADCAL) == 0),50)){ //Wait for calibration
        return SHAL_Result::ERROR;
    }

    return SHAL_Result::OKAY;
}

uint16_t SHAL_ADC::singleConvertSingle(ADC_Channel channel, ADC_SampleTime time) {

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

void SHAL_ADC::multiConvertSingle(ADC_Channel* channels, const int numChannels, uint16_t* result, ADC_SampleTime time) {
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

SHAL_ADC &ADCManager::get(ADC_Key key) {
    return m_ADCs[static_cast<uint8_t>(key)];
}

SHAL_ADC& ADCManager::getByIndex(int index) {

    if(index < static_cast<int>(ADC_Key::NUM_ADC)){
        return m_ADCs[index];
    }
    return m_ADCs[0];
}
