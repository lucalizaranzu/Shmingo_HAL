#include "SHAL.h"
#include "stm32f0xx.h"


void c3Interrupt(){
    SHAL_UART2.sendString("Begin\r\n");

    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    SHAL_I2C1.masterWrite(0x38, cmd, 3);

    SHAL_UART2.sendString("Hello\r\n");

    SHAL_delay_ms(100);

    uint8_t buffer[7] = {0};

    SHAL_UART2.sendString("Buffer created?\r\n");

    //Read 7 bytes (status + 5 data + CRC)
    SHAL_I2C1.masterRead(0x38, buffer, 7);

    SHAL_UART2.sendString("Read complete\r\n");

    //Parse humidity (20 bits)
    uint32_t rawHumidity = ((uint32_t)buffer[1] << 12) |
                           ((uint32_t)buffer[2] << 4) |
                           ((uint32_t)buffer[3] >> 4);

    // Parse temperature (20 bits)
    uint32_t rawTemp = (((uint32_t)buffer[3] & 0x0F) << 16) |
                       ((uint32_t)buffer[4] << 8) |
                       ((uint32_t)buffer[5]);

    float humidity = (rawHumidity * 100.0f) / 1048576.0f;     // 2^20 = 1048576
    float temperature = (rawTemp * 200.0f) / 1048576.0f - 50.0f;

    char buf[64];
    sprintf(buf, "Temp: %.2f C, Hum: %.2f %%\r\n", temperature, humidity);
    SHAL_UART2.sendString(buf);
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



    SHAL_delay_ms(3000); //Wait 100 ms from datasheet

    uint8_t cmd = 0x71;
    uint8_t status = 0;

    SHAL_I2C1.masterWriteRead(0x38, &cmd, 1, &status, 1);

    char statusString[32];
    sprintf(statusString, "Status = 0x%02X\r\n", status);
    SHAL_UART2.sendString(statusString);

    //End setup

    while (true) {
    	__WFI();
    }
}
