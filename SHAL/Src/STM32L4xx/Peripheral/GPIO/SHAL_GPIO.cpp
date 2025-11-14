//
// Created by Luca on 8/30/2025.
//

#include <cstdio>
#include "SHAL_GPIO.h"
#include "SHAL_EXTI_CALLBACK.h"

#include "SHAL_UART.h"

SHAL_GPIO::SHAL_GPIO() : m_GPIO_KEY(GPIO_Key::INVALID){
    //Do not initialize anything
}

SHAL_GPIO::SHAL_GPIO(GPIO_Key key) : m_GPIO_KEY(key) {

    volatile unsigned long* gpioEnable = getGPIORCCEnable(key).reg;
    unsigned long gpioOffset = getGPIORCCEnable(key).offset;

    *gpioEnable |= (1 << gpioOffset); //Set enable flag
}

void SHAL_GPIO::setLow() {
    auto outputDataReg = getGPIOOutputDataRegister(m_GPIO_KEY);
    SHAL_set_bits(outputDataReg.reg,1,0,outputDataReg.offset);
}

void SHAL_GPIO::setHigh() {
    auto outputDataReg = getGPIOOutputDataRegister(m_GPIO_KEY);
    SHAL_set_bits(outputDataReg.reg,1,1,outputDataReg.offset);
}

void SHAL_GPIO::toggle() volatile {
    auto outputDataReg = getGPIOOutputDataRegister(m_GPIO_KEY);
    SHAL_flip_bits(outputDataReg.reg,1,outputDataReg.offset);
}

void SHAL_GPIO::setOutputType(PinType type) volatile {
    auto outputTypeReg = getGPIOOutputTypeRegister(m_GPIO_KEY);
    SHAL_set_bits(outputTypeReg.reg,2,static_cast<uint8_t>(type),outputTypeReg.offset);
}

void SHAL_GPIO::setOutputSpeed(OutputSpeed speed) volatile {
    auto outputSpeedReg = getGPIOOutputSpeedRegister(m_GPIO_KEY);
    SHAL_set_bits(outputSpeedReg.reg,2,static_cast<uint8_t>(speed),outputSpeedReg.offset);
}

void SHAL_GPIO::setInternalResistor(InternalResistorType type) volatile {
    auto pupdreg = getGPIOPUPDRegister(m_GPIO_KEY);
    SHAL_set_bits(pupdreg.reg,2,static_cast<uint8_t>(type),pupdreg.offset);
}

void SHAL_GPIO::setAlternateFunction(GPIO_Alternate_Function AF) volatile {
    auto alternateFunctionReg = getGPIOAlternateFunctionRegister(m_GPIO_KEY);
    SHAL_set_bits(alternateFunctionReg.reg,4,static_cast<uint8_t>(AF),alternateFunctionReg.offset);
}

SHAL_Result SHAL_GPIO::setPinMode(PinMode mode) volatile {
    auto pinModeReg = getGPIOModeRegister(m_GPIO_KEY);

    if(mode == PinMode::ANALOG_MODE && getGPIOPortInfo(m_GPIO_KEY).ADCChannel == SHAL_ADC_Channel::NO_ADC_MAPPING){
        char buff[100];
        sprintf(buff, "Error: GPIO pin %d has no valid ADC mapping\r\n", static_cast<uint8_t>(m_GPIO_KEY));
        SHAL_UART2.sendString(buff);
        return SHAL_Result::ERROR;
    }

    SHAL_set_bits(pinModeReg.reg,2,static_cast<uint8_t>(mode),pinModeReg.offset); //Set mode

    return SHAL_Result::OKAY;
}

void SHAL_GPIO::useAsExternalInterrupt(TriggerMode mode, EXTICallback callback) {


    /* ---- Connect PB6 to EXTI6 via SYSCFG ---- */
    uint32_t port_b_val = 1; // 0=A, 1=B, 2=C, 3=D, etc.
    SYSCFG->EXTICR[1] &= ~(0xFUL << 8);     // Clear EXTI6 bits (bits 8-11)
    SYSCFG->EXTICR[1] |=  (port_b_val << 8); // Set EXTI6 to PB6

    /* ---- Configure EXTI line 6 ---- */
    EXTI->IMR1  |=  (1UL << 6);  // Unmask line 6
    EXTI->RTSR1 |=  (1UL << 6);  // Rising trigger enable
    EXTI->FTSR1 &= ~(1UL << 6);  // Falling trigger disable

    /* ---- Enable NVIC interrupt for EXTI lines [9:5] ---- */
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    __enable_irq(); //Enable IRQ just in case
}

uint16_t SHAL_GPIO::analogRead(SHAL_ADC_SampleTime sampleTime) {

    SHAL_ADC_Channel channel = getGPIOPortInfo(m_GPIO_KEY).ADCChannel;

    return GPIOManager::getGPIOADC().singleConvertSingle(channel,sampleTime);
}

void SHAL_GPIO::setAlternateFunction(GPIO_Alternate_Function_Mapping AF) volatile {
    setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);
    auto alternateFunctionReg = getGPIOAlternateFunctionRegister(m_GPIO_KEY);
    SHAL_set_bits(alternateFunctionReg.reg,4,static_cast<uint8_t>(AF),alternateFunctionReg.offset);
}

uint16_t SHAL_GPIO::digitalRead() {
    auto inputDataReg = getGPIOInputDataRegister(m_GPIO_KEY);

    if((*inputDataReg.reg & (1 << 6)) != 0){
        return 1;
    }
    return 0;
}


SHAL_GPIO& GPIOManager::get(GPIO_Key key) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    uint8_t gpioPin = getGPIOPinNumber(key);

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = SHAL_GPIO(key);
    }

    return m_gpios[gpioPort][gpioPin];
}