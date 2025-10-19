#include "SHAL.h"

#include <cstdlib>
#include <cstdio>

void togglePin() {

    //PIN(B4).toggle();
    //SHAL_UART2.sendString("Test\r\n");
}

void timer2callback(){
    auto val = PIN(B7).analogRead();

    char buf [6];
    sprintf (buf, "%d\r\n", val);

    SHAL_UART2.sendString(buf);
}

int main() {

    SHAL_init();


    PIN(B4).setPinMode(PinMode::OUTPUT_MODE);
    PIN(B4).setLow();

    PIN(B3).setPinMode(PinMode::OUTPUT_MODE);

    SHAL_UART2.init(UART_Pair_Key::Tx2A2_Rx2A3);

    SHAL_UART2.begin(115200);


    SHAL_UART2.sendString("Hello\r\n");


    SHAL_TIM2.init(8000000,1000);

    SHAL_TIM2.setCallbackFunc(timer2callback);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    //End setup

    while (true) {

    }
}
