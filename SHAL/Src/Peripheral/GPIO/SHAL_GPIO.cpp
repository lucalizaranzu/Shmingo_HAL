//
// Created by Luca on 8/30/2025.
//

#include "SHAL_GPIO.h"


unsigned long getPinMode(PinMode mode){
    switch(mode){
        case PinMode::INPUT_MODE:
            return 0b00;
        case PinMode::OUTPUT_MODE:
            return 0b01;
        case PinMode::ALTERNATE_FUNCTION_MODE:
            return 0b10;
        case PinMode::ANALOG_MODE:
            return 0b11;
        case PinMode::INVALID:
            assert(false);
            return 0;
    }
    __builtin_unreachable();
}




GPIO::GPIO() : m_GPIO_KEY(GPIO_Key::INVALID){
    //Do not initialize anything
}

GPIO::GPIO(GPIO_Key key, PinMode pinMode) : m_GPIO_KEY(key) {

    SHAL_Peripheral gpioPeripheral = getGPIORegister(key);

    auto gpioRegister = static_cast<GPIO_TypeDef*>(gpioPeripheral.registers);
    unsigned long registerOffset = gpioPeripheral.global_offset;

    volatile unsigned long* gpioEnable = getGPIORCCEnable(key).reg;
    unsigned long gpioOffset = getGPIORCCEnable(key).offset;

    *gpioEnable |= (1 << gpioOffset); //Set enable flag

    gpioRegister->MODER &= ~(0b11 << (2 * registerOffset)); //Clear any previous mode
    gpioRegister->MODER |= (getPinMode(pinMode) << (2 * registerOffset)); //Set mode based on pinmode bit structure
}

void GPIO::setLow() {
    auto gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    static_cast<GPIO_TypeDef*>(gpioPeripheral.registers)->ODR &= ~(1 << gpioPeripheral.global_offset);
}

void GPIO::setHigh() {
    auto gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    static_cast<GPIO_TypeDef*>(gpioPeripheral.registers)->ODR |= (1 << gpioPeripheral.global_offset);
}

void GPIO::toggle() {
    auto gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    static_cast<GPIO_TypeDef*>(gpioPeripheral.registers)->ODR ^= (1 << gpioPeripheral.global_offset);
}



GPIO& GPIOManager::get(GPIO_Key key, PinMode pinMode) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    unsigned long gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = GPIO(key,pinMode);
    }

    return m_gpios[gpioPort][gpioPin];
}

void GPIOManager::getInterruptGPIO(GPIO_Key key, TriggerMode mode, EXTICallback callback) {
    unsigned int gpioPort = getGPIOPortNumber(key);
    unsigned long gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = GPIO(key,PinMode::INPUT_MODE); //Hardcode input mode for interrupt
    }

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //TODO check if this is different across STM32 models, enable EXT

    SHAL_EXTIO_Register EXTILineEnable = getGPIOEXTICR(key);

    *EXTILineEnable.EXT_ICR |= EXTILineEnable.mask; //Set bits to enable correct port on correct line


}
