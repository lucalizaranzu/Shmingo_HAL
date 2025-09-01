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




GPIO::GPIO(){
    //Do not initialize anything
}

GPIO::GPIO(GPIO_Key key, PinMode pinMode) : m_GPIO_KEY(key),
                                            p_GPIORegister(static_cast<GPIO_TypeDef*>(getGPIORegister(key).registers)),
                                            m_offset(getGPIORegister(key).global_offset){

    volatile unsigned long* gpioEnable = getGPIORCCEnable(key).reg;
    unsigned long gpioOffset = getGPIORCCEnable(key).offset;

    *gpioEnable |= (1 << gpioOffset); //Set enable flag

    p_GPIORegister->MODER &= ~(0b11 << (2 * m_offset)); //Clear any previous mode
    p_GPIORegister->MODER |= (getPinMode(pinMode) << (2 * m_offset)); //Set mode based on pinmode bit structure
}

void GPIO::setLow() {
    p_GPIORegister->ODR &= ~(1 << m_offset);
}

void GPIO::setHigh() {
    p_GPIORegister->ODR |= (1 << m_offset);

}

void GPIO::toggle() {
    p_GPIORegister->ODR ^= (1 << m_offset);
}



GPIO* GPIOManager::get(GPIO_Key key, PinMode pinMode) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    unsigned long gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    GPIO curr = *m_gpios[gpioPort][gpioPin];

    if(curr.m_GPIO_KEY == GPIO_Key::INVALID || curr.m_pinMode != pinMode){
        *m_gpios[gpioPort][gpioPin] = GPIO(key,pinMode);
    }

    return m_gpios[gpioPort][gpioPin];
}
