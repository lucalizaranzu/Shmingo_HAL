#include <cstdio>
#include "SHAL.h"


#define NUM_CHANNELS 6

// Physical order on right-side header: A0, A1, A3, A4, A5, A6, A7
SHAL_ADC_Channel channels[NUM_CHANNELS] = {
        SHAL_ADC_Channel::CH5,
        SHAL_ADC_Channel::CH6,
        SHAL_ADC_Channel::CH8,
        SHAL_ADC_Channel::CH9,
        SHAL_ADC_Channel::CH10,
        SHAL_ADC_Channel::CH12,
};

bool isDeviceOn = false;
bool shouldToggleDeviceState = true;
bool shouldCheckSensorThresholds = true;

uint16_t vals[NUM_CHANNELS] = {0,0,0,0,0,0};
uint8_t currentSensor = 0;

bool isAlarmBeeping = false;

uint16_t sensorThresholds[NUM_CHANNELS] = {0,0,0,0,0,0};

int buzzer_beepCount = 0;
bool isBeepingForCalibration = false;

bool prevIsCalibrateButtonHigh = false;

int cyclesPerPrint = 2;
int currentCycle = 0;

bool areSensorRequirementsMetCurrent = false;
bool areSensorRequirementsMetPrevious = false;

void getSensorData(){

    vals[currentSensor] = SHAL_ADC1.singleConvertSingle(channels[currentSensor]);

    if(currentSensor == (NUM_CHANNELS - 1) && currentCycle == cyclesPerPrint - 1){
        char buff[125];
        // Print in the same order as the channels[] array (physical order)
        sprintf(buff, "A0:%u,A1:%u,A3:%u,A4:%u,A5:%u,A6:%u\r\n",
                vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);
        SHAL_UART2.sendString(buff);
    }

    currentSensor = (currentSensor + 1) % NUM_CHANNELS;
    currentCycle = (currentCycle + 1) % cyclesPerPrint;

}

void startBeeping(){

    SHAL_TIM6.setPrescaler(4000);
    SHAL_TIM6.setARR(200);

    SHAL_TIM6.start();
}

void stopBeeping(){
    SHAL_TIM1.stop();
    SHAL_TIM6.stop();
    isAlarmBeeping = false;
    isBeepingForCalibration = false;
}

void checkSensorThresholds(){

    bool localFlag = true;

    for(int i = 0; i < NUM_CHANNELS; i++){
        if(vals[i] < sensorThresholds[i]){
            areSensorRequirementsMetCurrent = false; //Conditions not met
            localFlag = false;
            break;
        }
    }
    if(localFlag){
        areSensorRequirementsMetCurrent = true;
    }

    if(areSensorRequirementsMetCurrent){
        if(!areSensorRequirementsMetPrevious){
            SHAL_TIM1.stop();
            SHAL_TIM6.stop();
            SHAL_TIM15.stop();
            PIN(A9).setLow();
            stopBeeping();
        }
    }
    else{
        if(areSensorRequirementsMetPrevious){
            SHAL_TIM15.start();
            PIN(A9).setHigh();
        }
    }
    areSensorRequirementsMetPrevious = areSensorRequirementsMetCurrent;
}

void calibrateThresholds(){

    // Read every channel once and set threshold to 80% of reading
    for(int i = 0; i < NUM_CHANNELS; i++){
        uint16_t sensorVal = vals[i];

        if(sensorVal < 50){
            sensorVal = 0;
        }
        else{
            sensorVal = sensorVal - 50;
        }
        sensorThresholds[i] = sensorVal;
    }

    char buff[125];
    // Print in the same order as the channels[] array (physical order)
    sprintf(buff, "Thresholds calibrated to: A0:%u,A1:%u,A3:%u,A4:%u,A5:%u,A6:%u\r\n",
            sensorThresholds[0], sensorThresholds[1], sensorThresholds[2], sensorThresholds[3], sensorThresholds[4], sensorThresholds[5]);
    SHAL_UART2.sendString(buff);
}

void PWMToggle(){

    //Flash light
    PIN(A9).toggle();

    SHAL_TIM15.stop(); //Stop timer for allowed time off sensors

    if(isBeepingForCalibration && buzzer_beepCount > 2){
        isBeepingForCalibration = false;
        buzzer_beepCount = 0;
        SHAL_TIM6.stop(); //Reset timer 6
        SHAL_TIM1.stop(); //Stop buzzer

        SHAL_TIM6.setPrescaler(4000);
        SHAL_TIM6.setARR(400);
    }

    if(!isAlarmBeeping){
        SHAL_TIM1.start();
        buzzer_beepCount++;
    }
    else{
        SHAL_TIM1.stop();
    }
    isAlarmBeeping = !isAlarmBeeping;
}

