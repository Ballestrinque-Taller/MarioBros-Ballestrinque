project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run TestBasico.cpp)

# linking Google_Tests_run with lib to be tested
#target_link_libraries(Google_Tests_run DateConverter_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)