include_directories(${kontr-tests_SOURCE_DIR})
link_directories(${kontr-tests_SOURCE_DIR})

add_executable(MasterTest MasterTest.cpp)
target_link_libraries(MasterTest ${LIBRARIES})
add_test(MasterTest MasterTest)
