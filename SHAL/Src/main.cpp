#include "SHAL.h"
#include "stm32f0xx.h"

#include <stdlib.h>

void c3Interrupt(){
    SHAL_UART2.sendString("Begin\r\n");

    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    SHAL_I2C1.masterWrite(0x38, cmd, 3);

    SHAL_delay_ms(100);

    uint8_t dht_buf[7] = {0};

    //Read 7 bytes (status + 5 data + CRC)
    SHAL_I2C1.masterRead(0x38, dht_buf, 7);

    //Parse humidity (20 bits)
    uint32_t rawHumidity = ((uint32_t)dht_buf[1] << 12) |
                           ((uint32_t)dht_buf[2] << 4) |
                           ((uint32_t)dht_buf[3] >> 4);

    uint32_t rawTemp = (((uint32_t)dht_buf[3] & 0x0F) << 16) |
                       ((uint32_t)dht_buf[4] << 8) |
                       ((uint32_t)dht_buf[5]);

    // Use 64-bit intermediate to avoid overflow
    uint32_t hum_hundredths = (uint32_t)(((uint64_t)rawHumidity * 10000ULL) >> 20);
    int32_t temp_hundredths = (int32_t)((((uint64_t)rawTemp * 20000ULL) >> 20) - 5000);

    char out[80];
    sprintf(out, "rawH=0x%05lX rawT=0x%05lX\r\n",
            (unsigned long)rawHumidity, (unsigned long)rawTemp);
    SHAL_UART2.sendString(out);

    // print as X.YY
    sprintf(out, "Temp: %ld.%02ld C, Hum: %ld.%02ld %%\r\n",
            (long)(temp_hundredths / 100), (long)(abs(temp_hundredths % 100)),
            (long)(hum_hundredths / 100), (long)(hum_hundredths % 100));
    SHAL_UART2.sendString(out);
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


    SHAL_delay_ms(10);

    c3Interrupt();
    //End setup

    while (true) {
    	__WFI();
    }
}
