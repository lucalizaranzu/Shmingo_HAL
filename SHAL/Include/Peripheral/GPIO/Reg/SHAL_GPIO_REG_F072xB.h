//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H
#define SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H

#include <stm32f072xb.h>
#include <cassert>

#include "SHAL_GPIO_TYPES.h"

#define AVAILABLE_PORTS 3
#define PINS_PER_PORT 16
#define NUM_EXTI_LINES 16

#define AVAILABLE_GPIO \
    X(A0) X(A1) X(A2) X(A3) X(A4) X(A5) X(A6) X(A7) X(A8) X(A9) X(A10) X(A11) X(A12) X(A13) X(A14) X(A15) \
    X(B0) X(B1) X(B2) X(B3) X(B4) X(B5) X(B6) X(B7) X(B8) X(B9) X(B10) X(B11) X(B12) X(B13) X(B14) X(B15) \
    X(C0) X(C1) X(C2) X(C3) X(C4) X(C5) X(C6) X(C7) X(C8) X(C9) X(C10) X(C11) X(C12) X(C13) X(C14) X(C15)


//Build enum map of available GPIO pins
enum class GPIO_Key : uint8_t {
    #define X(key) key,
        AVAILABLE_GPIO
    #undef X
    NUM_GPIO,
    INVALID
};

//Valid usart Tx and Rx pairings for STM32F072
enum class UART_Pair : uint8_t{
    //UART1
    Tx1A9_Rx1A10,
    Tx1B6_Rx1B7,

    //UART2
    Tx2A2_Rx2A3,
    Tx2A14_Rx2A15,

    //UART3
    Tx3B10_Rx3B11,
    Tx3C4_Rx3C5,
    Tx3C10_Rx3C11,

    //UART4
    Tx4A0_Rx4A1,
    Tx4C10_Rx4C11
};

enum class USART {
    USART_1,
    USART_2,
    USART_3,
    USART_4
};

constexpr SHAL_UART_Pair getUARTPair(const UART_Pair pair){
    switch(pair){
        case UART_Pair::Tx1A9_Rx1A10: return {9,10,&GPIOA->AFR[1],&GPIOA->AFR[1],AF_Mask::AF1,AF_Mask::AF1};
        case UART_Pair::Tx1B6_Rx1B7: return {6,7,&GPIOB->AFR[0],&GPIOB->AFR[0],AF_Mask::AF0,AF_Mask::AF0};
        case UART_Pair::Tx2A2_Rx2A3: return {2,3,&GPIOA->AFR[0],&GPIOA->AFR[0],AF_Mask::AF1,AF_Mask::AF1};
        case UART_Pair::Tx2A14_Rx2A15: return {14,15,&GPIOA->AFR[1],&GPIOA->AFR[1],AF_Mask::AF1,AF_Mask::AF1};
        case UART_Pair::Tx3B10_Rx3B11: return {10,11,&GPIOB->AFR[1],&GPIOB->AFR[1],AF_Mask::AF4,AF_Mask::AF4};
        case UART_Pair::Tx3C4_Rx3C5: return {4,5,&GPIOC->AFR[0],&GPIOC->AFR[0],AF_Mask::AF1,AF_Mask::AF1};
        case UART_Pair::Tx3C10_Rx3C11: return {10,11,&GPIOC->AFR[1],&GPIOC->AFR[1],AF_Mask::AF1,AF_Mask::AF1};
        case UART_Pair::Tx4A0_Rx4A1: return {0,1,&GPIOA->AFR[0],&GPIOA->AFR[0],AF_Mask::AF4,AF_Mask::AF4};
        case UART_Pair::Tx4C10_Rx4C11: return {10,11,&GPIOC->AFR[1],&GPIOC->AFR[1],AF_Mask::AF0,AF_Mask::AF0};
    }
}

