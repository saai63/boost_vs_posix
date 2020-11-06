cmake_minimum_required(VERSION 3.5)

project(MessageQueue)
set(CMAKE_CXX_STANDARD 11)

# Posix Queues
add_library(MessageQueue_posix src/posix_queues.cpp)
target_include_directories(MessageQueue_posix PUBLIC ./include)
target_include_directories(MessageQueue_posix PUBLIC ../include)

# Boost Queues
find_package(Boost REQUIRED)
add_library(MessageQueue_boost src/boost_queues.cpp)
target_include_directories(MessageQueue_boost PUBLIC ./include)
target_include_directories(MessageQueue_boost PUBLIC ../include)
include_directories($(BOOST_INCLUDE_DIR))

# Install
install(TARGETS MessageQueue_posix DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
install(TARGETS MessageQueue_boost DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
install(DIRECTORY "include" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES_MATCHING PATTERN "*.h")

#add_executable(genericLoggerTest test/test_logger.cpp)
#target_include_directories(genericLoggerTest PUBLIC ./include)
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})
#target_link_libraries(genericLoggerTest PUBLIC genericLogger ${GTEST_LIBRARY} pthread)
#install(TARGETS genericLoggerTest DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
