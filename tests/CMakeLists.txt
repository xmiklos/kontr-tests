include_directories(${kontr-tests_SOURCE_DIR})
link_directories(${kontr-tests_SOURCE_DIR})

add_executable(MasterTest MasterTest.cpp)
target_link_libraries(MasterTest ${LIBRARIES})
add_test(MasterTest MasterTest)

add_executable(Variable Variable.cpp)
target_link_libraries(Variable ${LIBRARIES})
add_test(Variable Variable)

add_executable(Names Names.cpp)
target_link_libraries(Names ${LIBRARIES})
add_test(Names Names)
