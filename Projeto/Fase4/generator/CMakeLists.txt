cmake_minimum_required(VERSION 3.13.4)

# Project Name
PROJECT(gen)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} main.cpp generator.cpp)

