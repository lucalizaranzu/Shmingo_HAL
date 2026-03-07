//
// Created by Luca on 9/21/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_REG_F072XB_H
#define SHMINGO_HAL_SHAL_ADC_REG_F072XB_H

#include "SHAL_CORE.h"
#include "SHAL_ADC_TYPES.h"

enum class SHAL_ADC_Channel : uint32_t { //TODO remove unused or non existing channels?
    CH0 = 0,
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
    CH16,
    CHTemp,
    CHRef,
    CHBat,
    NO_ADC_MAPPING
};

#define SHAL_ADC1 SHAL_ADC(1)

enum class ADC_Key : uint8_t{
    S_ADC1,
    NUM_ADC,
    INVALID
};

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

#endif //SHMINGO_HAL_SHAL_ADC_REG_F072XB_H
