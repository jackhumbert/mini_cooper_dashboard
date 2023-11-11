cmake_minimum_required(VERSION 3.16)

project(serial_sender)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(serial_sender main.c)