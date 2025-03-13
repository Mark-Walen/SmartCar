add_subdirectory(stm32f1xx)

add_library(bsp STATIC
        # BSP Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gpiod.c
)

target_link_libraries(bsp PRIVATE stm32f1xx)

target_include_directories(bsp PRIVATE
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/common/inc
        ${CMAKE_SOURCE_DIR}/drivers/bsp/inc
)