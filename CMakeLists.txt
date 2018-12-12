cmake_minimum_required(VERSION 3.5)
project(nyx)

set(CMAKE_CXX_STANDARD 17)
set(NYX_SRC nyx/Builtin.cpp nyx/Interpreter.cpp nyx/Main.cpp nyx/Parser.cpp nyx/Ast.cpp)

add_executable(nyx ${NYX_SRC})

enable_testing()
file(GLOB test_file_name ${PROJECT_SOURCE_DIR}/nyx_demo/*.nyx)

foreach(each_file ${test_file_name})
 string(REGEX REPLACE ".*/(.*)\.nyx" "\\1" curated_name ${each_file})
    add_test(NAME test_${curated_name} COMMAND nyx ${each_file})
endforeach(each_file ${test_file_name})
