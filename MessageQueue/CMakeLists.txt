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

# Test
## Posix Message queues
add_executable(MessageQueue_posix_Test test/test_posix_queues.cpp)
target_include_directories(MessageQueue_posix_Test PUBLIC ./include)
target_include_directories(MessageQueue_posix_Test PUBLIC ../include)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
find_library(genericLogger_lib NAMES genericLogger HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../lib )
target_link_libraries(MessageQueue_posix_Test PUBLIC MessageQueue_posix ${genericLogger_lib} ${GTEST_LIBRARY} pthread rt)
install(TARGETS MessageQueue_posix_Test DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

## Boost Message queues
add_executable(MessageQueue_boost_Test test/test_boost_queues.cpp)
target_include_directories(MessageQueue_boost_Test PUBLIC ./include)
target_include_directories(MessageQueue_boost_Test PUBLIC ../include)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
find_library(genericLogger_lib NAMES genericLogger HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../lib )
target_link_libraries(MessageQueue_boost_Test PUBLIC MessageQueue_boost ${genericLogger_lib} ${GTEST_LIBRARY} pthread rt)
install(TARGETS MessageQueue_boost_Test DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
