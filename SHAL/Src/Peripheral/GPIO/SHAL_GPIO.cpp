//
// Created by Luca on 8/30/2025.
//

#include "SHAL_GPIO.h"

GPIO::GPIO(){
    //Do not initialize anything
}

GPIO::GPIO(GPIO_Key key, PinMode pinMode) : m_GPIO_KEY(key),
                                            p_GPIORegister(static_cast<GPIO_TypeDef*>(getGPIORegister(key).registers)),
                                            m_offset(getGPIORegister(key).global_offset){

    volatile unsigned long* gpioEnable = getGPIORCCEnable(key).reg;
    unsigned long gpioOffset = getGPIORCCEnable(key).offset;

    *gpioEnable |= (1 << gpioOffset);

    p_GPIORegister->MODER |= (getPinMode(pinMode) << (2 * m_offset));
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



GPIO &GPIOManager::get(GPIO_Key key, PinMode pinMode) {

    unsigned int gpioPort = getGPIOPortNumber(key);
    unsigned long gpioPin = getGPIORegister(key).global_offset; //Use existing structs to get offset

    GPIO curr = m_gpios[gpioPort][gpioPin];

    if(curr.m_GPIO_KEY == GPIO_Key::INVALID || curr.m_pinMode != pinMode){
        m_gpios[gpioPort][gpioPin] = GPIO(key,pinMode);
    }

    return m_gpios[gpioPort][gpioPin];
}
