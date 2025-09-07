//
// Created by Luca on 9/6/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_TYPES_H
#define SHMINGO_HAL_SHAL_GPIO_TYPES_H

#include "SHAL_CORE.h"


struct SHAL_EXTIO_Register{
    volatile uint32_t* EXT_ICR;
    uint32_t mask;
    IRQn_Type IRQN;
};



#endif //SHMINGO_HAL_SHAL_GPIO_TYPES_H
