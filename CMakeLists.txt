cmake_minimum_required(VERSION 3.10)
project(r53_dash LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_COLOR_DIAGNOSTICS ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(/opt/homebrew/include/)
INCLUDE_DIRECTORIES(/opt/homebrew/opt/freetype/include/freetype2)

LINK_DIRECTORIES(/opt/homebrew/lib)
LINK_DIRECTORIES(/opt/homebrew/opt/freetype/lib)

set(LV_CONF_INCLUDE_SIMPLE ON)
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
    CACHE STRING "" FORCE)

add_subdirectory(lib/lvgl/)
add_subdirectory(lib/lv_drivers/)

file(GLOB_RECURSE INCLUDES "src/*.h" )
file(GLOB_RECURSE SOURCES "src/*.c" )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")

find_package(SDL2 REQUIRED SDL2)
find_package(Freetype REQUIRED Freetype)
# include_directories(${SDL2_INCLUDE_DIRS})
add_executable(main main.c ${SOURCES} ${INCLUDES})
target_link_libraries(main PRIVATE SDL2 Freetype m pthread lvgl::lvgl lvgl::drivers)

add_executable(simulator sim.c ${SOURCES} ${INCLUDES})
target_link_libraries(simulator PRIVATE SDL2 Freetype m pthread lvgl::lvgl lvgl::drivers)


add_custom_target (simulator_run COMMAND $<TARGET_FILE:simulator> WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} DEPENDS simulator)
