//
// Created by Luca on 9/21/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_TYPES_H
#define SHMINGO_HAL_SHAL_ADC_TYPES_H


struct SHAL_ADC_Common_Control_Reg {

    volatile uint32_t* reg;
    uint32_t VoltageRefEnable;
    uint32_t TempSensorEnable;
    uint32_t VBatteryEnable;

};

struct SHAL_ADC_RCC_Enable_Reg {
    volatile uint32_t* reg;
    uint32_t mask;
};

struct SHAL_ADC_Control_Reg {
    volatile uint32_t* reg;
    uint32_t enable_mask;
    uint32_t disable_mask;
    uint32_t calibration_mask;
    uint32_t start_mask;
};

struct SHAL_ADC_Config_Reg {
    volatile uint32_t* reg;
    uint32_t continue_mask;

    uint32_t resolution_offset;
    uint32_t alignment_offset;
};

struct SHAL_ADC_Data_Reg {
    volatile uint32_t* reg;
    uint32_t mask;
};

struct SHAL_ADC_ISR {

    volatile uint32_t* reg;
    uint32_t end_of_conversion_mask;
};

struct SHAL_ADC_Clock_Reg {
    volatile uint32_t* reg;
    uint32_t clear;
    uint32_t mask;
};

struct SHAL_ADC_Channel_Sampling_Time_Reg {
    volatile uint32_t* reg;
    uint32_t channel_offset;
};


enum class SHAL_ADC_Channel : uint32_t {
    CH0,
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
    CH8,
    CH9,
    CH10,
    CH11,
    CH12,
    CH13,
    CH14,
    CH15,
    CHTemp,
    CHRef,
    CHBat
};

enum class ADC_SampleTime : uint32_t {
    C1          = 0x00, //1.5 cycles per sample F0
    C2          = 0x01, //7.5 cycles
    C3          = 0x02, //13.5 cycles
    C4          = 0x03, //28.5 cycles
    C5          = 0x04, //41.5 cycles
    C6          = 0x05, //55.5 cycles
    C7          = 0x06, //71.5 cycles
    C8          = 0x07  //239.5 cycles
};

enum class SHAL_ADC_Resolution : uint8_t {
    B12         = 0x00,
    B10         = 0x01,
    B8          = 0x02,
    B6          = 0x03,
};

enum class SHAL_ADC_Alignment : uint8_t {
    RIGHT       = 0x00,
    LEFT        = 0x01,
};

#endif //SHMINGO_HAL_SHAL_ADC_TYPES_H
