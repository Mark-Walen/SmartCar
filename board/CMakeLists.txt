add_library(board STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/board.c
)

target_include_directories(board PRIVATE
    ${CMAKE_SOURCE_DIR}/drivers/bsp/inc
)

target_link_libraries(board PRIVATE bsp)