void buttonHoldCallback(){

    shouldCheckSensorThresholds = false; //Dont check sensor thresholds yet, ensure that calibration beep happens

    SHAL_TIM7.stop(); //Stop this timer

    SHAL_TIM2.stop(); //Stop reading from ADC

    buzzer_beepCount = 0;
    isBeepingForCalibration = true;

    SHAL_TIM6.init(4000,50);
    SHAL_TIM6.start();

    calibrateThresholds();
    SHAL_TIM1.start();

    SHAL_TIM2.start(); //Restart value checks

    shouldToggleDeviceState = false;
    shouldCheckSensorThresholds = true;
}

int main() {

    SHAL_init();

    //SHAL_UART2.init(UART_Pair_Key::Tx2A2_Rx2A3);
    //SHAL_UART2.begin(115200);

    PIN(A0).setPinMode(PinMode::ANALOG_MODE);
    PIN(A1).setPinMode(PinMode::ANALOG_MODE);
    PIN(A3).setPinMode(PinMode::ANALOG_MODE);
    PIN(A4).setPinMode(PinMode::ANALOG_MODE);
    PIN(A5).setPinMode(PinMode::ANALOG_MODE);
    PIN(A6).setPinMode(PinMode::ANALOG_MODE);
    PIN(A7).setPinMode(PinMode::ANALOG_MODE);

    PIN(B6).setPinMode(PinMode::INPUT_MODE);
    PIN(A9).setPinMode(PinMode::OUTPUT_MODE);
    PIN(B0).setAlternateFunction(GPIO_Alternate_Function_Mapping::B0_TIM1CH2N);

    PIN(A8).setPinMode(PinMode::OUTPUT_MODE);
    PIN(A8).setInternalResistor(InternalResistorType::NO_PULL);

    SHAL_TIM2.init(4000,200);

    SHAL_TIM2.setCallbackFunc(getSensorData);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    SHAL_TIM1.init(0,2400); //PWM signal
    SHAL_TIM1.setPWMMode(SHAL_Timer_Channel::CH2,SHAL_TIM_Output_Compare_Mode::PWMMode1,SHAL_Timer_Channel_Main_Output_Mode::Polarity_Normal,SHAL_Timer_Channel_Complimentary_Output_Mode::Polarity_Reversed);
    SHAL_TIM1.setPWMDutyCycle(900);

    SHAL_TIM6.init(4000,500); //PWM switcher
    SHAL_TIM6.setCallbackFunc(PWMToggle);
    SHAL_TIM6.enableInterrupt();

    SHAL_TIM7.init(4000,3000); //Calibrate timer
    SHAL_TIM7.setCallbackFunc(buttonHoldCallback);
    SHAL_TIM7.enableInterrupt();

    SHAL_TIM15.init(4000,5000); //5 seconds
    SHAL_TIM15.setCallbackFunc(startBeeping);
    SHAL_TIM15.enableInterrupt();


    SHAL_UART2.sendString("Hello3\r\n");

    while (true) { //TODO set to use button for simulating off sensor, uncomment for real functionality

        if(PIN(B6).digitalRead() != 1){

            if(prevIsCalibrateButtonHigh){
                SHAL_TIM7.start();
            }
            prevIsCalibrateButtonHigh = false;

        }
        else{
            if(!prevIsCalibrateButtonHigh){
                if(shouldToggleDeviceState){
                    if(!isDeviceOn){ //Turn device on
                        PIN(A8).setHigh();
                        isDeviceOn = true;
                    }
                    else{ //Turn device off
                        PIN(A8).setLow();
                        PIN(A9).setLow();
                        isDeviceOn = false;

                        areSensorRequirementsMetCurrent = true;
                        areSensorRequirementsMetPrevious = true;

                        stopBeeping();
                    }

                }

                shouldToggleDeviceState = true;

                SHAL_TIM7.stop();
            }
            prevIsCalibrateButtonHigh = true;
        }

        if(isDeviceOn && shouldCheckSensorThresholds){
            checkSensorThresholds();
        }
    }
}
