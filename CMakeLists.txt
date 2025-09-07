cmake_minimum_required(VERSION 3.19)

project(shmingo-HAL)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(MCU_FAMILY "STM32F0xx" CACHE STRING "MCU family")
set(MCU_MODEL "" CACHE STRING "MCU model")

set(CPU_PARAMETERS 
    -mcpu=cortex-m0
    -mthumb)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/MX/startup_stm32f072rbtx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/MX/STM32F072RBTX_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(MX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
        SHAL/Include/Core/
        SHAL/Include/Peripheral/Timer
        SHAL/Include/Peripheral/Timer/Reg
        SHAL/Include/Peripheral/GPIO
        SHAL/Include/Peripheral/GPIO/Reg
        SHAL/Include/Peripheral/UART
        SHAL/Include/Peripheral/UART/Reg
        SHAL/Include/Peripheral/EXT/
        ${CMAKE_CURRENT_SOURCE_DIR}/SHAL/Include
)

file(GLOB_RECURSE PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/SHAL/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/SHAL/*.cpp
)

add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT}
)

target_compile_definitions(${EXECUTABLE} PRIVATE
    ${MCU_MODEL}
)

target_include_directories(${EXECUTABLE} PRIVATE
    ${MX_INCLUDE_DIRECTORIES}
    ${PROJECT_INCLUDE_DIRECTORIES}
)

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wsuggest-override>
    
        $<$<CONFIG:Debug>:-Og -g3 -ggdb>
        $<$<CONFIG:Release>:-Og -g0>
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.bin
)
