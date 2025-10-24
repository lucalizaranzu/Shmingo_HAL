#include "SHAL.h"

void timer2callback(){
    uint16_t val = PIN(A5).analogRead(SHAL_ADC_SampleTime::C4);

    if(val <= 600){
        PIN(B3).setHigh();
    }
    else{
        PIN(B3).setLow();
    }
}

int main() {

    SHAL_init();

    PIN(B3).setPinMode(PinMode::OUTPUT_MODE);
    PIN(A5).setPinMode(PinMode::ANALOG_MODE);

    SHAL_TIM2.init(4000000,50);

    SHAL_TIM2.setCallbackFunc(timer2callback);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    while (true) {

    }
}