//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H
#define SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H

#include "SHAL_CORE.h"

#define EXTI_PENDING_REG(line) ((line) < 32 ? EXTI->PR1 : EXTI->PR2)

#endif //SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H
