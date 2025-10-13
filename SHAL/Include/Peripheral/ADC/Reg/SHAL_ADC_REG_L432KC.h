//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_REG_L432KC_H
#define SHMINGO_HAL_SHAL_ADC_REG_L432KC_H

#include "SHAL_CORE.h"
#include "SHAL_ADC_TYPES.h"

#define SHAL_ADC1 SHAL_ADC(1)


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

SHAL_ADC_ISR getADCISR(ADC_Key key){
    SHAL_ADC_ISR res = {nullptr, ADC_ISR_EOC};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->ISR);
    return res;
}

SHAL_ADC_Data_Reg getADCDataReg(ADC_Key key){
    SHAL_ADC_Data_Reg res = {nullptr, 0xFFFF};

    res.reg = &(ADC_TABLE[static_cast<uint8_t>(key)]->DR);
    return res;
}

SHAL_ADC_Clock_Reg getADCClockSelectRegister(ADC_Clock_Source clockSource) {
    constexpr uint32_t ADCSEL_MASK = RCC_CCIPR_ADCSEL_Msk; // covers bits 29:28

    SHAL_ADC_Clock_Reg res = {&RCC->CCIPR, ADCSEL_MASK, 1U << RCC_CCIPR_ADCSEL_Pos}; //Default to PLLSAI1

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
