.PHONY: all build cmake clean format

# --- Configurable variables ---
BUILD_DIR    := build
BUILD_TYPE   ?= Debug
TOOLCHAIN    := gcc-arm-none-eabi.cmake

CFLAGS_DEBUG   ?= -g3 -O0
CXXFLAGS_DEBUG ?= -g3 -O0

# MCU target (override on command line: make build MCU_MODEL=STM32F051x8)
MCU_MODEL    ?= STM32L432xx
MCU_FAMILY   ?= STM32L4xx

# --- Default target ---
all: build

# --- Configure step (runs CMake if build dir missing or stale) ---
${BUILD_DIR}/build.ninja:
	cmake \
		-G Ninja \
		-B ${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_C_FLAGS_DEBUG="${CFLAGS_DEBUG}" \
        -DCMAKE_CXX_FLAGS_DEBUG="${CXXFLAGS_DEBUG}" \
		-DMCU_MODEL=$(MCU_MODEL) \
		-DMCU_FAMILY=$(MCU_FAMILY)

cmake: ${BUILD_DIR}/build.ninja

# --- Build step ---
build: cmake
	cmake --build ${BUILD_DIR}

# --- Format all sources ---
SRCS := $(shell find . -type f \( -name '*.[ch]' -o -name '*.[ch]pp' \))
format: $(addsuffix .format,${SRCS})

%.format: %
	clang-format -i $<

# --- Clean ---
clean:
	rm -rf ${BUILD_DIR} compile_commands.json
