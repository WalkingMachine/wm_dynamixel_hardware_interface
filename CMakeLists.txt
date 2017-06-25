cmake_minimum_required(VERSION 3.7)
project(wm_dynamixel_hardware_interface)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/dynamixel_sdk/group_bulk_read.cpp
        src/dynamixel_sdk/group_bulk_read.h
        src/dynamixel_sdk/group_bulk_write.cpp
        src/dynamixel_sdk/group_bulk_write.h
        src/dynamixel_sdk/group_sync_read.cpp
        src/dynamixel_sdk/group_sync_read.h
        src/dynamixel_sdk/group_sync_write.cpp
        src/dynamixel_sdk/group_sync_write.h
        src/dynamixel_sdk/packet_handler.cpp
        src/dynamixel_sdk/packet_handler.h
        src/dynamixel_sdk/port_handler.cpp
        src/dynamixel_sdk/port_handler.h
        src/dynamixel_sdk/port_handler_linux.cpp
        src/dynamixel_sdk/port_handler_linux.h
        src/dynamixel_sdk/protocol1_packet_handler.cpp
        src/dynamixel_sdk/protocol1_packet_handler.h
        src/dynamixel_sdk/protocol2_packet_handler.cpp
        src/dynamixel_sdk/protocol2_packet_handler.h
        src/dynamixel_sdk.h
        src/main.cpp)

add_executable(wm_dynamixel_hardware_interface ${SOURCE_FILES})