#include <cstdio>
#include "SHAL.h"

GPIO_Key gpios[6] = {
        GPIO_Key::A0,
        GPIO_Key::A1,
        GPIO_Key::A4,
        GPIO_Key::A5,
        GPIO_Key::A6,
        GPIO_Key::A7,
};

void timer2callback(){

    uint16_t val[6];

    for(int i = 0; i < 6; i++){
        val[i] = GPIOManager::get(gpios[i]).analogRead(SHAL_ADC_SampleTime::C8);
        SHAL_delay_ms(30);
    }

    char buff[64];
    sprintf(buff, "%d, %d, %d, %d, %d, %d\r\n", val[0],val[1],val[2],val[3],val[4],val[5]);

    SHAL_UART2.sendString(buff);

}

void b0PWM(){
    PIN(B0).toggle();
}

int main() {

    SHAL_init();

    SHAL_UART2.init(UART_Pair_Key::Tx2A2_Rx2A3);
    SHAL_UART2.begin(115200);

    PIN(A0).setPinMode(PinMode::ANALOG_MODE);
    PIN(A1).setPinMode(PinMode::ANALOG_MODE);
    PIN(A4).setPinMode(PinMode::ANALOG_MODE);
    PIN(A5).setPinMode(PinMode::ANALOG_MODE);
    PIN(A6).setPinMode(PinMode::ANALOG_MODE);
    PIN(A7).setPinMode(PinMode::ANALOG_MODE);

    PIN(B0).setPinMode(PinMode::OUTPUT_MODE);

    SHAL_TIM2.init(4000000,400);

    SHAL_TIM2.setCallbackFunc(timer2callback);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    SHAL_TIM6.init(4000000,1);
    SHAL_TIM6.setCallbackFunc(b0PWM);
    SHAL_TIM6.enableInterrupt();
    SHAL_TIM6.start();

    while (true) {
    }
}