#include "SHAL.h"
#include "stm32f0xx.h"


void c3Interrupt(){
    PIN(A5).toggle();
    UART(2).sendString("New test");
}

void tim2Handler(){
    PIN(A4).toggle();
}

int main() {

    //Setup UART2 (used by nucleo devices for USB comms)
    SHAL_UART2.init(UART_Pair::Tx2A2_Rx2A3);
    SHAL_UART2.begin(115200);

    //Use pin C3 to trigger a function on external interrupt
    PIN(C3).useAsExternalInterrupt(TriggerMode::RISING_EDGE,c3Interrupt);

    SHAL_TIM2.init(8000-1,1500-1);
    SHAL_TIM2.setCallbackFunc(tim2Handler);
    SHAL_TIM2.start();

    PIN(A4).setPinMode(PinMode::OUTPUT_MODE);
    PIN(A5).setPinMode(PinMode::OUTPUT_MODE);

    while (true) {
    	__WFI();
    }
}
