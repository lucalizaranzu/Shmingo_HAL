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
    AF_Mask SCL_Mask;
    AF_Mask SDA_Mask;
};

struct SHAL_I2C_Enable_REG{
    volatile uint32_t* reg;
    uint32_t mask;
};

#endif //SHMINGO_HAL_SHAL_I2C_TYPES_H
