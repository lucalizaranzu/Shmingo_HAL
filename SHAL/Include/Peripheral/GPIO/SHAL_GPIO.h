//
// Created by Luca on 8/29/2025.
//

#ifndef SHAL_GPIO_H
#define SHAL_GPIO_H

#include "SHAL_GPIO_REG.h"

#include <cassert>

#include "SHAL_EXTI_CALLBACK.h"

enum class PinMode : uint8_t{
    INPUT_MODE                  = 0x00,
    OUTPUT_MODE                 = 0x01,
    ALTERNATE_FUNCTION_MODE     = 0x02,
    ANALOG_MODE                 = 0x03,
    INVALID                     = 0x00,
};

enum class GPIO_Alternate_Function : uint8_t{
    AF0                         = 0x00,
    AF1                         = 0x01,
    AF2                         = 0x02,
    AF3                         = 0x03,
    AF4                         = 0x04,
    AF5                         = 0x05,
    AF6                         = 0x06,
    AF7                         = 0x07,
};

enum class PinType : uint8_t{
    PUSH_PULL                   = 0x00,
    OPEN_DRAIN                  = 0x01,
};

enum class InternalResistorType : uint8_t{
    NO_PULL                     = 0x00,
    PULLUP                      = 0x01,
    PULLDOWN                    = 0x02,
};

enum class OutputSpeed : uint8_t{
    LOW_SPEED                   = 0x00,
    MEDIUM_SPEED                = 0x01,
    HIGH_SPEED                  = 0x02,
    VERY_HIGH_SPEED             = 0x03,
};

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

    void setAlternateFunction(GPIO_Alternate_Function AF) volatile;

    void setPinType(PinType type) volatile;

    void setOutputSpeed(OutputSpeed speed) volatile;

    void setInternalResistor(InternalResistorType type) volatile;

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
