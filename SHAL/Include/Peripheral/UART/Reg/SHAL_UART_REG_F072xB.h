//
// Created by Luca on 9/7/2025.
//

#ifndef SHAL_UART_REG_F072XB_H
#define SHAL_UART_REG_F072XB_H

#include <stm32f072xb.h>
#include <cassert>

#include "SHAL_UART_TYPES.h"

#define NUM_USART_LINES 4

#define SHAL_UART1 UART(1)
#define SHAL_UART2 UART(2)
#define SHAL_UART3 UART(3)
#define SHAL_UART4 UART(4)

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
    Tx4C10_Rx4C11,

    NUM_PAIRS,
    INVALID
};


constexpr SHAL_UART_Pair getUARTPair(const UART_Pair pair){
    switch(pair){
        case UART_Pair::Tx1A9_Rx1A10: return {USART1, GPIO_Key::A9, GPIO_Key::A10, GPIO_Alternate_Function::AF1, GPIO_Alternate_Function::AF1};
        case UART_Pair::Tx1B6_Rx1B7: return {USART1, GPIO_Key::B6, GPIO_Key::B7, GPIO_Alternate_Function::AF0, GPIO_Alternate_Function::AF0};
        case UART_Pair::Tx2A2_Rx2A3: return {USART2, GPIO_Key::A2, GPIO_Key::A3, GPIO_Alternate_Function::AF1, GPIO_Alternate_Function::AF1};
        case UART_Pair::Tx2A14_Rx2A15: return {USART2, GPIO_Key::A14, GPIO_Key::A15, GPIO_Alternate_Function::AF1, GPIO_Alternate_Function::AF1};
        case UART_Pair::Tx3B10_Rx3B11: return {USART3, GPIO_Key::B10, GPIO_Key::B11, GPIO_Alternate_Function::AF4, GPIO_Alternate_Function::AF4};
        case UART_Pair::Tx3C4_Rx3C5: return {USART3, GPIO_Key::C4, GPIO_Key::C5, GPIO_Alternate_Function::AF1, GPIO_Alternate_Function::AF1};
        case UART_Pair::Tx3C10_Rx3C11: return {USART3, GPIO_Key::C10, GPIO_Key::C11, GPIO_Alternate_Function::AF1, GPIO_Alternate_Function::AF1};
        case UART_Pair::Tx4A0_Rx4A1: return {USART4, GPIO_Key::A0, GPIO_Key::A1, GPIO_Alternate_Function::AF4, GPIO_Alternate_Function::AF4};
        case UART_Pair::Tx4C10_Rx4C11: return {USART4, GPIO_Key::C10, GPIO_Key::C11, GPIO_Alternate_Function::AF0, GPIO_Alternate_Function::AF0};
        case UART_Pair::NUM_PAIRS:
        case UART_Pair::INVALID:
            assert(false);
            return {nullptr, GPIO_Key::INVALID, GPIO_Key::INVALID, GPIO_Alternate_Function::AF0, GPIO_Alternate_Function::AF0};
    }
    __builtin_unreachable();
}

constexpr uint8_t getUARTChannel(const UART_Pair pair){
    switch(pair){
        case UART_Pair::Tx1A9_Rx1A10:
        case UART_Pair::Tx1B6_Rx1B7:
            return 0;
        case UART_Pair::Tx2A2_Rx2A3:
        case UART_Pair::Tx2A14_Rx2A15:
            return 1;
        case UART_Pair::Tx3B10_Rx3B11:
        case UART_Pair::Tx3C4_Rx3C5:
        case UART_Pair::Tx3C10_Rx3C11:
            return 2;
        case UART_Pair::Tx4A0_Rx4A1:
        case UART_Pair::Tx4C10_Rx4C11:
            return 3;
        case UART_Pair::NUM_PAIRS:
        case UART_Pair::INVALID:
            assert(false);
            return 0;
    }
    __builtin_unreachable();
}

constexpr SHAL_UART_ENABLE_REG getUARTEnableReg(const UART_Pair pair){
    switch(pair){
        case UART_Pair::Tx1A9_Rx1A10:
        case UART_Pair::Tx1B6_Rx1B7:
            return {&RCC->APB2ENR,RCC_APB2ENR_USART1EN};
        case UART_Pair::Tx2A2_Rx2A3:
        case UART_Pair::Tx2A14_Rx2A15:
            return {&RCC->APB1ENR,RCC_APB1ENR_USART2EN};
        case UART_Pair::Tx3B10_Rx3B11:
        case UART_Pair::Tx3C4_Rx3C5:
        case UART_Pair::Tx3C10_Rx3C11:
            return {&RCC->APB1ENR,RCC_APB1ENR_USART3EN};
        case UART_Pair::Tx4A0_Rx4A1:
        case UART_Pair::Tx4C10_Rx4C11:
            return {&RCC->APB1ENR,RCC_APB1ENR_USART4EN};
        case UART_Pair::NUM_PAIRS:
        case UART_Pair::INVALID:
            assert(false);
            return {nullptr, 0};
    }
    __builtin_unreachable();
}

constexpr uint32_t getAFMask(const GPIO_Alternate_Function mask){
    switch(mask){
        case GPIO_Alternate_Function::AF0: return 0x00;
        case GPIO_Alternate_Function::AF1: return 0x01;
        case GPIO_Alternate_Function::AF2: return 0x02;
        case GPIO_Alternate_Function::AF3: return 0x03;
        case GPIO_Alternate_Function::AF4: return 0x04;
        case GPIO_Alternate_Function::AF5: return 0x05;
        case GPIO_Alternate_Function::AF6: return 0x06;
        case GPIO_Alternate_Function::AF7: return 0x07;
    }
    __builtin_unreachable();
}

#endif //SHMINGO_HAL_SHAL_UART_REG_F072XB_H
