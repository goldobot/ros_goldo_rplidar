cmake_minimum_required(VERSION 3.1...3.15)

project(rplidar_sdk VERSION 1.0 LANGUAGES C CXX)

add_library(rplidar_sdk STATIC 
  src/rplidar_driver.cpp
  src/hal/thread.cpp
  src/arch/linux/net_serial.cpp
  src/arch/linux/net_socket.cpp
  src/arch/linux/timer.cpp
  )
  
target_include_directories(rplidar_sdk PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>  
  )