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

    void init(I2C_Pair pair) volatile;

    ///
    /// \param addr I2C address of slave device
    /// \param reg Address of register in slave device to write to
    /// \param data Data to write to slave register
    void masterTransmit(uint8_t addr, uint8_t reg, uint8_t data);

    ///
    /// \param addr I2C address of slave device
    /// \param reg Register to read data from
    uint8_t masterReceive(uint8_t addr, uint8_t reg);

    //Manually set the clock configuration. Refer to your MCU's reference manual for examples
    void setClockConfig(uint8_t prescaler, uint8_t dataSetupTime, uint8_t dataHoldTime, uint8_t SCLHighPeriod, uint8_t SCLLowPeriod);

    //Set clock configuration based on a value calculated from STM32CubeMX, or other similar tools
    void setClockConfig(uint32_t configuration);

private:

    SHAL_I2C() = default;

    I2C_Pair m_I2CPair = I2C_Pair::INVALID; //Initialize to invalid

};

#define I2C(num) I2CManager::get(num)

class I2CManager{

public:

    static SHAL_I2C& get(uint8_t i2c);

    I2CManager() = delete;

private:

    inline static SHAL_I2C m_I2CBuses[NUM_I2C_BUSES] = {};

};

#endif //SHMINGO_HAL_SHAL_I2C_H
