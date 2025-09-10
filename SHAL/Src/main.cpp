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


    UART(2).init(UART_Pair::Tx2A2_Rx2A3);

    UART(2).begin(115200);

    PIN(C3).useAsExternalInterrupt(TriggerMode::RISING_EDGE,c3Interrupt);

    Timer timer2 = getTimer(Timer_Key::S_TIM2);

    PIN(A4).setPinMode(PinMode::OUTPUT_MODE);
    PIN(A5).setPinMode(PinMode::OUTPUT_MODE);

    timer2.setPrescaler(8000 - 1);
    timer2.setARR(1500 - 1);
    timer2.setCallbackFunc(tim2Handler);
    timer2.start();

    while (true) {
    	__WFI();
    }
}
