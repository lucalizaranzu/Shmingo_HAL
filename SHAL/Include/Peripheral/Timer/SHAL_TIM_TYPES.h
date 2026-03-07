//
// Created by Luca on 9/7/2025.
//

#ifndef SHAL_TIM_TYPES_H
#define SHAL_TIM_TYPES_H

#include "SHAL_CORE.h"

struct SHAL_TIM_Info{
    volatile TIM_TypeDef* timer;
    IRQn_Type IRQn;
    uint8_t numChannels;
};

struct SHAL_TIM_RCC_Register{
    volatile uint32_t* reg;
    uint32_t enable_mask;
};

struct SHAL_TIM_Control_Register_1 {
    volatile uint32_t* reg;
    uint32_t counter_enable_mask;
    uint32_t update_disable_mask;
    uint32_t one_pulse_mode_mask;
    uint32_t center_align_mode_offset;
    uint32_t auto_reload_preload_enable_mask;
};

struct SHAL_TIM_DMA_Interrupt_Enable_Register {
    volatile uint32_t* reg;
    uint32_t update_interrupt_enable_mask;
};

struct SHAL_TIM_Status_Register {
    volatile uint32_t* reg;
    uint32_t update_interrupt_flag_mask;
};

struct SHAL_TIM_Event_Generation_Register {
    volatile uint32_t* reg;
    uint32_t update_generation_mask;
};

struct SHAL_TIM_Prescaler_Register {
    volatile uint32_t* reg;
    uint32_t offset;
};

struct SHAL_TIM_Auto_Reload_Register {
    volatile uint32_t* reg;
    uint32_t offset;
};

struct SHAL_TIM_Capture_Compare_Mode_Registers_Input {
    volatile uint32_t* regs;
    uint32_t input_capture_1_filter_offset;
    uint32_t input_capture_1_prescaler_offset;
    uint32_t capture_compare_1_selection_offset;
    uint32_t input_capture_2_filter_offset;
    uint32_t input_capture_2_prescaler_offset;
    uint32_t capture_compare_2_selection_offset;
};

struct SHAL_TIM_Output_Capture_Compare_Mode_Register {
    volatile uint32_t* reg;
    uint32_t capture_compare_1_selection_offset;
    uint32_t output_compare_1_fast_enable_mask;
    uint32_t output_compare_1_preload_enable_mask;
    uint32_t output_compare_1_mode_offset;
    uint32_t output_compare_1_clear_enable_mask;
    uint32_t capture_compare_2_selection_offset;
    uint32_t output_compare_2_fast_enable_mask;
    uint32_t output_compare_2_preload_enable_mask;
    uint32_t output_compare_2_mode_offset;
    uint32_t output_compare_2_clear_enable_mask;
};

struct SHAL_TIM_Break_Dead_Time_Register {
    volatile uint32_t* reg;
    uint32_t dead_time_offset;                    // [7:0]   DTG - Dead-time generator setup
    uint32_t lock_configuration_offset;           // [9:8]   LOCK - Lock configuration
    uint32_t off_state_selection_idle_mask;       // [10]    OSSI - Off-state selection for idle mode
    uint32_t off_state_selection_run_mask;        // [11]    OSSR - Off-state selection for run mode
    uint32_t break_enable_mask;                   // [12]    BKE - Break enable
    uint32_t break_polarity_mask;                 // [13]    BKP - Break polarity
    uint32_t automatic_output_enable_mask;        // [14]    AOE - Automatic output enable
    uint32_t main_output_enable_mask;             // [15]    MOE - Main output enable
};

struct SHAL_TIM_Capture_Compare_Enable_Register {
    volatile uint32_t* reg;
    uint32_t cc_output_enable_offset;
    uint32_t cc_output_polarity_offset;
    uint32_t cc_complimentary_output_enable_offset;
    uint32_t cc_complimentary_output_polarity_offset;
};

struct SHAL_TIM_Capture_Compare_Register {
    volatile uint32_t* reg;
    uint32_t offset;
};



enum class SHAL_TIM_Output_Compare_Mode : uint8_t {
    Frozen                      = 0b000,  //Output compare frozen
    ActiveOnMatch               = 0b001,  //Set output to active level on match
    InactiveOnMatch             = 0b010,  //Set output to inactive level on match
    Toggle                      = 0b011,  //Toggle output on match
    ForceInactive               = 0b100,  //Force output to inactive
    ForceActive                 = 0b101,  //Force output to active
    PWMMode1                    = 0b110,  //PWM mode 1 (active until compare match)
    PWMMode2                    = 0b111,  //PWM mode 2 (inactive until compare match)
};

enum class SHAL_TIM_Output_Compare_Preload : uint8_t {
    Disabled = 0b0,  //CCRx register is updated immediately
    Enabled  = 0b1,  //CCRx register is buffered; updated on update event (UEV)
};

enum class SHAL_Timer_Channel : uint8_t { //TODO change if other timers have fewer than 6 channels
    CH1                         = 1,
    CH2                         = 2,
    CH3                         = 3,
    CH4                         = 4,
};

enum class SHAL_Timer_Channel_Main_Output_Mode : uint8_t {
    Disabled                        = 0b00,
    Polarity_Normal                 = 0b01,
    Polarity_Reversed               = 0b11,
};

enum class SHAL_Timer_Channel_Complimentary_Output_Mode : uint8_t {
    Disabled                        = 0b00,
    Polarity_Normal                 = 0b01,
    Polarity_Reversed               = 0b11,
};


#endif //SHAL_TIM_TYPES_H
