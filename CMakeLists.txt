cmake_minimum_required(VERSION 3.16)

# include($ENV{IDF_PATH}/tools/cmake/toolchain-${IDF_TARGET}.cmake)

# set(EXTRA_COMPONENT_DIRS src)


# set(CMAKE_SYSTEM_NAME Generic)

project(r53_dash C CXX)

# idf_build_process("esp32s3")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_COLOR_DIAGNOSTICS ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/r53)
INCLUDE_DIRECTORIES(/opt/homebrew/include/)
# INCLUDE_DIRECTORIES(/opt/homebrew/opt/freetype/include/freetype2)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/.pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/include)

LINK_DIRECTORIES(/opt/homebrew/lib)
LINK_DIRECTORIES(/opt/homebrew/opt/freetype/lib)

add_compile_definitions(TINY_TTF_FILE_SUPPORT=1)
add_compile_definitions(DASH_SIMULATION=1)

set(LV_CONF_INCLUDE_SIMPLE ON)
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lv_conf.h
    CACHE STRING "" FORCE)

add_subdirectory(.pio/libdeps/esp32-s3-devkitc-1-myboard/lvgl/)
add_subdirectory(lib/lv_drivers/)

file(GLOB_RECURSE SIM_INCLUDES "simulation/*.h" )
file(GLOB_RECURSE SIM_SOURCES "simulation/*.c*" )
file(GLOB_RECURSE INCLUDES "src/*.h" )
file(GLOB_RECURSE SOURCES "src/*.c" )
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp" )
file(GLOB TINY_TTF_INCLUDES ".pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/include/*")
file(GLOB TINY_TTF_SOURCES ".pio/libdeps/esp32-s3-devkitc-1-myboard/tiny_ttf/src/*")

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

add_custom_target(simulation_run COMMAND $<TARGET_FILE:simulation> WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} DEPENDS simulation)

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

add_custom_target(r53 COMMAND ../dbcc/dbcc r53.dbc WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/r53 SOURCES lib/r53/r53.dbc BYPRODUCTS lib/r53/r53.c lib/r53/r53.h)


# include_directories(${SDL2_INCLUDE_DIRS})

# Let the build system know what the project executable is to attach more targets, dependencies, etc.
# idf_build_executable(${CMAKE_PROJECT_NAME}.elf
#     BUILD_DIR ${CMAKE_BINARY_DIR})

# add_subdirectory(simulation)
# add_subdirectory(device)
add_subdirectory(serial_sender)