constexpr USART getUSARTReg(const UART_Pair pair){
    switch(pair){
        case UART_Pair::Tx1A9_Rx1A10:
        case UART_Pair::Tx1B6_Rx1B7:
            return USART::USART_1;
        case UART_Pair::Tx2A2_Rx2A3:
        case UART_Pair::Tx2A14_Rx2A15:
            return USART::USART_2;
        case UART_Pair::Tx3B10_Rx3B11:
        case UART_Pair::Tx3C4_Rx3C5:
        case UART_Pair::Tx3C10_Rx3C11:
            return USART::USART_3;
        case UART_Pair::Tx4A0_Rx4A1:
        case UART_Pair::Tx4C10_Rx4C11:
            return USART::USART_4;
    }
}


constexpr SHAL_Peripheral getGPIORegister(const GPIO_Key g){
    switch(g) {
        case GPIO_Key::A0: return {GPIOA,0};
        case GPIO_Key::A1: return {GPIOA,1};
        case GPIO_Key::A2: return {GPIOA,2};
        case GPIO_Key::A3: return {GPIOA,3};
        case GPIO_Key::A4: return {GPIOA,4};
        case GPIO_Key::A5: return {GPIOA,5};
        case GPIO_Key::A6: return {GPIOA,6};
        case GPIO_Key::A7: return {GPIOA,7};
        case GPIO_Key::A8: return {GPIOA,8};
        case GPIO_Key::A9: return {GPIOA,9};
        case GPIO_Key::A10: return {GPIOA,10};
        case GPIO_Key::A11: return {GPIOA,11};
        case GPIO_Key::A12: return {GPIOA,12};
        case GPIO_Key::A13: return {GPIOA,13};
        case GPIO_Key::A14: return {GPIOA,14};
        case GPIO_Key::A15: return {GPIOA,15};
        case GPIO_Key::B0: return {GPIOB,0};
        case GPIO_Key::B1: return {GPIOB,1};
        case GPIO_Key::B2: return {GPIOB,2};
        case GPIO_Key::B3: return {GPIOB,3};
        case GPIO_Key::B4: return {GPIOB,4};
        case GPIO_Key::B5: return {GPIOB,5};
        case GPIO_Key::B6: return {GPIOB,6};
        case GPIO_Key::B7: return {GPIOB,7};
        case GPIO_Key::B8: return {GPIOB,8};
        case GPIO_Key::B9: return {GPIOB,9};
        case GPIO_Key::B10: return {GPIOB,10};
        case GPIO_Key::B11: return {GPIOB,11};
        case GPIO_Key::B12: return {GPIOB,12};
        case GPIO_Key::B13: return {GPIOB,13};
        case GPIO_Key::B14: return {GPIOB,14};
        case GPIO_Key::B15: return {GPIOB,15};
        case GPIO_Key::C0: return {GPIOC,0};
        case GPIO_Key::C1: return {GPIOC,1};
        case GPIO_Key::C2: return {GPIOC,2};
        case GPIO_Key::C3: return {GPIOC,3};
        case GPIO_Key::C4: return {GPIOC,4};
        case GPIO_Key::C5: return {GPIOC,5};
        case GPIO_Key::C6: return {GPIOC,6};
        case GPIO_Key::C7: return {GPIOC,7};
        case GPIO_Key::C8: return {GPIOC,8};
        case GPIO_Key::C9: return {GPIOC,9};
        case GPIO_Key::C10: return {GPIOC,10};
        case GPIO_Key::C11: return {GPIOC,11};
        case GPIO_Key::C12: return {GPIOC,12};
        case GPIO_Key::C13: return {GPIOC,13};
        case GPIO_Key::C14: return {GPIOC,14};
        case GPIO_Key::C15: return {GPIOC,15};
        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return SHAL_Peripheral(nullptr,0); //Unreachable
    }
    __builtin_unreachable();
}

