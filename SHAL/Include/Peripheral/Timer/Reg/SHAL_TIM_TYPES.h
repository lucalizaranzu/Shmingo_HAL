//
// Created by Luca on 9/7/2025.
//

#ifndef SHMINGO_HAL_SHAL_TIM_TYPES_H
#define SHMINGO_HAL_SHAL_TIM_TYPES_H

#include "SHAL_CORE.h"

struct TIM_RCC_Enable{
    volatile uint32_t* busEnableReg;
    uint32_t offset;
};

#endif //SHMINGO_HAL_SHAL_TIM_TYPES_H
