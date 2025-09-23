//
// Created by Luca on 9/6/2025.
//

#ifndef SHAL_GPIO_TYPES_H
#define SHAL_GPIO_TYPES_H

#include "SHAL_CORE.h"

struct SHAL_EXTIO_Register{
    volatile uint32_t* EXT_ICR;
    uint32_t mask;
    IRQn_Type IRQN;
};

struct SHAL_GPIO_Peripheral {
    GPIO_TypeDef * reg;
    unsigned long global_offset;
};

struct SHAL_Peripheral_Register {
    volatile uint32_t* reg;
    unsigned long offset;
};

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



#endif //SHMINGO_HAL_SHAL_GPIO_TYPES_H
