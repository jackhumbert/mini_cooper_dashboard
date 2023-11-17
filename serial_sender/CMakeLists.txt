cmake_minimum_required(VERSION 3.16)

project(serial_sender)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(serial_sender main.c ini.c)

add_custom_target(get_logs 
    COMMAND $<TARGET_FILE:serial_sender> get_logs 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
    DEPENDS serial_sender)

add_custom_target(get_latest_logs 
    COMMAND $<TARGET_FILE:serial_sender> get_latest_logs 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
    DEPENDS serial_sender)

add_custom_target(set_time 
    COMMAND $<TARGET_FILE:serial_sender> set_time 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
    DEPENDS serial_sender)