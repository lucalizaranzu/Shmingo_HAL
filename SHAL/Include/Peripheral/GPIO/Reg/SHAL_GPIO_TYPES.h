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



#endif //SHMINGO_HAL_SHAL_GPIO_TYPES_H
