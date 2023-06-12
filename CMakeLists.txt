cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH ~/pico/pico-sdk)
set(RF24_PATH ~/pico/RF24)
set(PROJECT_NAME uMyoMinecraftHID)


include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${RF24_PATH}/CMakeLists.txt)

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
  main.cpp
  radio.cpp
  uMyo.cpp
  quat_math.cpp
  controller.cpp
  led.cpp
  )


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
  RF24
  pico_stdlib
  hardware_spi
)