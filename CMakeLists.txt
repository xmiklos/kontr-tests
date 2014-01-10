project(kontr-tests)
cmake_minimum_required(VERSION 2.8)
ADD_DEFINITIONS(-std=c++11 -g -Wall -Wextra)

if(EXISTS backtracexx/)
    add_subdirectory("backtracexx/")
    SET(LIBRARIES "backtracexx")
    add_definitions(-DBACKTRACEXX)
endif(EXISTS backtracexx/)

aux_source_directory(. SRC_LIST)
foreach (src ${SRC_LIST})
        string(REGEX REPLACE "^[.\\/]*([^.]+).*$" "\\1" executable ${src})
        add_executable(bin/${executable} ${src})
        target_link_libraries(bin/${executable} ${LIBRARIES})
endforeach(src)

if(NOT EXISTS bin/)
	file(MAKE_DIRECTORY bin/)
endif()

# Tests
ENABLE_TESTING()
add_subdirectory("tests/")
