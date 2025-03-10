add_library(lwrb STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lwrb.c
)

target_include_directories(lwrb PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(lwrb PRIVATE LWRB_DISABLE_ATOMIC)
