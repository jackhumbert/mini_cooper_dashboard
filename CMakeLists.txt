cmake_minimum_required(VERSION 3.16)

# include($ENV{IDF_PATH}/tools/cmake/toolchain-${IDF_TARGET}.cmake)

# set(EXTRA_COMPONENT_DIRS src)


# set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_OSX_SYSROOT "")

set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)

unset(_CMAKE_APPLE_ARCHS_DEFAULT)

set(CMAKE_C_COMPILER_FORCED   1)
set(CMAKE_CXX_COMPILER_FORCED 1)

set(CMAKE_C_COMPILER "/opt/homebrew/bin/aarch64-none-elf-gcc")
set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/aarch64-none-elf-g++")
set(CMAKE_AR "/opt/homebrew/bin/aarch64-none-elf-ar")
set(CMAKE_LINKER "/opt/homebrew/bin/aarch64-none-elf-ld")
set(CMAKE_OBJCOPY "/opt/homebrew/bin/aarch64-none-elf-objcopy")

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")

project(r53_dash C CXX)

set(RASPPI 4)

# idf_build_process("esp32s3")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_COLOR_DIAGNOSTICS ON)

# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/r53)
# INCLUDE_DIRECTORIES(/opt/homebrew/include/)
# INCLUDE_DIRECTORIES(/opt/homebrew/opt/freetype/include/freetype2)
# include_directories(/Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/include/c++/13.2.1/)
include_directories(/Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/include/c++/13.2.1/aarch64-none-elf/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/.pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/include)

# LINK_DIRECTORIES(/opt/homebrew/lib)
# LINK_DIRECTORIES(/opt/homebrew/opt/freetype/lib)

set(LV_CONF_INCLUDE_SIMPLE ON)
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/raspi/lv_conf.h
    CACHE STRING "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/circle/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(.pio/libdeps/esp32-s3-devkitc-1-myboard/lvgl/)
add_subdirectory(lib/lv_drivers/)

file(GLOB_RECURSE SIM_INCLUDES "simulation/*.h" )
file(GLOB_RECURSE SIM_SOURCES "simulation/*.c*" )
file(GLOB_RECURSE INCLUDES "src/*.h" )
file(GLOB_RECURSE SOURCES "src/*.c" )
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp" )
file(GLOB TINY_TTF_INCLUDES ".pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/include/*")
file(GLOB TINY_TTF_SOURCES ".pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/src/*")

list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lib/r53/r53.c)

list(REMOVE_ITEM INCLUDES 
    ${PROJECT_SOURCE_DIR}/src/serial_can.h
    ${PROJECT_SOURCE_DIR}/src/sd_card.h
)
list(REMOVE_ITEM CPP_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/serial_can.cpp
    ${PROJECT_SOURCE_DIR}/src/sd_card.cpp
    ${PROJECT_SOURCE_DIR}/src/dev.cpp
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")


find_package(SDL2)
# find_package(Freetype)

add_executable(simulation ${SIM_SOURCES} ${SOURCES} ${CPP_SOURCES} ${INCLUDES} ${SIM_INCLUDES} ${TINY_TTF_SOURCES} ${TINY_TTF_INCLUDES} ${PROJECT_SOURCE_DIR}/lib/r53/r53.c)
target_compile_definitions(simulation PUBLIC DASH_SIMULATION)
target_link_libraries(simulation PRIVATE SDL2 Freetype m pthread lvgl::lvgl lvgl::drivers)

target_compile_definitions(simulation PUBLIC
    TINY_TTF_FILE_SUPPORT=1
    DASH_SIMULATION=1
    AARCH=64
)

add_custom_target(simulation_run 
    COMMAND $<TARGET_FILE:simulation> 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
    DEPENDS simulation
    VERBATIM)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/src/assets/fonts.c") 
    include(bin2h.cmake)

    file(WRITE src/assets/fonts.c "#include <stddef.h>\n")
    file(GLOB_RECURSE FONTS "fonts/*.ttf")
    foreach(file ${FONTS})
        get_filename_component(variableName ${file} NAME)
        bin2h(SOURCE_FILE ${file} HEADER_FILE src/assets/fonts.c VARIABLE_NAME ${variableName} APPEND NULL_TERMINATE)
        file(APPEND src/assets/fonts.c "\n")
    endforeach()
endif()

add_custom_target(r53
    COMMAND ../dbcc/dbcc r53.dbc 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/r53 
    SOURCES lib/r53/r53.dbc 
    BYPRODUCTS lib/r53/r53.c lib/r53/r53.h
)


# include_directories(${SDL2_INCLUDE_DIRS})

# Let the build system know what the project executable is to attach more targets, dependencies, etc.
# idf_build_executable(${CMAKE_PROJECT_NAME}.elf
#     BUILD_DIR ${CMAKE_BINARY_DIR})

# add_subdirectory(simulation)
# add_subdirectory(device)
add_subdirectory(serial_sender)

add_subdirectory(lib/circle)
add_subdirectory(lib/circle/addon/lvgl)
add_subdirectory(lib/circle/tools/touchscreen-calibrator)
add_subdirectory(raspi)