//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_H
#define SHMINGO_HAL_SHAL_GPIO_H

#include "SHAL_CORE.h"

#include <cassert>
#include "SHAL_GPIO_REG_F072xB.h"

enum class PinMode : uint8_t{
    INPUT_MODE,
    OUTPUT_MODE,
    ALTERNATE_FUNCTION_MODE,
    ANALOG_MODE,
    INVALID
};

unsigned long getPinMode(PinMode mode);

//Abstraction of GPIO registers
class GPIO{

public:

    void toggle();

    //TODO replace stupid offset hack from APB
    void setHigh();
    void setLow();

private:

    friend class GPIOManager;

    explicit GPIO(GPIO_Key key, PinMode pinMode);
    GPIO();

    GPIO_Key m_GPIO_KEY = GPIO_Key::INVALID;

};


#define initGPIO(GPIO_KEY, PIN_MODE) GPIOManager::get(GPIO_KEY, PIN_MODE)

//Manages instances of GPIO objects
class GPIOManager{

public:

    static GPIO& get(GPIO_Key, PinMode pinMode);
    GPIOManager() = delete;

private:

inline static GPIO m_gpios[AVAILABLE_PORTS][PINS_PER_PORT] = {{}};

};

#endif //SHMINGO_HAL_SHAL_GPIO_H
