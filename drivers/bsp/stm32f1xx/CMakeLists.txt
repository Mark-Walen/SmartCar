if (CHIP_UPPER MATCHES "STM32F103.(.).*")
    if (CMAKE_MATCH_1 STREQUAL "6")
        set(FLASH STM32F10X_LD)
        set(STARTUP STM32F103x6)
    elseif (CMAKE_MATCH_1 STREQUAL "8" OR CMAKE_MATCH_1 STREQUAL "B")
        set(FLASH STM32F10X_MD)
        set(STARTUP STM32F103xB)
    elseif (CMAKE_MATCH_1 STREQUAL "C" OR CMAKE_MATCH_1 STREQUAL "D" OR CMAKE_MATCH_1 STREQUAL "E")
        set(FLASH STM32F10X_HD)
        set(STARTUP STM32F103xE)
    elseif (CMAKE_MATCH_1 STREQUAL "F" OR CMAKE_MATCH_1 STREQUAL "G")
        set(FLASH STM32F10X_HD)
        set(STARTUP STM32F103xG)
    endif ()
endif ()

add_library(bsp STATIC
        # BSP Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/time.c
)
#message(STATUS "${STARTUP} ${FLASH}")
target_compile_definitions(bsp PRIVATE USE_HAL_DRIVER STM32F103xB ${STARTUP} ${FLASH})

target_link_libraries(bsp PRIVATE hal)

target_include_directories(bsp PRIVATE
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/config/stm32f1xx
        ${CMAKE_SOURCE_DIR}/drivers/
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/inc
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/inc/Legacy
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/inc
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/Device/ST/STM32F1xx/Include
)
