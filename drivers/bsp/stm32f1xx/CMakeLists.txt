add_library(stm32f1xx STATIC
        # BSP Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/uart.c
)

target_compile_definitions(stm32f1xx PRIVATE USE_HAL_DRIVER ${STARTUP} ${FLASH})

target_link_libraries(stm32f1xx PRIVATE hal lwrb)

target_include_directories(stm32f1xx PRIVATE
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/common/inc
        ${CMAKE_SOURCE_DIR}/config/stm32f1xx
        ${CMAKE_SOURCE_DIR}/drivers/bsp/inc
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/inc
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/inc/Legacy
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/inc
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/drivers/hal/stm32f1xx/core/cmsis/Device/ST/STM32F1xx/Include
        ${CMAKE_SOURCE_DIR}/lib/lwrb/include
)
