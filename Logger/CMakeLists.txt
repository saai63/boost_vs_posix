cmake_minimum_required(VERSION 3.5)

project(genericLogger)

add_library(genericLogger src/logger.cpp)
target_include_directories(genericLogger PUBLIC ./include)

install(TARGETS genericLogger DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
install(DIRECTORY "include" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES_MATCHING PATTERN "*.h")
