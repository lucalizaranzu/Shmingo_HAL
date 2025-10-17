//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_REG_L432KC_H
#define SHMINGO_HAL_SHAL_ADC_REG_L432KC_H

#include "SHAL_CORE.h"
#include "SHAL_ADC_TYPES.h"

#define SHAL_ADC1 SHAL_ADC(1)



#define NUM_ADCS 1
#define NUM_ADC_CHANNELS 16

enum class SHAL_ADC_Channel : uint32_t {
    CH0,
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
    CH8,
    CH9,
    CH10,
    CH11,
    CH12,
    CH13,
    CH14,
    CH15,
    CHTemp,
    CHRef,
    CHBat
};

enum class ADC_Key : uint8_t{
    S_ADC1 = 0,
    NUM_ADC = 1,
    INVALID = 255
};

enum class ADC_Clock_Source : uint8_t {
    SHAL_SYSCLK,
    SHAL_PLLSAI1,
    SHAL_PLL,
    SHAL_MSI
};

static volatile ADC_TypeDef* ADC_TABLE[1] = { //Lookup table for ADCs
        ADC1,
};

SHAL_ADC_Common_Control_Reg getADCCommonControl() {
    return {&ADC1_COMMON->CCR ,ADC_CCR_VREFEN,ADC_CCR_TSEN,ADC_CCR_VBATEN};
}

SHAL_ADC_RCC_Enable_Reg getADCRCCEnableRegister(ADC_Key key){
    SHAL_ADC_RCC_Enable_Reg res = {nullptr, RCC_AHB2ENR_ADCEN};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->ISR);
    return res;
}

SHAL_ADC_Control_Reg getADCControlReg(ADC_Key key) {

    SHAL_ADC_Control_Reg res = {nullptr, ADC_CR_ADEN, ADC_CR_ADDIS, ADC_CR_ADCAL, ADC_CR_ADSTART};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->CR);
    return res;
}

SHAL_ADC_Config_Reg getADCConfigReg(ADC_Key key) {

    SHAL_ADC_Config_Reg res = {nullptr, ADC_CFGR_CONT, ADC_CFGR_RES_Pos, ADC_CFGR_ALIGN_Pos};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->CFGR);
    return res;
}

SHAL_ADC_ISR_Reg getADCISRReg(ADC_Key key){
    SHAL_ADC_ISR_Reg res = {nullptr, ADC_ISR_EOC, ADC_ISR_EOS, ADC_ISR_ADRDY};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->ISR);
    return res;
}

SHAL_ADC_Data_Reg getADCDataReg(ADC_Key key){
    SHAL_ADC_Data_Reg res = {nullptr, 0xFFFF};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->DR);
    return res;
}

SHAL_ADC_Clock_Reg getADCClockSelectRegister(ADC_Clock_Source clockSource) {
    SHAL_ADC_Clock_Reg res = {&RCC->CCIPR, RCC_CCIPR_ADCSEL_Msk, 1U << RCC_CCIPR_ADCSEL_Pos}; //Default to PLLSAI1

    switch(clockSource){
        case ADC_Clock_Source::SHAL_PLLSAI1:
            res.mask = 1U << RCC_CCIPR_ADCSEL_Pos;
        case ADC_Clock_Source::SHAL_PLL:
            res.mask = 2U << RCC_CCIPR_ADCSEL_Pos;
        case ADC_Clock_Source::SHAL_SYSCLK:
            res.mask = 3U << RCC_CCIPR_ADCSEL_Pos;
        case ADC_Clock_Source::SHAL_MSI:
            break; //TODO implement this
    }
    return res;
}

SHAL_ADC_Channel_Sampling_Time_Reg getADCChannelSamplingTimeRegister(ADC_Key key, SHAL_ADC_Channel channel){
    volatile ADC_TypeDef* ADCReg = ADC_TABLE[static_cast<uint8_t>(key)];

    volatile uint32_t* SMPReg = nullptr;
    uint32_t pos;

    auto channelNum = static_cast<uint8_t>(channel);

    if (channelNum <= 9) {
        SMPReg = &ADCReg->SQR1;
        pos = (channelNum * 3);
    } else {
        SMPReg = &ADCReg->SQR2;
        pos = ((channelNum - 10) * 3);
    }

    return {SMPReg, pos};
}

SHAL_ADC_Sequence_Amount_Reg getADCSequenceAmountRegister(ADC_Key key){
    SHAL_ADC_Sequence_Amount_Reg res = {nullptr, ADC_SQR1_L_Pos};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->SQR1);
    return res;
}

SHAL_ADC_Sequence_Reg getADCSequenceRegisters(ADC_Key key){

    volatile ADC_TypeDef* adc_reg = ADC_TABLE[static_cast<uint8_t>(key)];


    SHAL_ADC_Sequence_Reg res = {{&adc_reg->SQR1,
                                 &adc_reg->SQR2,
                                 &adc_reg->SQR3,
                                 &adc_reg->SQR4,
                                 nullptr,
                                 nullptr},
                                 {1UL << 0,
                                 1UL << 6,
                                 1UL << 12,
                                 1UL << 18,
                                 1UL << 24}
    };

    return res;
}

constexpr ADC_TypeDef* getADCRegister(ADC_Key key){
    switch(key){
        case ADC_Key::S_ADC1:
            return ADC1;

        case ADC_Key::NUM_ADC:
        case ADC_Key::INVALID:
            return nullptr;
    }
    __builtin_unreachable();
}

#endif //SHMINGO_HAL_SHAL_ADC_REG_L432KC_H
