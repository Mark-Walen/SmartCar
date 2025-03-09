message(STATUS "STARTUP_FILE ${STARTUP_FILE}")
add_executable(smart_car_main
        app_main.c
        main.c
        ${STARTUP_FILE}
)

target_include_directories(smart_car_main PRIVATE
        ${CMAKE_SOURCE_DIR}/common/inc
)

target_link_libraries(smart_car_main board bsp)
