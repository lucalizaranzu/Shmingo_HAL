//
// Created by Luca on 9/9/2025.
//

#include "SHAL_I2C.h"
#include "SHAL_GPIO.h"

void SHAL_I2C::init(I2C_Pair pair) volatile {
    m_I2CPair = pair;

    SHAL_I2C_Pair I2CPair = getI2CPair(pair); //Get the UART_PAIR information to be initialized

    //Get the SHAL_GPIO pins for this SHAL_I2C setup
    GPIO_Key SCL_Key = I2CPair.SCL_Key; //SCL pin
    GPIO_Key SDA_Key = I2CPair.SDA_Key; //SDA pin

    GET_GPIO(SCL_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE); //Implicitly initializes and enables GPIO bus
    GET_GPIO(SDA_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    GET_GPIO(SCL_Key).setAlternateFunction(I2CPair.SCL_Mask);
    GET_GPIO(SDA_Key).setAlternateFunction(I2CPair.SDA_Mask);

    //These may be abstracted further to support multiple I2C configurations
    GET_GPIO(SCL_Key).setPinType(PinType::OPEN_DRAIN);
    GET_GPIO(SDA_Key).setPinType(PinType::OPEN_DRAIN);

    GET_GPIO(SCL_Key).setOutputSpeed(OutputSpeed::HIGH_SPEED);
    GET_GPIO(SDA_Key).setOutputSpeed(OutputSpeed::HIGH_SPEED);

    GET_GPIO(SCL_Key).setInternalResistor(InternalResistorType::PULLUP);
    GET_GPIO(SDA_Key).setInternalResistor(InternalResistorType::PULLUP);

    SHAL_I2C_Enable_Reg pairI2CEnable = getI2CEnableReg(pair); //Register and mask to enable the I2C peripheral
    SHAL_I2C_Reset_Reg pairI2CReset = getI2CResetReg(pair);

    *pairI2CReset.reg |= pairI2CReset.mask; //Reset peripheral
    *pairI2CEnable.reg |= pairI2CEnable.mask; //Enable I2C peripheral clock

    I2CPair.I2CReg->CR1 |= I2C_CR1_PE; //Enable I2C peripheral
}

void SHAL_I2C::setClockConfig(uint8_t prescaler, uint8_t dataSetupTime, uint8_t dataHoldTime, uint8_t SCLHighPeriod, uint8_t SCLLowPeriod) {

    SHAL_I2C_Timing_Reg clockReg = getI2CTimerReg(m_I2CPair);

    *clockReg.reg |= (prescaler << clockReg.prescaler_offset);
    *clockReg.reg |= (dataSetupTime << clockReg.dataSetupTime_offset);
    *clockReg.reg |= (dataHoldTime << clockReg.dataHoldTime_offset);
    *clockReg.reg |= (SCLHighPeriod << clockReg.SCLHighPeriod_offset);
    *clockReg.reg |= (SCLLowPeriod << clockReg.SCLLowPeriod_offset);
}

void SHAL_I2C::setClockConfig(uint32_t configuration) {
    *getI2CTimerReg(m_I2CPair).reg = configuration;
}

void SHAL_I2C::masterTransmit(uint8_t addr, uint8_t reg, uint8_t data) {

    volatile I2C_TypeDef* I2CPeripheral = getI2CPair(m_I2CPair).I2CReg;

    //Wait until not busy
    while (I2CPeripheral->ISR & I2C_ISR_BUSY);

    //Send start + slave address
    I2CPeripheral->CR2 = (addr << 1) | (2 << I2C_CR2_NBYTES_Pos) | I2C_CR2_START | I2C_CR2_AUTOEND; //Pack bits in compliance with I2C format

    //Wait until TX ready
    while (!(I2CPeripheral->ISR & I2C_ISR_TXIS));

    //Send register address
    I2CPeripheral->TXDR = reg;

    //Wait until TX ready
    while (!(I2CPeripheral->ISR & I2C_ISR_TXIS));

    //Send data to write to register
    I2CPeripheral->TXDR = data;
}


uint8_t SHAL_I2C::masterReceive(uint8_t addr, uint8_t reg) {

    volatile I2C_TypeDef* I2CPeripheral = getI2CPair(m_I2CPair).I2CReg;

    //Send register address with write

    //Wait for bus
    while (I2CPeripheral->ISR & I2C_ISR_BUSY);

    //Send start with I2C config
    I2CPeripheral->CR2 = (addr << 1) | (1 << I2C_CR2_NBYTES_Pos) | I2C_CR2_START;

    //Wait for transmit
    while (!(I2CPeripheral->ISR & I2C_ISR_TXIS));

    //Set address to read from
    I2CPeripheral->TXDR = reg;

    //Wait for transfer to complete
    while (!(I2CPeripheral->ISR & I2C_ISR_TC));

    //Restart in read mode, auto end
    I2CPeripheral->CR2 = (addr << 1) | I2C_CR2_RD_WRN |
                (1 << I2C_CR2_NBYTES_Pos) |
                I2C_CR2_START | I2C_CR2_AUTOEND;

    //Wait
    while (!(I2C1->ISR & I2C_ISR_RXNE));
    return (uint8_t)I2C1->RXDR;
}

SHAL_I2C& I2CManager::get(uint8_t I2CBus) {

    if(I2CBus > NUM_I2C_BUSES - 1){
        assert(false);
        //Memory fault
    }

    return m_I2CBuses[I2CBus];
}
