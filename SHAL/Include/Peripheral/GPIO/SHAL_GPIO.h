//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_H
#define SHMINGO_HAL_SHAL_GPIO_H

#include "SHAL_GPIO_REG.h"

#include <cassert>

#include "SHAL_EXTI_CALLBACK.h"



enum class PinMode : uint8_t{
    INPUT_MODE,
    OUTPUT_MODE,
    ALTERNATE_FUNCTION_MODE,
    ANALOG_MODE,
    INVALID
};
unsigned long getPinMode(PinMode mode);

enum class TriggerMode : uint8_t{
    RISING_EDGE,
    FALLING_EDGE,
    RISING_FALLING_EDGE
};

//Abstraction of GPIO registers
class GPIO{

public:

    void toggle() volatile;

    //TODO replace stupid offset hack from APB
    void setHigh();
    void setLow();

private:

    friend class GPIOManager;

    explicit GPIO(GPIO_Key key, PinMode pinMode);
    GPIO();

    GPIO_Key m_GPIO_KEY = GPIO_Key::INVALID;

};

//Init GPIO for normal use
#define initGPIO(GPIO_KEY, PIN_MODE) GPIOManager::get(GPIO_KEY, PIN_MODE)

//Init GPIO for use as an external interrupt
#define useGPIOAsInterrupt(GPIO_KEY, Trigger_Mode, Callback) GPIOManager::getInterruptGPIO(GPIO_KEY, Trigger_Mode, Callback)

//Manages instances of GPIO objects
class GPIOManager{

public:

    static GPIO& get(GPIO_Key, PinMode pinMode);

    static void getInterruptGPIO(GPIO_Key key, TriggerMode mode, EXTICallback callback);

    GPIOManager() = delete;

private:

inline static GPIO m_gpios[AVAILABLE_PORTS][PINS_PER_PORT] = {{}};

};


#endif //SHMINGO_HAL_SHAL_GPIO_H
