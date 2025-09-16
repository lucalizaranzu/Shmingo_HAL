#include "SHAL.h"
#include "stm32f0xx.h"


void c3Interrupt(){
    PIN(A5).toggle();
    SHAL_UART2.sendString("New test");
}

void tim2Handler(){
    PIN(A4).toggle();
}

int main() {

    SHAL_init();

    //Setup UART2 (used by nucleo devices for USB comms)
    SHAL_UART2.init(UART_Pair::Tx2A2_Rx2A3);
    SHAL_UART2.begin(115200);

    SHAL_I2C1.init(I2C_Pair::SCL1B6_SDA1B7);
    SHAL_I2C1.setClockConfig(0x2000090E);

    //Use pin C3 to trigger a function on external interrupt
    PIN(C3).useAsExternalInterrupt(TriggerMode::RISING_EDGE,c3Interrupt);

    SHAL_TIM2.init(8000-1,1500-1);
    SHAL_TIM2.setCallbackFunc(tim2Handler);
    SHAL_TIM2.start();

    PIN(A4).setPinMode(PinMode::OUTPUT_MODE);
    PIN(A5).setPinMode(PinMode::OUTPUT_MODE);

    //Temporary setup for DHT20

    PIN(A5).setLow();

    SHAL_delay_ms(5000); //Wait 100 ms from datasheet

    PIN(A5).setHigh();

    uint8_t initByte[1] = {0x71};

    uint8_t status = SHAL_I2C1.masterWriteReadByte(0x38,initByte,1);

    if ((status & 0x18) != 0x18) {
        SHAL_UART2.sendString("DHT ready");
    } else {
        SHAL_UART2.sendString("DHT broke");
    }

    PIN(A5).setLow();

    //End setup

    while (true) {
    	__WFI();
    }
}
