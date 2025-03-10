cmake_minimum_required(VERSION 3.22)

set(MCU_ARCH Cortex-M3)

set(CHIP STM32F103C8T6)
#set(STARTUP_FILE  CACHE STRING "Path to the startup file")
string(TOUPPER "${CHIP}" CHIP_UPPER)
message(STATUS ${CHIP})

# Include architecture options
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/arch_options.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/driver.cmake)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/armclang.cmake)

project(smart_car C ASM)

add_subdirectory(drivers)
add_subdirectory(app)
add_subdirectory(board)
add_subdirectory(lib)
