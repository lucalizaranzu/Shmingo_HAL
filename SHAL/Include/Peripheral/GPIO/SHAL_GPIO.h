//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_H
#define SHMINGO_HAL_SHAL_GPIO_H

#include "SHAL_CORE.h"

#include <cassert>
#include "SHAL_GPIO_REG_F072xB.h"

enum class PinMode {
    INPUT_MODE = 0b00,
    OUTPUT_MODE = 0b01,
    ALTERNATE_FUNCTION_MODE = 0b10,
    ANALOG_MODE = 0b11
};

enum class PinValue {
    HI = 1,
    LOW = 0,
};

class GPIO{

public:

    void toggle();

    //TODO replace stupid offset hack from APB
    void setHigh();
    void setLow();

private:

    friend class GPIOManager;

    explicit GPIO(GPIO_Key key);
    GPIO();

    GPIO_Key GPIO_KEY;
};

#endif //SHMINGO_HAL_SHAL_GPIO_H
