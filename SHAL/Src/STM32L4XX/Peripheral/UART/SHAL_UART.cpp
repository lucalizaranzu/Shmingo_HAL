/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Related to USART and SHAL_UART abstractions
  ******************************************************************************
  */


#include "SHAL_UART.h"
#include "SHAL_GPIO.h"

void SHAL_UART::init(UART_Pair_Key pair){

    m_key = pair;

    SHAL_UART_Pair uart_pair = getUARTPair(pair); //Get the UART_PAIR information to be initialized

    //Get the SHAL_GPIO pins for this SHAL_UART setup
    GPIO_Key Tx_Key = uart_pair.TxKey; //Tx pin
    GPIO_Key Rx_Key = uart_pair.RxKey; //Rx pin

    GET_GPIO(Tx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);
    GET_GPIO(Rx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    GET_GPIO(Tx_Key).setAlternateFunction(uart_pair.TxAlternateFunctionMask);
    GET_GPIO(Rx_Key).setAlternateFunction(uart_pair.RxAlternateFunctionMask);

    SHAL_UART_Enable_Register pairUARTEnable = getUARTEnableReg(pair); //Register and mask to enable the SHAL_UART channel

    *pairUARTEnable.reg |= pairUARTEnable.mask; //Enable SHAL_UART line


}

void SHAL_UART::begin(uint32_t baudRate) volatile {

    USART_TypeDef* usart = getUARTPair(m_key).USARTReg;

    auto control_reg = getUARTControlRegister1(m_key);

    SHAL_clear_bitmask(control_reg.reg, control_reg.usart_enable_mask); //Clear enable bit (turn off usart)

    usart->CR1 = 0; //Clear USART config

    SHAL_apply_bitmask(control_reg.reg, control_reg.transmit_enable_mask); //Enable Tx
    SHAL_apply_bitmask(control_reg.reg, control_reg.receive_enable_mask);  //Enable Rx

    auto baud_rate_reg = getUARTBaudRateGenerationRegister(m_key);

    uint32_t adjustedBaudRate = SystemCoreClock / baudRate;

    SHAL_set_register_value(baud_rate_reg.reg,adjustedBaudRate);

    SHAL_apply_bitmask(control_reg.reg, control_reg.usart_enable_mask); //Turn on usart
}

void SHAL_UART::sendString(const char *s) volatile {
    while (*s) {//Send chars while we haven't reached end of s
        sendChar(*s++);
    }
}

void SHAL_UART::sendChar(char c) volatile {

    auto ISR_non_fifo = getUARTISRFifoDisabled(m_key);

    if(!SHAL_WAIT_FOR_CONDITION_MS((*ISR_non_fifo.reg & ISR_non_fifo.transmit_data_register_empty_mask) != 0, 500)){
        PIN(B3).toggle();
        return;
    }

    auto transmit_reg = getUARTTransmitDataRegister(m_key);
    SHAL_set_register_value_16(transmit_reg.reg, static_cast<uint16_t>(c));
}



SHAL_UART& UARTManager::get(uint8_t uart) {

    if(uart > NUM_USART_LINES - 1){
        assert(false);
        //Memory fault
    }

    return m_UARTs[uart];
}
