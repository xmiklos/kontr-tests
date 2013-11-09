project(kontr-tests)
cmake_minimum_required(VERSION 2.8)
ADD_DEFINITIONS(-std=c++11)
aux_source_directory(. SRC_LIST)
aux_source_directory(Generator/ GENERATOR_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${GENERATOR_LIST})

