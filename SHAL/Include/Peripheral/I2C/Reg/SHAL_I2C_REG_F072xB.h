//
// Created by Luca on 9/9/2025.
//

#ifndef SHMINGO_HAL_SHAL_I2C_REG_F072XB_H
#define SHMINGO_HAL_SHAL_I2C_REG_F072XB_H

#include "SHAL_CORE.h"

enum class I2C_Pair : uint8_t{
    //I2C_1
    SCL1B6_SDA1B7, //AF1
    SCL1B8_SDA1B9, //AF1

    //I2C_2
    SCL2B10_SDA2B11, //AF1
    SCL2B13_SDA2B14, //AF5
};

#endif //SHMINGO_HAL_SHAL_I2C_REG_F072XB_H
