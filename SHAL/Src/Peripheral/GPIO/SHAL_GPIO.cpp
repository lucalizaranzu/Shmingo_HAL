//
// Created by Luca on 8/30/2025.
//

#include "SHAL_GPIO.h"

GPIO::GPIO() {

}

GPIO::GPIO(GPIO_Key key) {

}

void GPIO::setLow() {
    getGPIORegister(GPIO_KEY)->ODR &= ~(1 << getGPIOAPB(GPIO_KEY).offset);
}

void GPIO::setHigh() {
    getGPIORegister(GPIO_KEY)->ODR |= (1 << getGPIOAPB(GPIO_KEY).offset);
}

void GPIO::toggle() {

}
