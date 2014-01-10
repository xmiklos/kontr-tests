include_directories("../")

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/bin)
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
endif()

add_executable(bin/MasterTest MasterTest.cpp)
target_link_libraries(bin/MasterTest ${LIBRARIES})
add_test(MasterTest bin/MasterTest)

add_executable(bin/UnitTest UnitTest.cpp)
target_link_libraries(bin/UnitTest ${LIBRARIES})
add_test(UnitTest bin/UnitTest)

add_executable(bin/Execution Execution.cpp)
target_link_libraries(bin/Execution ${LIBRARIES})
add_test(Execution bin/Execution)

add_executable(bin/Variable Variable.cpp)
target_link_libraries(bin/Variable ${LIBRARIES})
add_test(Variable bin/Variable)

add_executable(bin/Names Names.cpp)
target_link_libraries(bin/Names ${LIBRARIES})
add_test(Names bin/Names)

add_executable(bin/Session Session.cpp)
target_link_libraries(bin/Session ${LIBRARIES})
add_test(Session bin/Session)

add_executable(bin/Language Language.cpp)
target_link_libraries(bin/Language ${LIBRARIES})
add_test(Language bin/Language)
