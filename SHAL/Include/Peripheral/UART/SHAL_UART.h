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

    //Sends a string
    void sendString(const char* s);

    //Sends a char
    void sendChar(const char c);

private:

    UART() = default; //Initializer for array

    //Creates a UART based on a pair of two valid U(S)ART pins
    explicit UART(const UART_Pair pair);

    UART_Pair m_UARTPair = UART_Pair::INVALID;

};

class UARTManager{

public:

    static UART& get(UART_Pair);


    UARTManager() = delete;

private:

    inline static UART m_UARTs[NUM_USART_LINES] = {};

};

#endif //SHMINGO_HAL_SHAL_UART_H
