//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_I2C_REG_L432KC_H
#define SHMINGO_HAL_SHAL_I2C_REG_L432KC_H

#include "SHAL_CORE.h"
#include "SHAL_I2C_TYPES.h"

enum class I2C_Pair : uint8_t{
    //I2C_1
    SCL1A9_SDA1A10, //AF4
    SCL1B6_SDA1B7, //AF4

    //I2C_3
    SCL3A7_SDA3B4, //AF4

    NUM_PAIRS,
    INVALID
};

constexpr SHAL_I2C_Pair getI2CPair(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1A9_SDA1A10: return {I2C1,GPIO_Key::A9,GPIO_Key::A10,GPIO_Alternate_Function::AF4,GPIO_Alternate_Function::AF4};
        case I2C_Pair::SCL1B6_SDA1B7: return {I2C1,GPIO_Key::B6,GPIO_Key::B7,GPIO_Alternate_Function::AF4,GPIO_Alternate_Function::AF4};
        case I2C_Pair::SCL3A7_SDA3B4: return {I2C3,GPIO_Key::A7,GPIO_Key::B4,GPIO_Alternate_Function::AF4,GPIO_Alternate_Function::AF4};
        case I2C_Pair::INVALID:
            assert(false);
            return {nullptr,GPIO_Key::INVALID,GPIO_Key::INVALID,GPIO_Alternate_Function::AF0,GPIO_Alternate_Function::AF0};
    }
    __builtin_unreachable();
}

constexpr SHAL_I2C_Enable_Reg getI2CEnableReg(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1A9_SDA1A10:
        case I2C_Pair::SCL1B6_SDA1B7:
            return {&RCC->APB1ENR1,RCC_APB1ENR1_I2C1EN};
        case I2C_Pair::SCL3A7_SDA3B4:
            return {&RCC->APB1ENR1,RCC_APB1ENR1_I2C3EN};
        case I2C_Pair::NUM_PAIRS:
        case I2C_Pair::INVALID:
            assert(false);
            return {nullptr, 0};
    }
    __builtin_unreachable();
}

constexpr SHAL_I2C_Reset_Reg getI2CResetReg(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1A9_SDA1A10:
        case I2C_Pair::SCL1B6_SDA1B7:
            return {&RCC->APB1RSTR1,RCC_APB1RSTR1_I2C1RST};
        case I2C_Pair::SCL3A7_SDA3B4:
            return {&RCC->APB1RSTR1,RCC_APB1RSTR1_I2C3RST};
        case I2C_Pair::NUM_PAIRS:
        case I2C_Pair::INVALID:
            assert(false);
            return {nullptr, 0};
    }
    __builtin_unreachable();
}

constexpr SHAL_I2C_Reset_Reg getI2CResetRe() {
    return {&RCC->APB1RSTR1,RCC_APB1RSTR1_I2C1RST};
}
//Gets all the bits in the I2C timer register, these values should rarely be manually set, but I wanted to support it anyway
constexpr SHAL_I2C_Timing_Reg getI2CTimerReg(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1A9_SDA1A10:
        case I2C_Pair::SCL1B6_SDA1B7:
            return {&I2C1->TIMINGR,31,23,19,15,7};
        case I2C_Pair::SCL3A7_SDA3B4:
            return {&I2C3->TIMINGR,31,23,19,15,7};
        case I2C_Pair::NUM_PAIRS:
        case I2C_Pair::INVALID:
            assert(false);
            __builtin_unreachable();
    }
    __builtin_unreachable();
}

#endif //SHMINGO_HAL_SHAL_I2C_REG_L432KC_H
