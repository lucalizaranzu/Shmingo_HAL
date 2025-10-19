//
// Created by Luca on 9/7/2025.
//

#ifndef SHAL_UART_REG_L432KC_H
#define SHAL_UART_REG_L432KC_H

#include <stm32l432xx.h>
#include <cassert>

#include "SHAL_UART_TYPES.h"

#define NUM_USART_LINES 4

#define SHAL_UART1 UART(1)
#define SHAL_UART2 UART(2)


//Valid usart Tx and Rx pairings for STM32L432KC
enum class UART_Pair_Key : uint8_t{
    //UART1
    Tx1A9_Rx1A10,
    Tx1B6_Rx1B7,

    //UART2
    Tx2A2_Rx2A3,

    NUM_PAIRS,
    INVALID
};


static inline SHAL_UART_Pair getUARTPair(const UART_Pair_Key pair){
    switch(pair){
        case UART_Pair_Key::Tx1A9_Rx1A10: return {USART1, GPIO_Key::A9, GPIO_Key::A10, GPIO_Alternate_Function::AF7, GPIO_Alternate_Function::AF7};
        case UART_Pair_Key::Tx1B6_Rx1B7: return {USART1, GPIO_Key::B6, GPIO_Key::B7, GPIO_Alternate_Function::AF7, GPIO_Alternate_Function::AF7};
        case UART_Pair_Key::Tx2A2_Rx2A3: return {USART2, GPIO_Key::A2, GPIO_Key::A3, GPIO_Alternate_Function::AF7, GPIO_Alternate_Function::AF7};

        case UART_Pair_Key::NUM_PAIRS:
        case UART_Pair_Key::INVALID:
            __builtin_unreachable();
    }
    __builtin_unreachable();
}

static inline uint8_t getUARTChannel(const UART_Pair_Key pair){ //TODO remove?
    switch(pair){
        case UART_Pair_Key::Tx1A9_Rx1A10:
        case UART_Pair_Key::Tx1B6_Rx1B7:
            return 0;
        case UART_Pair_Key::Tx2A2_Rx2A3:
            return 1;
        case UART_Pair_Key::NUM_PAIRS:
        case UART_Pair_Key::INVALID:
            assert(false);
            return 0;
    }
    __builtin_unreachable();
}

constexpr SHAL_UART_Enable_Register getUARTEnableReg(const UART_Pair_Key pair){
    switch(pair){
        case UART_Pair_Key::Tx1A9_Rx1A10:
        case UART_Pair_Key::Tx1B6_Rx1B7:
            return {&RCC->APB2ENR,RCC_APB2ENR_USART1EN};
        case UART_Pair_Key::Tx2A2_Rx2A3:
            return {&RCC->APB1ENR1,RCC_APB1ENR1_USART2EN};
        case UART_Pair_Key::NUM_PAIRS:
        case UART_Pair_Key::INVALID:
            assert(false);
            return {nullptr, 0};
    }
    __builtin_unreachable();
}

static inline SHAL_UART_Control_Register_1 getUARTControlRegister1(UART_Pair_Key key){
    SHAL_UART_Control_Register_1 res = {nullptr, USART_CR1_UE, USART_CR1_TE, USART_CR1_RE};

    res.reg = &getUARTPair(key).USARTReg->CR1;
    return res;
};

static inline SHAL_UART_Baud_Rate_Generation_Register getUARTBaudRateGenerationRegister(UART_Pair_Key key){
    SHAL_UART_Baud_Rate_Generation_Register res = {nullptr}; //TODO un-hardcode if other devices have wider baud rate allowances

    res.reg = &getUARTPair(key).USARTReg->BRR;
    return res;
};

static inline SHAL_UART_Transmit_Data_Register getUARTTransmitDataRegister(UART_Pair_Key key){
    SHAL_UART_Transmit_Data_Register res = {nullptr}; //TODO un-hardcode if other devices have wider baud rate allowances

    res.reg = &getUARTPair(key).USARTReg->TDR;
    return res;
};

static inline SHAL_UART_ISR_FIFO_Disabled getUARTISRFifoDisabled(UART_Pair_Key key){
    SHAL_UART_ISR_FIFO_Disabled res = {nullptr, USART_ISR_TXE};

    res.reg = &getUARTPair(key).USARTReg->ISR;
    return res;
};

#endif //SHAL_UART_REG_F072XB_H
