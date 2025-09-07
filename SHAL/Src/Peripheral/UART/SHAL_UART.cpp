/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Related to USART and UART abstractions
  ******************************************************************************
  */


#include "SHAL_UART.h"
#include "SHAL_GPIO.h"

UART::UART(const UART_Pair pair){
    SHAL_UART_Pair uart_pair = getUARTPair(pair); //Get the UART_PAIR information to be initialized

    //Get the GPIO pins for this UART setup
    GPIO_Key Tx_Key = uart_pair.TxKey; //Tx pin
    GPIO_Key Rx_Key = uart_pair.RxKey; //Rx pin

    initGPIO(Tx_Key,PinMode::ALTERNATE_FUNCTION_MODE); //Initialize Tx GPIO with alternate function (initializes GPIO port as well)
    initGPIO(Rx_Key,PinMode::ALTERNATE_FUNCTION_MODE); //Initialize Rx GPIO with alternate function

    SHAL_UART_ENABLE_REG pairUARTEnable = getUARTEnableReg(pair); //Register and mask to enable the UART channel

    *pairUARTEnable.reg |= pairUARTEnable.mask; //Enable UART line
}


UART& UARTManager::get(UART_Pair pair) {

    //Perform logic for reassigning UART object in array

    return m_UARTs[getUARTChannel(pair)];
}
