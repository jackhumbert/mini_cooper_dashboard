# Save current host flags
# set(HOST_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "GCC flags for the host compiler." FORCE)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION "0.0.0")

# # Change compiler
# set(CMAKE_SYSTEM_NAME ${NATIVE_SYSTEM_NAME})
# set(CMAKE_SYSTEM_PROCESSOR ${NATIVE_SYSTEM_PROCESSOR})
# set(CMAKE_C_COMPILER ${NATIVE_C_COMPILER})
# set(CMAKE_C_FLAGS ${NATIVE_C_FLAGS})
# set(CURRENT_COMPILER "NATIVE" CACHE STRING "Which compiler we are using." FORCE)

set(IDF_TARGET "esp32s3")
set(ENV{IDF_PATH} "/Users/jackhumbert/esp/esp-idf")


set(CMAKE_SYSTEM_PROCESSOR esp32s3)
set(CMAKE_C_COMPILER xtensa-esp32s3-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-esp32s3-elf-g++)
set(CMAKE_ASM_COMPILER xtensa-esp32s3-elf-gcc)
set(_CMAKE_TOOLCHAIN_PREFIX xtensa-esp32s3-elf-)
# unset(CMAKE_C_FLAGS)

# include("$ENV{IDF_PATH}/tools/cmake/toolchain-${IDF_TARGET}.cmake")

set(PYTHON /Users/jackhumbert/.espressif/python_env/idf5.1_py3.9_env/bin/python3)
set(TARGET esp32s3)
# -m venv /Users/jackhumbert/.espressif/python_env/idf5.1_py3.9_env/")
# set(__CHECK_PYTHON FALSE)

# set(CMAKE_C_COMPILER "/Users/jackhumbert/.espressif/tools/xtensa-esp32s3-elf/esp-12.2.0_20230208/xtensa-esp32s3-elf/bin/xtensa-esp32s3-elf-gcc")
# set(CMAKE_CXX_COMPILER "/Users/jackhumbert/.espressif/tools/xtensa-esp32s3-elf/esp-12.2.0_20230208/xtensa-esp32s3-elf/bin/xtensa-esp32s3-elf-g++")

list(APPEND CMAKE_PROGRAM_PATH "/Users/jackhumbert/.espressif/tools/xtensa-esp32s3-elf/esp-12.2.0_20230208/xtensa-esp32s3-elf/bin")

# set(CMAKE_TOOLCHAIN_FILE "$ENV{IDF_PATH}/tools/cmake/toolchain-${IDF_TARGET}.cmake")

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

add_executable(${CMAKE_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/main.c ${SOURCES} ${INCLUDES})
target_link_libraries(${CMAKE_PROJECT_NAME}.elf PRIVATE SDL2 Freetype m pthread lvgl::lvgl lvgl::drivers)