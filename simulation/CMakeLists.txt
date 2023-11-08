# Save current native flags
# set(NATIVE_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "GCC flags for the native compiler." FORCE)

# Change compiler
set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
set(CMAKE_C_COMPILER ${HOST_C_COMPILER})
set(CMAKE_C_FLAGS ${HOST_C_FLAGS})
set(CURRENT_COMPILER "HOST" CACHE STRING "Which compiler we are using." FORCE)
  
add_executable(simulation ${SOURCES} ${INCLUDES})
target_compile_definitions(simulation PUBLIC DASH_SIMULATION)
target_link_libraries(simulation PRIVATE SDL2 Freetype m pthread lvgl::lvgl lvgl::drivers)

add_custom_target(simulation_run COMMAND $<TARGET_FILE:simulation> WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} DEPENDS simulation)