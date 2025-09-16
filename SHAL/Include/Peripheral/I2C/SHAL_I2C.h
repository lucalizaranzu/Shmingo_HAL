//
// Created by Luca on 9/9/2025.
//

#ifndef SHMINGO_HAL_SHAL_I2C_H
#define SHMINGO_HAL_SHAL_I2C_H

#include <cstdio>

#include "SHAL_CORE.h"
#include "SHAL_I2C_REG.h"


class SHAL_I2C{

    friend class I2CManager;

public:

    void init(I2C_Pair pair) volatile;

    /// General I2C function to send commands to a device, then read back any returned data if necessary
    /// \param addr address of slave device
    /// \param writeData pointer to array of write commands
    /// \param writeLen number of write commands
    /// \param readData pointer to buffer to write received data to
    /// \param readLen number of bytes to be read
    void masterWriteRead(uint8_t addr,const uint8_t* writeData, size_t writeLen, uint8_t* readData, size_t readLen);

    /// Function to write an array of commands to an I2C device
    /// \param addr Address of slave device
    /// \param writeData Pointer to array of commands
    /// \param writeLen Number of commands
    void masterWrite(uint8_t addr, const uint8_t* writeData, uint8_t writeLen);

    /// Function to read bytes from an I2C device
    /// \param addr Address of slave device
    /// \param readBuffer Pointer to buffer where data will be placed
    /// \param bytesToRead Number of bytes to read
    void masterRead(uint8_t addr, uint8_t* readBuffer, uint8_t bytesToRead);


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
