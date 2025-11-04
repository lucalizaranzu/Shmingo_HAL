//
// Created by Luca on 8/29/2025.
//

#ifndef SHAL_GPIO_REG_F072XB_H
#define SHAL_GPIO_REG_F072XB_H

#include <stm32l432xx.h>
#include <cassert>

#include "SHAL_GPIO_TYPES.h"

#define AVAILABLE_PORTS 2
#define PINS_PER_PORT 16
#define NUM_EXTI_LINES 16


//Build enum map of available SHAL_GPIO pins
enum class GPIO_Key : uint8_t {
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,
    A11,
    A12,
    A13,
    A14,
    A15,
    B0,
    B1,
    B3 = 19, //Offset to compensate for lack of B2
    B4,
    B5,
    B6,
    B7,
    NUM_GPIO,
    INVALID
};

static volatile GPIO_TypeDef * GPIO_TABLE[2] = { //Lookup table for ADCs
        GPIOA,
        GPIOB
};

constexpr uint8_t getGPIOPinNumber(GPIO_Key key){
    return static_cast<uint8_t>(key) % 16;
}

constexpr SHAL_GPIO_EXTI_Register getGPIOEXTICR(const GPIO_Key g){
    switch(g) {
        case GPIO_Key::A0: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0_PA,EXTI0_IRQn};
        case GPIO_Key::A1: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1_PA,EXTI1_IRQn};
        case GPIO_Key::A2: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI2_PA,EXTI2_IRQn};
        case GPIO_Key::A3: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3_PA,EXTI3_IRQn};
        case GPIO_Key::A4: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4_PA,EXTI4_IRQn};
        case GPIO_Key::A5: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5_PA,EXTI9_5_IRQn};
        case GPIO_Key::A6: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6_PA,EXTI9_5_IRQn};
        case GPIO_Key::A7: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7_PA,EXTI9_5_IRQn};
        case GPIO_Key::A8: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI8_PA,EXTI9_5_IRQn};
        case GPIO_Key::A9: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9_PA,EXTI9_5_IRQn};
        case GPIO_Key::A10: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI10_PA,EXTI15_10_IRQn};
        case GPIO_Key::A11: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI11_PA,EXTI15_10_IRQn};
        case GPIO_Key::A12: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI12_PA,EXTI15_10_IRQn};
        case GPIO_Key::A13: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI13_PA,EXTI15_10_IRQn};
        case GPIO_Key::A14: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI14_PA,EXTI15_10_IRQn};
        case GPIO_Key::A15: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI15_PA,EXTI15_10_IRQn};
        case GPIO_Key::B0: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0_PB,EXTI0_IRQn};
        case GPIO_Key::B1: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1_PB,EXTI1_IRQn};
        case GPIO_Key::B3: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3_PB,EXTI3_IRQn};
        case GPIO_Key::B4: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4_PB,EXTI4_IRQn};
        case GPIO_Key::B5: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5_PB,EXTI9_5_IRQn};
        case GPIO_Key::B6: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6_PB,EXTI9_5_IRQn};
        case GPIO_Key::B7: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7_PB,EXTI9_5_IRQn};


        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return SHAL_GPIO_EXTI_Register(nullptr, 0, static_cast<IRQn_Type>(0)); //Unreachable
    }
    __builtin_unreachable();
}

static inline SHAL_GPIO_RCC_Enable_Register getGPIORCCEnable(const GPIO_Key g){
    volatile uint32_t* reg = &RCC->AHB2ENR; //register
    uint32_t offset;
    offset = (static_cast<uint8_t>(g) / 16) == 0 ? RCC_AHB2ENR_GPIOAEN_Pos : RCC_AHB2ENR_GPIOBEN_Pos;

    return {reg,offset};
}

constexpr uint32_t getGPIOPortNumber(const GPIO_Key g){
    return (static_cast<uint8_t>(g) / 16);
}

static inline SHAL_GPIO_Mode_Register getGPIOModeRegister(const GPIO_Key key){
    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->MODER;
    uint32_t offset = 2 * static_cast<uint8_t>(key) % 16;
    return {reg,offset};
}

static inline SHAL_GPIO_Pullup_Pulldown_Register getGPIOPUPDRegister(const GPIO_Key key){
    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->PUPDR;
    uint32_t offset = 2 * static_cast<uint8_t>(key) % 16;
    return {reg,offset};
}

static inline SHAL_GPIO_Alternate_Function_Register getGPIOAlternateFunctionRegister(const GPIO_Key key){

    uint32_t pinNumber = static_cast<uint8_t>(key); //Number of pin (We need 0-7 to be AFR 1 and 8-15 to be AFR 2
    uint32_t afrIndex = pinNumber < 8 ? 0 : 1;

    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->AFR[afrIndex];
    uint32_t offset = (pinNumber % 8) * 4; //Increment in groups of four
    return {reg,offset};
}

static inline SHAL_GPIO_Output_Speed_Register getGPIOOutputSpeedRegister(const GPIO_Key key){
    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->OSPEEDR;
    uint32_t offset = 2 * static_cast<uint8_t>(key) % 16;
    return {reg,offset};
}

static inline SHAL_GPIO_Output_Type_Register getGPIOOutputTypeRegister(const GPIO_Key key){
    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->OTYPER;
    uint32_t offset = static_cast<uint8_t>(key) % 16;
    return {reg,offset};
}

static inline SHAL_GPIO_Output_Data_Register getGPIOOutputDataRegister(const GPIO_Key key){
    volatile uint32_t* reg = &GPIO_TABLE[static_cast<uint8_t>(key) / 16]->ODR;
    uint32_t offset = static_cast<uint8_t>(key) % 16;
    return {reg,offset};
}

constexpr SHAL_GPIO_Port_Info getGPIOPortInfo(GPIO_Key key){
    switch(key){
        case GPIO_Key::A0:
            return {0, SHAL_ADC_Channel::CH5};
        case GPIO_Key::B0:
            return {0, SHAL_ADC_Channel::CH15};
        case GPIO_Key::A1:
            return {1, SHAL_ADC_Channel::CH6};
        case GPIO_Key::B1:
            return {1, SHAL_ADC_Channel::CH16};
        case GPIO_Key::A2:
            return {2, SHAL_ADC_Channel::CH7};
        case GPIO_Key::A3:
            return {3, SHAL_ADC_Channel::CH8};
        case GPIO_Key::B3:
            return {3, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A4:
            return {4, SHAL_ADC_Channel::CH9};
        case GPIO_Key::B4:
            return {4, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A5:
            return {5, SHAL_ADC_Channel::CH10};
        case GPIO_Key::B5:
            return {5, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A6:
            return {6, SHAL_ADC_Channel::CH11};
        case GPIO_Key::B6:
            return {6, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A7:
            return {7, SHAL_ADC_Channel::CH12};
        case GPIO_Key::B7:
            return {7, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A8:
            return {8, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A9:
            return {9, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A10:
            return {10, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A11:
            return {11, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A12:
            return {12, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A13:
            return {13, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A14:
            return {14, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::A15:
            return {15, SHAL_ADC_Channel::NO_ADC_MAPPING};
        case GPIO_Key::NUM_GPIO:
        case GPIO_Key::INVALID:
            return {0, SHAL_ADC_Channel::NO_ADC_MAPPING};
    }
    __builtin_unreachable();
}

#endif //SHAL_GPIO_REG_F072XB_H