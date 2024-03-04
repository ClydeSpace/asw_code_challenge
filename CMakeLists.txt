cmake_minimum_required(VERSION 3.10)
project(asw_code_challenge)

# Change the name of the executable target
set(EXECUTABLE_NAME asw_code_challenge) 

# Add executable target with the custom name
add_executable(${EXECUTABLE_NAME} main.c lis3mdl.c i2c.c)

# Include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries if needed
# target_link_libraries(${EXECUTABLE_NAME} <library_name>)

# Set C standard to C99
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY C_STANDARD 99)
