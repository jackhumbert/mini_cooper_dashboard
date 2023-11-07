cmake_minimum_required(VERSION 3.16)

# include($ENV{IDF_PATH}/tools/cmake/toolchain-${IDF_TARGET}.cmake)

# set(EXTRA_COMPONENT_DIRS src)


# set(CMAKE_SYSTEM_NAME Generic)

project(r53_dash C)

# idf_build_process("esp32s3")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_COLOR_DIAGNOSTICS ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(/opt/homebrew/include/)
INCLUDE_DIRECTORIES(/opt/homebrew/opt/freetype/include/freetype2)

LINK_DIRECTORIES(/opt/homebrew/lib)
LINK_DIRECTORIES(/opt/homebrew/opt/freetype/lib)

set(LV_CONF_INCLUDE_SIMPLE ON)
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lv_conf.h
    CACHE STRING "" FORCE)

add_subdirectory(lib/lvgl/)
add_subdirectory(lib/lv_drivers/)

file(GLOB_RECURSE INCLUDES "src/*.h" )
file(GLOB_RECURSE SOURCES "src/*.c" )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")


find_package(SDL2)
find_package(Freetype)
# include_directories(${SDL2_INCLUDE_DIRS})

# Let the build system know what the project executable is to attach more targets, dependencies, etc.
# idf_build_executable(${CMAKE_PROJECT_NAME}.elf
#     BUILD_DIR ${CMAKE_BINARY_DIR})

add_subdirectory(simulation)
# add_subdirectory(device)