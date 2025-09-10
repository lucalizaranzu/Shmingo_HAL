//
// Created by Luca on 9/9/2025.
//

#ifndef SHAL_I2C_REG_F072XB_H
#define SHAL_I2C_REG_F072XB_H

#include "SHAL_CORE.h"
#include <cassert>

#include "SHAL_I2C_TYPES.h"

#define NUM_I2C_BUSES 2


enum class I2C_Pair : uint8_t{
    //I2C_1
    SCL1B6_SDA1B7, //AF1
    SCL1B8_SDA1B9, //AF1

    //I2C_2
    SCL2B10_SDA2B11, //AF1
    SCL2B13_SDA2B14, //AF5

    NUM_PAIRS,
    INVALID
};

constexpr SHAL_I2C_Pair getI2CPair(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1B6_SDA1B7: return {I2C1,GPIO_Key::B6,GPIO_Key::B7,GPIO_Alternate_Function::AF1,GPIO_Alternate_Function::AF1};
        case I2C_Pair::SCL1B8_SDA1B9: return {I2C1,GPIO_Key::B8,GPIO_Key::B9,GPIO_Alternate_Function::AF1,GPIO_Alternate_Function::AF1};
        case I2C_Pair::SCL2B10_SDA2B11: return {I2C2,GPIO_Key::B10,GPIO_Key::B11,GPIO_Alternate_Function::AF1,GPIO_Alternate_Function::AF1};
        case I2C_Pair::SCL2B13_SDA2B14: return {I2C2,GPIO_Key::B13,GPIO_Key::B14,GPIO_Alternate_Function::AF5,GPIO_Alternate_Function::AF5};
        case I2C_Pair::NUM_PAIRS:
        case I2C_Pair::INVALID:
            assert(false);
            return {nullptr,GPIO_Key::INVALID,GPIO_Key::INVALID,GPIO_Alternate_Function::AF0,GPIO_Alternate_Function::AF0};
    }
    __builtin_unreachable();
}

constexpr SHAL_I2C_Enable_REG getI2CEnableReg(const I2C_Pair pair){
    switch(pair){
        case I2C_Pair::SCL1B6_SDA1B7:
        case I2C_Pair::SCL1B8_SDA1B9:
            return {&RCC->APB1ENR,RCC_APB1ENR_I2C1EN};
        case I2C_Pair::SCL2B10_SDA2B11:
        case I2C_Pair::SCL2B13_SDA2B14:
            return {&RCC->APB1ENR,RCC_APB1ENR_I2C2EN};
        case I2C_Pair::NUM_PAIRS:
        case I2C_Pair::INVALID:
            assert(false);
            return {nullptr, 0};
    }
    __builtin_unreachable();
}

#endif //SHAL_I2C_REG_F072XB_H
