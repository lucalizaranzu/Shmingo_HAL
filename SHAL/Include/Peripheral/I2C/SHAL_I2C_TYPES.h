//
// Created by Luca on 9/9/2025.
//

#ifndef SHMINGO_HAL_SHAL_I2C_TYPES_H
#define SHMINGO_HAL_SHAL_I2C_TYPES_H

#include "SHAL_CORE.h"
#include "SHAL_GPIO_REG.h"

//Information necessary for I2C
struct SHAL_I2C_Pair {
    I2C_TypeDef* I2CReg;
    GPIO_Key SCL_Key;
    GPIO_Key SDA_Key;
    GPIO_Alternate_Function SCL_Mask;
    GPIO_Alternate_Function SDA_Mask;
};

struct SHAL_I2C_Enable_Reg{
    volatile uint32_t* reg;
    uint32_t mask;
};

struct SHAL_I2C_Reset_Reg{
    volatile uint32_t* reg;
    uint32_t mask;
};

//Manual values for I2C timer register
struct SHAL_I2C_Timing_Reg{
    volatile uint32_t* reg;
    uint8_t prescaler_offset;
    uint8_t dataSetupTime_offset;
    uint8_t dataHoldTime_offset;
    uint8_t SCLHighPeriod_offset;
    uint8_t SCLLowPeriod_offset;
};

#endif //SHMINGO_HAL_SHAL_I2C_TYPES_H
