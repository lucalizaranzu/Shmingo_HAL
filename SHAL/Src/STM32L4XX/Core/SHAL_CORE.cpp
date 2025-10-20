//
// Created by Luca on 9/15/2025.
//

#include <cstdio>

#include "SHAL_CORE.h"
#include "SHAL_GPIO.h"
#include "SHAL_ADC.h"
#include "SHAL_UART.h"

void SHAL_init(){
    systick_init();


    for(auto i = static_cast<uint8_t>(ADC_Key::S_ADC1); i < static_cast<uint8_t>(ADC_Key::NUM_ADC); i++){ //Init all ADCs
        auto adc_key = static_cast<ADC_Key>(i);

        ADCManager::getByIndex(i).init(adc_key);
    }
    SET_ANALOGREAD_ADC(SHAL_ADC1); //Default ADC1 for analogread calls

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

void SHAL_print_register(const volatile uint32_t* reg){
    char buff[32];
    sprintf(buff, "0x%08lX\r\n", (unsigned long)(*reg));
    SHAL_UART2.sendString(buff);
}
