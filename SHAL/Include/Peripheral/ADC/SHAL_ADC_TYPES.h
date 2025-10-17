//
// Created by Luca on 9/21/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_TYPES_H
#define SHMINGO_HAL_SHAL_ADC_TYPES_H

//Common register among all ADC peripherals
struct SHAL_ADC_Common_Control_Reg {
    volatile uint32_t* reg;
    uint32_t VoltageRefEnable;
    uint32_t TempSensorEnable;
    uint32_t VBatteryEnable;
};

//Register controlling the ADC peripheral clock
struct SHAL_ADC_RCC_Enable_Reg {
    volatile uint32_t* reg;
    uint32_t mask;
};

//Register with ADC controls
struct SHAL_ADC_Control_Reg {
    volatile uint32_t* reg;
    uint32_t enable_mask;
    uint32_t disable_mask;
    uint32_t calibration_mask;
    uint32_t start_mask;
};

//Register controlling ADC configuration
struct SHAL_ADC_Config_Reg {
    volatile uint32_t* reg;
    uint32_t continue_mask;

    uint32_t resolution_offset;
    uint32_t alignment_offset;
};

//Register for all ADC data
struct SHAL_ADC_Data_Reg {
    volatile uint32_t* reg;
    uint32_t mask;
};

//Register for the interrupt service routine for ADCs
struct SHAL_ADC_ISR_Reg {
    volatile uint32_t* reg;
    uint32_t end_of_conversion_mask;
    uint32_t end_of_sequence_mask;
    uint32_t ready_mask;
};

//Register controlling the clock source for the ADC
struct SHAL_ADC_Clock_Reg {
    volatile uint32_t* reg;
    uint32_t clear;
    uint32_t mask;
};

//Register controlling the sampling time of ADC samples
struct SHAL_ADC_Channel_Sampling_Time_Reg {
    volatile uint32_t* reg;
    uint32_t channel_offset;
};

//Register controlling the number of conversions to do in one sequence
struct SHAL_ADC_Sequence_Amount_Reg {
    volatile uint32_t* reg;
    uint32_t offset;
};

/*Register group controlling which ADC channels to convert. DO NOT USE THE FOLLOWING ILLEGAL COMBINATIONS:
*reg 1 + offset 1
*Any sections after the last one (for example, max for a 16 channel register is reg 4 offset 2*/
struct SHAL_ADC_Sequence_Reg {
    volatile uint32_t* regs[6];

    uint32_t offsets[5];
};




enum class SHAL_ADC_SampleTime : uint32_t {
    C1          = 0x00, //1.5 cycles per sample (F0 only, timings change on other ADC architectures)
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