constexpr SHAL_EXTIO_Register getGPIOEXTICR(const GPIO_Key g){
    switch(g) {
        case GPIO_Key::A0: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0_PA,EXTI0_1_IRQn};
        case GPIO_Key::A1: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1_PA,EXTI0_1_IRQn};
        case GPIO_Key::A2: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI2_PA,EXTI2_3_IRQn};
        case GPIO_Key::A3: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3_PA,EXTI2_3_IRQn};
        case GPIO_Key::A4: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4_PA,EXTI4_15_IRQn};
        case GPIO_Key::A5: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5_PA,EXTI4_15_IRQn};
        case GPIO_Key::A6: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6_PA,EXTI4_15_IRQn};
        case GPIO_Key::A7: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7_PA,EXTI4_15_IRQn};
        case GPIO_Key::A8: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI8_PA,EXTI4_15_IRQn};
        case GPIO_Key::A9: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9_PA,EXTI4_15_IRQn};
        case GPIO_Key::A10: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI10_PA,EXTI4_15_IRQn};
        case GPIO_Key::A11: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI11_PA,EXTI4_15_IRQn};
        case GPIO_Key::A12: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI12_PA,EXTI4_15_IRQn};
        case GPIO_Key::A13: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI13_PA,EXTI4_15_IRQn};
        case GPIO_Key::A14: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI14_PA,EXTI4_15_IRQn};
        case GPIO_Key::A15: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI15_PA,EXTI4_15_IRQn};
        case GPIO_Key::B0: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0_PB,EXTI0_1_IRQn};
        case GPIO_Key::B1: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1_PB,EXTI0_1_IRQn};
        case GPIO_Key::B2: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI2_PB,EXTI2_3_IRQn};
        case GPIO_Key::B3: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3_PB,EXTI2_3_IRQn};
        case GPIO_Key::B4: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4_PB,EXTI4_15_IRQn};
        case GPIO_Key::B5: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5_PB,EXTI4_15_IRQn};
        case GPIO_Key::B6: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6_PB,EXTI4_15_IRQn};
        case GPIO_Key::B7: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7_PB,EXTI4_15_IRQn};
        case GPIO_Key::B8: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI8_PB,EXTI4_15_IRQn};
        case GPIO_Key::B9: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9_PB,EXTI4_15_IRQn};
        case GPIO_Key::B10: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI10_PB,EXTI4_15_IRQn};
        case GPIO_Key::B11: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI11_PB,EXTI4_15_IRQn};
        case GPIO_Key::B12: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI12_PB,EXTI4_15_IRQn};
        case GPIO_Key::B13: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI13_PB,EXTI4_15_IRQn};
        case GPIO_Key::B14: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI14_PB,EXTI4_15_IRQn};
        case GPIO_Key::B15: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI15_PB,EXTI4_15_IRQn};
        case GPIO_Key::C0: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0_PC,EXTI0_1_IRQn};
        case GPIO_Key::C1: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1_PC,EXTI0_1_IRQn};
        case GPIO_Key::C2: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI2_PC,EXTI2_3_IRQn};
        case GPIO_Key::C3: return {&SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3_PC,EXTI2_3_IRQn};
        case GPIO_Key::C4: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4_PC,EXTI4_15_IRQn};
        case GPIO_Key::C5: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5_PC,EXTI4_15_IRQn};
        case GPIO_Key::C6: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6_PC,EXTI4_15_IRQn};
        case GPIO_Key::C7: return {&SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7_PC,EXTI4_15_IRQn};
        case GPIO_Key::C8: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI8_PC,EXTI4_15_IRQn};
        case GPIO_Key::C9: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9_PC,EXTI4_15_IRQn};
        case GPIO_Key::C10: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI10_PC,EXTI4_15_IRQn};
        case GPIO_Key::C11: return {&SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI11_PC,EXTI4_15_IRQn};
        case GPIO_Key::C12: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI12_PC,EXTI4_15_IRQn};
        case GPIO_Key::C13: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI13_PC,EXTI4_15_IRQn};
        case GPIO_Key::C14: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI14_PC,EXTI4_15_IRQn};
        case GPIO_Key::C15: return {&SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI15_PC,EXTI4_15_IRQn};

        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return SHAL_EXTIO_Register(nullptr,0); //Unreachable
    }
    __builtin_unreachable();
}

