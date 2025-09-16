//
// Created by Luca on 9/15/2025.
//

#include "SHAL_CORE.h"

void SHAL_init(){
    systick_init(); //Just this for now
}


void systick_init(){
    SysTick->CTRL = 0;           //disable first
    SysTick->LOAD = 0xFFFFFF;    //max 24-bit
    SysTick->VAL  = 0;           //clear
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SHAL_delay_us(uint32_t us){
    uint32_t start = SysTick->VAL;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);

    //handle wraparound with 24-bit mask
    while (((start - SysTick->VAL) & 0x00FFFFFF) < ticks) { }
}

void SHAL_delay_ms(uint32_t ms){
    while(ms-- > 0){
        SHAL_delay_us(1000);
    }
}