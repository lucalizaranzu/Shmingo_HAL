//
// Created by Luca on 8/30/2025.
//

#include "SHAL_GPIO.h"
//#include "SHAL_EXTI_CALLBACK.h"



SHAL_GPIO::SHAL_GPIO() : m_GPIO_KEY(GPIO_Key::INVALID){
    //Do not initialize anything
}

SHAL_GPIO::SHAL_GPIO(GPIO_Key key) : m_GPIO_KEY(key) {

    auto GPIORCCEnable = getGPIORCCEnable(key);

    SHAL_set_register_value(GPIORCCEnable.reg,GPIORCCEnable.mask);
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

    /*
    if(mode == PinMode::ANALOG_MODE && getGPIOPortInfo(m_GPIO_KEY).ADCChannel == SHAL_ADC_Channel::NO_ADC_MAPPING){
        char buff[100];
        sprintf(buff, "Error: GPIO pin %d has no valid ADC mapping\r\n", static_cast<uint8_t>(m_GPIO_KEY));
        SHAL_UART2.sendString(buff);
        return SHAL_Result::ERROR;
    }
    */
    SHAL_set_bits(pinModeReg.reg,2,static_cast<uint8_t>(mode),pinModeReg.offset); //Set mode

    return SHAL_Result::OKAY;
}

/* TODO Fix implementation for STM32F072
void SHAL_GPIO::useAsExternalInterrupt(TriggerMode mode, EXTICallback callback) {

    uint32_t gpioPin = getGPIORegister(m_GPIO_KEY).global_offset; //Use existing structs to get offset

    setPinMode(PinMode::INPUT_MODE); //Explicitly set mode to input

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //Enable EXT, TODO check if this is different across STM32 models
    NVIC_EnableIRQ(getGPIOEXTICR(m_GPIO_KEY).IRQN); //Enable IRQN for pin
    EXTI->IMR |= (1 << gpioPin); //Enable correct EXTI line

    SHAL_EXTIO_Register EXTILineEnable = getGPIOEXTICR(m_GPIO_KEY);
    *EXTILineEnable.EXT_ICR |= EXTILineEnable.mask; //Set bits to enable correct port on correct line TODO Find way to clear bits before

    uint32_t rising_mask = 0x00;
    uint32_t falling_mask = 0x00;

    //Set rising and falling edge triggers based on pin offset (enabled EXTI line)
    switch(mode){
        case TriggerMode::RISING_EDGE:
            rising_mask = 1 << gpioPin;
            break;
        case TriggerMode::FALLING_EDGE:
            falling_mask = 1 << gpioPin;
            break;
        case TriggerMode::RISING_FALLING_EDGE:
            falling_mask = 1 << gpioPin;
            falling_mask = 1 << gpioPin;
    }

    //Set triggers
    EXTI->RTSR |= rising_mask;
    EXTI->FTSR |= falling_mask;

    //Set callback
    registerEXTICallback(m_GPIO_KEY,callback);

    __enable_irq(); //Enable IRQ just in case
}
*/

/* TODO reimplement
uint16_t SHAL_GPIO::analogRead(SHAL_ADC_SampleTime sampleTime) {

    SHAL_ADC_Channel channel = getGPIOPortInfo(m_GPIO_KEY).ADCChannel;

    return GPIOManager::getGPIOADC().singleConvertSingle(channel,sampleTime);
}
*/

SHAL_GPIO& GPIOManager::get(GPIO_Key key) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    uint8_t gpioPin = getGPIOPinNumber(key);

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = SHAL_GPIO(key);
    }

    return m_gpios[gpioPort][gpioPin];
}
