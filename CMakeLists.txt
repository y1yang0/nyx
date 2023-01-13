cmake_minimum_required(VERSION 3.5)
project(nyx)

set(CMAKE_CXX_STANDARD 17)
set(NYX_SRC nyx/Builtin.cpp nyx/Interpreter.cpp nyx/Main.cpp nyx/Parser.cpp nyx/Utils.cpp nyx/Runtime.cpp)


# Nyx compiler
add_executable(nyx ${NYX_SRC})

enable_testing()
file(GLOB test_file_namea ${PROJECT_SOURCE_DIR}/nyx_test/example/*.nyx)

# Create unit tests
foreach(each_file ${test_file_namea})
 string(REGEX REPLACE ".*/(.*)\.nyx" "\\1" curated_name ${each_file})
    add_test(NAME example_${curated_name} COMMAND nyx ${each_file})
endforeach(each_file ${test_file_namea})

file(GLOB test_file_nameb ${PROJECT_SOURCE_DIR}/nyx_test/tiresome/*.nyx)
foreach(each_file ${test_file_nameb})
 string(REGEX REPLACE ".*/(.*)\.nyx" "\\1" curated_name ${each_file})
    add_test(NAME tiresome_${curated_name} COMMAND nyx ${each_file})
endforeach(each_file ${test_file_nameb})