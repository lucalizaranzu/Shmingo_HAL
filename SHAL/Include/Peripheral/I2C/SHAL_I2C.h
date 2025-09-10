//
// Created by Luca on 9/9/2025.
//

#ifndef SHMINGO_HAL_SHAL_I2C_H
#define SHMINGO_HAL_SHAL_I2C_H

#include "SHAL_CORE.h"
#include "SHAL_I2C_REG.h"


class SHAL_I2C{

    friend class I2CManager;

public:

private:

    SHAL_I2C() = default;

    I2C_Pair m_I2CPair = I2C_Pair::INVALID; //Initialize to invalid

};





class I2CManager{

public:

    static SHAL_I2C& get(uint8_t i2c);

    I2CManager() = delete;

private:

    inline static SHAL_I2C m_UARTs[NUM_I2C_BUSES] = {};

};

#endif //SHMINGO_HAL_SHAL_I2C_H
