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

bool isAlarmBeeping = false;
bool isCalibrateButtonHigh = false;

uint16_t sensorThresholds[6] = {4096,4096,4096,4096,4096,4096};

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

void PWMToggle(){

    if(!isAlarmBeeping){
        SHAL_TIM1.start();
    }
    else{
        SHAL_TIM1.stop();
    }
    isAlarmBeeping = !isAlarmBeeping;
}

void buttonCallback(){

}

void calibrateSensors(){
    PIN(B3).setHigh();
    SHAL_TIM7.stop();
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

    PIN(B0).setAlternateFunction(GPIO_Alternate_Function_Mapping::B0_TIM1CH2N);

    PIN(B6).setPinMode(PinMode::INPUT_MODE);
    PIN(B6).useAsExternalInterrupt(TriggerMode::RISING_FALLING_EDGE,buttonCallback);

    SHAL_TIM2.init(4000000,400);

    SHAL_TIM2.setCallbackFunc(timer2callback);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    SHAL_TIM1.init(300,999);
    SHAL_TIM1.setPWMMode(SHAL_Timer_Channel::CH2,SHAL_TIM_Output_Compare_Mode::PWMMode1,SHAL_Timer_Channel_Main_Output_Mode::Disabled,SHAL_Timer_Channel_Complimentary_Output_Mode::Polarity_Reversed);
    SHAL_TIM1.setPWMDutyCycle(499);
    SHAL_TIM1.start();

    SHAL_TIM6.init(4000000,400);
    SHAL_TIM6.setCallbackFunc(PWMToggle);
    SHAL_TIM6.enableInterrupt();
    SHAL_TIM6.start();

    SHAL_TIM7.init(4000000,3000);
    SHAL_TIM7.setCallbackFunc(calibrateSensors);
    SHAL_TIM7.enableInterrupt();

    PIN(B3).setPinMode(PinMode::OUTPUT_MODE); //Test

    while (true) {
        SHAL_UART2.sendString("HELLO\r\n");
    }
}