constexpr SHAL_Peripheral_Register getGPIORCCEnable(const GPIO_Key g){
    switch(g) {
        case GPIO_Key::A0:
        case GPIO_Key::A1:
        case GPIO_Key::A2:
        case GPIO_Key::A3:
        case GPIO_Key::A4:
        case GPIO_Key::A5:
        case GPIO_Key::A6:
        case GPIO_Key::A7:
        case GPIO_Key::A8:
        case GPIO_Key::A9:
        case GPIO_Key::A10:
        case GPIO_Key::A11:
        case GPIO_Key::A12:
        case GPIO_Key::A13:
        case GPIO_Key::A14:
        case GPIO_Key::A15:
            return {&RCC->AHBENR, RCC_AHBENR_GPIOAEN_Pos};
        case GPIO_Key::B0:
        case GPIO_Key::B1:
        case GPIO_Key::B2:
        case GPIO_Key::B3:
        case GPIO_Key::B4:
        case GPIO_Key::B5:
        case GPIO_Key::B6:
        case GPIO_Key::B7:
        case GPIO_Key::B8:
        case GPIO_Key::B9:
        case GPIO_Key::B10:
        case GPIO_Key::B11:
        case GPIO_Key::B12:
        case GPIO_Key::B13:
        case GPIO_Key::B14:
        case GPIO_Key::B15:
            return {&RCC->AHBENR, RCC_AHBENR_GPIOBEN_Pos};
        case GPIO_Key::C0:
        case GPIO_Key::C1:
        case GPIO_Key::C2:
        case GPIO_Key::C3:
        case GPIO_Key::C4:
        case GPIO_Key::C5:
        case GPIO_Key::C6:
        case GPIO_Key::C7:
        case GPIO_Key::C8:
        case GPIO_Key::C9:
        case GPIO_Key::C10:
        case GPIO_Key::C11:
        case GPIO_Key::C12:
        case GPIO_Key::C13:
        case GPIO_Key::C14:
        case GPIO_Key::C15:
            return {&RCC->AHBENR, RCC_AHBENR_GPIOCEN_Pos};
        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return SHAL_Peripheral_Register(nullptr,0); //Unreachable
    }
    __builtin_unreachable();
}

constexpr unsigned int getGPIOPortNumber(const GPIO_Key g){
    switch(g) {
        case GPIO_Key::A0:
        case GPIO_Key::A1:
        case GPIO_Key::A2:
        case GPIO_Key::A3:
        case GPIO_Key::A4:
        case GPIO_Key::A5:
        case GPIO_Key::A6:
        case GPIO_Key::A7:
        case GPIO_Key::A8:
        case GPIO_Key::A9:
        case GPIO_Key::A10:
        case GPIO_Key::A11:
        case GPIO_Key::A12:
        case GPIO_Key::A13:
        case GPIO_Key::A14:
        case GPIO_Key::A15:
            return 0;
        case GPIO_Key::B0:
        case GPIO_Key::B1:
        case GPIO_Key::B2:
        case GPIO_Key::B3:
        case GPIO_Key::B4:
        case GPIO_Key::B5:
        case GPIO_Key::B6:
        case GPIO_Key::B7:
        case GPIO_Key::B8:
        case GPIO_Key::B9:
        case GPIO_Key::B10:
        case GPIO_Key::B11:
        case GPIO_Key::B12:
        case GPIO_Key::B13:
        case GPIO_Key::B14:
        case GPIO_Key::B15:
            return 1;
        case GPIO_Key::C0:
        case GPIO_Key::C1:
        case GPIO_Key::C2:
        case GPIO_Key::C3:
        case GPIO_Key::C4:
        case GPIO_Key::C5:
        case GPIO_Key::C6:
        case GPIO_Key::C7:
        case GPIO_Key::C8:
        case GPIO_Key::C9:
        case GPIO_Key::C10:
        case GPIO_Key::C11:
        case GPIO_Key::C12:
        case GPIO_Key::C13:
        case GPIO_Key::C14:
        case GPIO_Key::C15:
            return 2;
        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return 0;
    }
    __builtin_unreachable();
}



#endif //SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H
