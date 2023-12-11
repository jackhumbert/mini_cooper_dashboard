cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
# set(CMAKE_HOST_SYSTEM_NAME Mac)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)

unset(_CMAKE_APPLE_ARCHS_DEFAULT)

set(CMAKE_C_COMPILER "/opt/homebrew/bin/aarch64-none-elf-gcc")
set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/aarch64-none-elf-g++")
set(CMAKE_AR "/opt/homebrew/bin/aarch64-none-elf-ar")
set(CMAKE_LINKER "/opt/homebrew/bin/aarch64-none-elf-ld")

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")
# set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

# project(raspi C CXX)

file(GLOB RASPI_SOURCES "*.cpp")

add_executable(raspi ${RASPI_SOURCES} ${CPP_SOURCES} ${SOURCES} ${TINY_TTF_SOURCES})

set_target_properties(raspi PROPERTIES OUTPUT_NAME "raspi/${TARGET}.elf")

add_custom_command(TARGET raspi
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS $<TARGET_FILE:raspi> -O binary raspi/${TARGET}.img
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

target_compile_options(raspi PUBLIC
  -Wno-unused-variable
  -Wno-unused-function
  $<$<COMPILE_LANGUAGE:C>:-Wno-pointer-sign>
  $<$<COMPILE_LANGUAGE:C>:-Wno-incompatible-pointer-types>
  -Wno-narrowing
)

target_compile_definitions(raspi PUBLIC 
  ARM_ALLOW_MULTI_CORE
  # LEAVE_QEMU_ON_HALT
)

# target_include_directories(raspi PUBLIC
#     /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/include/c++/13.2.1/
# )

target_link_libraries(raspi PUBLIC 
  circle 
  lvgl     
  lvgl_demos
  m
  # c
  # g
  # gcc
  # nosys
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/bin/../lib/gcc/aarch64-none-elf/13.2.1/../../../../aarch64-none-elf/lib/libc.a
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libc.a
  # gcc
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libg.a
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libgcc.a
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libstdc++.a
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libnosys.a
  # /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/aarch64-none-elf/lib/libm.a
  # c
  # gcc
)

# target_link_options(raspi PUBLIC
#   # -lm
#   -Wl,/Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/bin/../lib/gcc/aarch64-none-elf/13.2.1/../../../../aarch64-none-elf/lib/libc.a
# )