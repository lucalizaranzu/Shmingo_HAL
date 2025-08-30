//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_CORE_H
#define SHMINGO_HAL_SHAL_CORE_H

#include <cstdint>


struct SHAL_Peripheral {
    volatile uint32_t* reg;
    uint32_t bitmask;
};

#endif //SHMINGO_HAL_SHAL_CORE_H
