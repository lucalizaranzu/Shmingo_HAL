//
// Created by Luca on 10/8/2025.
//

#ifndef SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H
#define SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H

#include "SHAL_CORE.h"
#include "SHAL_EXTI_TYPES.h"

#define EXTI_PENDING_REG(line) ((line) < 32 ? EXTI->PR1 : EXTI->PR2)

static inline SHAL_EXTI_Interrupt_Mask_Register getEXTIInterruptMaskRegister(uint32_t line){
    volatile uint32_t* reg =  line < 32 ? &EXTI->IMR1 : &EXTI->IMR2;
    return {reg};;
}

static inline SHAL_EXTI_Rising_Trigger_Selection_Register getEXTIRisingTriggerSelectionRegister(uint32_t line){
    volatile uint32_t* reg =  line < 32 ? &EXTI->RTSR1 : &EXTI->RTSR2;
    return {reg};
}

static inline SHAL_EXTI_Falling_Trigger_Selection_Register getEXTIFallingTriggerSelectionRegister(uint32_t line){
    volatile uint32_t* reg =  line < 32 ? &EXTI->FTSR1 : &EXTI->FTSR2;
    return {reg};
}

#endif //SHMINGO_HAL_SHAL_EXTI_REG_L432KC_H
