//
// Created by Luca on 9/7/2025.
//

#ifndef SHAL_TIM_TYPES_H
#define SHAL_TIM_TYPES_H

#include "SHAL_CORE.h"

struct SHAL_TIM_RCC_Register{
    volatile uint32_t* reg;
    uint32_t enable_mask;
};

struct SHAL_TIM_Control_Register_1 {
    volatile uint32_t* reg;
    uint32_t counter_enable_mask;
    uint32_t update_disable_mask;
    uint32_t one_pulse_mode_mask;
    uint32_t center_align_mode_offset;
};

struct SHAL_TIM_DMA_Interrupt_Enable_Register {
    volatile uint32_t* reg;
    uint32_t update_interrupt_enable_mask;
};

struct SHAL_TIM_Status_Register {
    volatile uint32_t* reg;
    uint32_t update_interrupt_flag_mask;
};

struct SHAL_TIM_Event_Generation_Register {
    volatile uint32_t* reg;
    uint32_t update_generation_mask;
};

struct SHAL_TIM_Prescaler_Register {
    volatile uint32_t* reg;
    uint32_t offset;
};

struct SHAL_TIM_Auto_Reload_Register {
    volatile uint32_t* reg;
    uint32_t offset;
};

#endif //SHMINGO_HAL_SHAL_TIM_TYPES_H
