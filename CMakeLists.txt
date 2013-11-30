project(kontr-tests)
cmake_minimum_required(VERSION 2.8)
ADD_DEFINITIONS(-std=c++11 -g -Wall -Wextra)

aux_source_directory(. SRC_LIST)
aux_source_directory(Generator/ GENERATOR_LIST)
SET(LIBRARIES "")
add_executable(${PROJECT_NAME} ${SRC_LIST} ${GENERATOR_LIST})

# Backtracexx library
find_program(ADDR2LINE addr2line)
IF(ADDR2LINE)
        add_definitions(-DADDR2LINE ${ADDR2LINE})
ENDIF(ADDR2LINE)

add_library(backtracexx STATIC backtracexx/backtracexx.cpp)

SET(LIBRARIES ${LIBRARIES} backtracexx)
IF(UNIX)
        SET(LIBRARIES ${LIBRARIES} dl)
ENDIF(UNIX)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Tests
ENABLE_TESTING()
add_subdirectory("tests/")
