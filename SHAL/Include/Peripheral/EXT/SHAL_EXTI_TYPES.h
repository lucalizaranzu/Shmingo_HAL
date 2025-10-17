//
// Created by Luca on 10/17/2025.
//

#ifndef SHMINGO_HAL_SHAL_EXTI_TYPES_H
#define SHMINGO_HAL_SHAL_EXTI_TYPES_H

#include "SHAL_CORE.h"

struct SHAL_EXTI_Interrupt_Mask_Register {
    volatile uint32_t* reg;
};

struct SHAL_EXTI_Rising_Trigger_Selection_Register {
    volatile uint32_t* reg;
};

struct SHAL_EXTI_Falling_Trigger_Selection_Register {
    volatile uint32_t* reg;
};

#endif //SHMINGO_HAL_SHAL_EXTI_TYPES_H
