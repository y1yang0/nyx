cmake_minimum_required(VERSION 3.5)
project(nyx)

# Enable debugging
# add_definitions(-DNYX_DEBUG=true)

set(CMAKE_CXX_STANDARD 17)
file(GLOB NYX_SRC nyx/**.cpp)

# Compile and link together
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