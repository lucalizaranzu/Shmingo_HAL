/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Relating to UART and USART object abstractions
  ******************************************************************************
  */

#ifndef SHMINGO_HAL_SHAL_UART_H
#define SHMINGO_HAL_SHAL_UART_H

#include "SHAL_UART_REG.h"

class UART{

    friend class UARTManager;
public:

    //begins Tx and Usart TODO either modify this function or add a new one that supports Rx
    void begin(uint32_t baudRate) volatile;

    //Sends a string
    void sendString(const char* s) volatile;

    //Sends a char
    void sendChar(char c) volatile;

private:

    UART() = default; //Initializer for array

    //Creates a UART based on a pair of two valid U(S)ART pins
    explicit UART(UART_Pair pair);

    UART_Pair m_UARTPair = UART_Pair::INVALID;

};


#define getUART(uart_pair) UARTManager::get(uart_pair)

class UARTManager{

public:

    static UART& get(UART_Pair pair);


    UARTManager() = delete;

private:

    inline static UART m_UARTs[NUM_USART_LINES] = {};

};

#endif //SHMINGO_HAL_SHAL_UART_H
