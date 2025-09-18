//
// Created by Luca on 9/15/2025.
//

#include "SHAL_CORE.h"

void SHAL_init(){
    systick_init(); //Just this for now
}


void systick_init(){
    SysTick->CTRL = 0;           //Disable first
    SysTick->LOAD = 0xFFFFFF;    //Max 24-bit
    SysTick->VAL  = 0;           //Clear
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}


void SHAL_delay_us(uint32_t us){
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    uint32_t start = SysTick->VAL;

    //Calculate target value (may wrap around)
    uint32_t target = (start >= ticks) ? (start - ticks) : (start + 0x01000000 - ticks);
    target &= 0x00FFFFFF;

    //Wait until we reach the target
    if (start >= ticks) {
        //No wraparound case
        while (SysTick->VAL > target) {}
    } else {
        while (SysTick->VAL <= start) {} //Wait for wraparound
        while (SysTick->VAL > target) {} //Wait for target
    }
}

void SHAL_delay_ms(uint32_t ms){
    while(ms-- > 0){
        SHAL_delay_us(1000);
    }
}

bool SHAL_wait_for_condition_us(condition_fn_t condition, uint32_t timeout_us){
    while (timeout_us--) {
        if (condition()) {
            return true; // Condition met
        }
        SHAL_delay_us(1); // Wait 1 µs
    }
    return false; // Timeout
}

bool SHAL_wait_for_condition_ms(condition_fn_t condition, uint32_t timeout_ms){
    while (timeout_ms--) {
        if (condition()) {
            return true; // Condition met
        }
        SHAL_delay_ms(1); // Wait 1 µs
    }
    return false; // Timeout
}