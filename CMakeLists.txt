cmake_minimum_required(VERSION 3.22)

# Cross compiled?
# set(TOOLCHAIN_PREFIX "arm-linux-gnueabihf-")
# Compiler
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")


project(i2c_driver VERSION 0.1)

add_executable(${PROJECT_NAME} i2c.c)

