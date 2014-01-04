include_directories(${kontr-tests_SOURCE_DIR})
link_directories(${kontr-tests_SOURCE_DIR})

add_executable(MasterTest MasterTest.cpp)
target_link_libraries(MasterTest ${LIBRARIES})
add_test(MasterTest MasterTest)

add_executable(UnitTest UnitTest.cpp)
target_link_libraries(UnitTest ${LIBRARIES})
add_test(UnitTest UnitTest)

add_executable(Execution Execution.cpp)
target_link_libraries(Execution ${LIBRARIES})
add_test(Execution Execution)

add_executable(Variable Variable.cpp)
target_link_libraries(Variable ${LIBRARIES})
add_test(Variable Variable)

add_executable(Names Names.cpp)
target_link_libraries(Names ${LIBRARIES})
add_test(Names Names)

add_executable(Session Session.cpp)
target_link_libraries(Session ${LIBRARIES})
add_test(Session Session)

add_executable(Language Language.cpp)
target_link_libraries(Language ${LIBRARIES})
add_test(Language Language)
