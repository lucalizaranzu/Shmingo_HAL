//
// Created by Luca on 9/9/2025.
//

#include "SHAL_I2C.h"
#include "SHAL_GPIO.h"

#include "SHAL_UART.h"

void SHAL_I2C::init(I2C_Pair pair) volatile {
    m_I2CPair = pair;

    SHAL_I2C_Pair I2CPair = getI2CPair(pair); //Get the I2C_PAIR information to be initialized

    //Get the SHAL_GPIO pins for this SHAL_I2C setup
    GPIO_Key SCL_Key = I2CPair.SCL_Key; //SCL pin
    GPIO_Key SDA_Key = I2CPair.SDA_Key; //SDA pin

    SHAL_I2C_Enable_Reg pairI2CEnable = getI2CEnableReg(pair); //Register and mask to enable the I2C peripheral

    *pairI2CEnable.reg &= ~pairI2CEnable.mask; //Enable I2C peripheral clock

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

    SHAL_I2C_Reset_Reg pairI2CReset = getI2CResetReg(pair);

    *pairI2CEnable.reg |= pairI2CEnable.mask; //Enable I2C peripheral clock

    *pairI2CReset.reg |= pairI2CReset.mask; //Reset peripheral
    *pairI2CReset.reg &= ~pairI2CReset.mask; //Reset peripheral
}

void SHAL_I2C::setClockConfig(uint8_t prescaler, uint8_t dataSetupTime, uint8_t dataHoldTime, uint8_t SCLHighPeriod, uint8_t SCLLowPeriod) {

    SHAL_I2C_Timing_Reg clockReg = getI2CTimerReg(m_I2CPair);

    *clockReg.reg |= (prescaler << clockReg.prescaler_offset);
    *clockReg.reg |= (dataSetupTime << clockReg.dataSetupTime_offset);
    *clockReg.reg |= (dataHoldTime << clockReg.dataHoldTime_offset);
    *clockReg.reg |= (SCLHighPeriod << clockReg.SCLHighPeriod_offset);
    *clockReg.reg |= (SCLLowPeriod << clockReg.SCLLowPeriod_offset);

    getI2CPair(m_I2CPair).I2CReg->CR1 |= I2C_CR1_PE; //Enable I2C peripheral
}

void SHAL_I2C::setClockConfig(uint32_t configuration) {
    *getI2CTimerReg(m_I2CPair).reg = configuration;

    getI2CPair(m_I2CPair).I2CReg->CR1 |= I2C_CR1_PE; //Enable I2C peripheral
}

void SHAL_I2C::masterWriteRead(uint8_t addr,const uint8_t* writeData, size_t writeLen, uint8_t* readData, size_t readLen) {

    volatile I2C_TypeDef* I2CPeripheral = getI2CPair(m_I2CPair).I2CReg;

    if(!SHAL_WAIT_FOR_CONDITION_MS((I2CPeripheral->ISR & I2C_ISR_BUSY) == 0, 100)){
        SHAL_UART2.sendString("I2C timed out waiting for not busy\r\n");
        return;
    }

    //Write phase
    if (writeLen > 0) {
        //Configure: NBYTES = wlen, write mode, START
        I2CPeripheral->CR2 = (addr << 1) | (writeLen << I2C_CR2_NBYTES_Pos) | I2C_CR2_START;

        for (size_t i = 0; i < writeLen; i++) {
            if(!SHAL_WAIT_FOR_CONDITION_MS((I2CPeripheral->ISR & I2C_ISR_TXIS) != 0, 100)){
                SHAL_UART2.sendString("I2C timed out waiting for TX\r\n");
                return;
            }
            I2CPeripheral->TXDR = writeData[i];
        }

        //Wait until transfer complete
        if(!SHAL_WAIT_FOR_CONDITION_MS((I2CPeripheral->ISR & I2C_ISR_TC) != 0, 100)){
            SHAL_UART2.sendString("I2C timed out waiting for TC\r\n");
            return;
        }
    }

    //Read phase
    if (readLen > 0) {

        SHAL_UART2.sendString("Read initiated\r\n");

        I2CPeripheral->CR2 &= ~(I2C_CR2_NBYTES | I2C_CR2_SADD | I2C_CR2_RD_WRN);
        I2CPeripheral->CR2 |= (addr << 1) |
                              I2C_CR2_RD_WRN |
                              (readLen << I2C_CR2_NBYTES_Pos) |
                              I2C_CR2_START | I2C_CR2_AUTOEND;

        for (size_t i = 0; i < readLen; i++) {
            if(!SHAL_WAIT_FOR_CONDITION_MS((I2CPeripheral->ISR & I2C_ISR_RXNE) != 0 , 100)){
                SHAL_UART2.sendString("I2C timed out waiting for RXNE\r\n");
                return;
            }
            SHAL_UART2.sendString("Read byte");
            readData[i] = static_cast<uint8_t>(I2CPeripheral->RXDR);
        }
    }
    else{
        I2CPeripheral->CR2 |= I2C_CR2_STOP;
    }
}

void SHAL_I2C::masterWrite(uint8_t addr, const uint8_t *writeData, uint8_t writeLen) {
    masterWriteRead(addr,writeData,writeLen,nullptr,0);
}

void SHAL_I2C::masterRead(uint8_t addr, uint8_t *readBuffer, uint8_t bytesToRead) {
    masterWriteRead(addr,nullptr,0,readBuffer,bytesToRead);
}

uint8_t SHAL_I2C::masterWriteReadByte(uint8_t addr, const uint8_t *writeData, size_t writeLen) {
    uint8_t val = 0;
    masterWriteRead(addr, writeData, writeLen, &val, 1);
    return val;
}

SHAL_I2C& I2CManager::get(uint8_t I2CBus) {

    if(I2CBus > NUM_I2C_BUSES - 1){
        assert(false);
        //Memory fault
    }

    return m_I2CBuses[I2CBus];
}
