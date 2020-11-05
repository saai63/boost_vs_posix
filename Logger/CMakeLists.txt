cmake_minimum_required(VERSION 3.5)

project(genericLogger)
set(CMAKE_CXX_STANDARD 11)

add_library(genericLogger src/logger.cpp)
target_include_directories(genericLogger PUBLIC ./include)

install(TARGETS genericLogger DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
install(DIRECTORY "include" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES_MATCHING PATTERN "*.h")

add_executable(genericLoggerTest test/test_logger.cpp)
target_include_directories(genericLoggerTest PUBLIC ./include)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
target_link_libraries(genericLoggerTest PUBLIC genericLogger ${GTEST_LIBRARY} pthread)
install(TARGETS genericLoggerTest DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
