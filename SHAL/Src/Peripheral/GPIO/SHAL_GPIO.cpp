//
// Created by Luca on 8/30/2025.
//

#include "SHAL_GPIO.h"
#include "SHAL_EXTI_CALLBACK.h"



GPIO::GPIO() : m_GPIO_KEY(GPIO_Key::INVALID){
    //Do not initialize anything
}

GPIO::GPIO(GPIO_Key key, PinMode pinMode) : m_GPIO_KEY(key) {

    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(key);

    auto gpioRegister = gpioPeripheral.reg;
    unsigned long registerOffset = gpioPeripheral.global_offset;

    volatile unsigned long* gpioEnable = getGPIORCCEnable(key).reg;
    unsigned long gpioOffset = getGPIORCCEnable(key).offset;

    *gpioEnable |= (1 << gpioOffset); //Set enable flag

    gpioRegister->MODER &= ~(0x03 << (2 * registerOffset)); //Clear any previous mode
    gpioRegister->MODER |= (static_cast<uint8_t>(pinMode) << (2 * registerOffset)); //Set mode based on pinmode bit structure
}

void GPIO::setLow() {
    auto gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->ODR &= ~(1 << gpioPeripheral.global_offset);
}

void GPIO::setHigh() {
    auto gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->ODR |= (1 << gpioPeripheral.global_offset);
}

void GPIO::toggle() volatile {
    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->ODR ^= (1 << gpioPeripheral.global_offset);
}

void GPIO::setPinType(PinType type) volatile {
    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->OTYPER &= ~(1 << gpioPeripheral.global_offset);
    gpioPeripheral.reg->OTYPER |= (static_cast<uint8_t>(type) << gpioPeripheral.global_offset);
}

void GPIO::setOutputSpeed(OutputSpeed speed) volatile {
    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->OSPEEDR |= (static_cast<uint8_t>(speed) << (2 * gpioPeripheral.global_offset));
}

void GPIO::setInternalResistor(InternalResistorType type) volatile {
    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(m_GPIO_KEY);
    gpioPeripheral.reg->PUPDR &= ~(0x03 << (2 * gpioPeripheral.global_offset));
    gpioPeripheral.reg->PUPDR |= (static_cast<uint8_t>(type) << (2 * gpioPeripheral.global_offset));
}

void GPIO::setAlternateFunction(GPIO_Alternate_Function AF) volatile {
    SHAL_GPIO_Peripheral gpioPeripheral = getGPIORegister(m_GPIO_KEY);

    int afrIndex = gpioPeripheral.global_offset < 8 ? 0 : 1; //Get index of AFR

    gpioPeripheral.reg->AFR[afrIndex] &= ~(0xF << (gpioPeripheral.global_offset * 4));
    gpioPeripheral.reg->AFR[afrIndex] |= (static_cast<int>(AF) << (gpioPeripheral.global_offset * 4));
}


GPIO& GPIOManager::get(GPIO_Key key, PinMode pinMode) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    unsigned long gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = GPIO(key,pinMode);
    }

    return m_gpios[gpioPort][gpioPin];
}

void GPIOManager::getInterruptGPIO(GPIO_Key key, TriggerMode triggerMode, EXTICallback callback) {

    uint32_t gpioPort = getGPIOPortNumber(key);
    uint32_t gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    if (m_gpios[gpioPort][gpioPin].m_GPIO_KEY == GPIO_Key::INVALID){
        m_gpios[gpioPort][gpioPin] = GPIO(key,PinMode::INPUT_MODE); //Hardcode input mode for interrupt
    }

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; //Enable EXT, TODO check if this is different across STM32 models
    NVIC_EnableIRQ(getGPIOEXTICR(key).IRQN); //Enable IRQN for pin
    EXTI->IMR |= (1 << gpioPin); //Enable correct EXTI line

    SHAL_EXTIO_Register EXTILineEnable = getGPIOEXTICR(key);
    *EXTILineEnable.EXT_ICR |= EXTILineEnable.mask; //Set bits to enable correct port on correct line TODO Find way to clear bits before


    uint32_t rising_mask = 0x00;
    uint32_t falling_mask = 0x00;

    //Set rising and falling edge triggers based on pin offset (enabled EXTI line)
    switch(triggerMode){
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
    registerEXTICallback(key,callback);

    __enable_irq(); //Enable IRQ just in case
}