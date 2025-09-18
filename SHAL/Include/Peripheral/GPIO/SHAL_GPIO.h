//
// Created by Luca on 8/29/2025.
//

#ifndef SHAL_GPIO_H
#define SHAL_GPIO_H

#include "SHAL_GPIO_REG.h"

#include <cassert>

#include "SHAL_EXTI_CALLBACK.h"





//Abstraction of SHAL_GPIO registers
class SHAL_GPIO{

public:

    void toggle() volatile;

    //TODO replace stupid offset hack from APB
    void setHigh();
    void setLow();

    void setPinMode(PinMode mode) volatile;

    void setAlternateFunction(GPIO_Alternate_Function AF) volatile;

    void setPinType(PinType type) volatile;

    void setOutputSpeed(OutputSpeed speed) volatile;

    void setInternalResistor(InternalResistorType type) volatile;


    void useAsExternalInterrupt(TriggerMode mode, EXTICallback callback);

private:

    friend class GPIOManager;

    explicit SHAL_GPIO(GPIO_Key key);
    SHAL_GPIO();

    GPIO_Key m_GPIO_KEY = GPIO_Key::INVALID;

};





//Init SHAL_GPIO for normal use
#define PIN_TO_KEY(name) GPIO_Key::name
#define PIN(name) GPIOManager::get(PIN_TO_KEY(name))

#define GET_GPIO(key) GPIOManager::get(key)

#define GPIO_A

//Manages instances of SHAL_GPIO objects
class GPIOManager{

public:

    static SHAL_GPIO& get(GPIO_Key);


    GPIOManager() = delete;

private:

inline static SHAL_GPIO m_gpios[AVAILABLE_PORTS][PINS_PER_PORT] = {{}};

};


#endif //SHMINGO_HAL_SHAL_GPIO_H
