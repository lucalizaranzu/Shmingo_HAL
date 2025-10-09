//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_REG_L432KC_H
#define SHMINGO_HAL_SHAL_ADC_REG_L432KC_H

#include "SHAL_CORE.h"
#include "SHAL_ADC_TYPES.h"

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

#endif //SHMINGO_HAL_SHAL_ADC_REG_L432KC_H
