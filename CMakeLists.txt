cmake_minimum_required(VERSION 3.13)

# Set PICO_SDK_PATH in the environment, or copy pico_sdk_import.cmake from the SDK (external/).
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(yd_rp2040_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(yd_rp2040_test src/main.c)
pico_generate_pio_header(yd_rp2040_test ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio)

target_link_libraries(yd_rp2040_test
    pico_stdlib
    pico_rand
    hardware_pio
)

pico_enable_stdio_usb(yd_rp2040_test 1)
pico_enable_stdio_uart(yd_rp2040_test 0)
pico_add_extra_outputs(yd_rp2040_test)
