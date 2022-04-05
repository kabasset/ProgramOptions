cmake_minimum_required(VERSION 2.8)
project(ProgramOptions)

find_package(Boost COMPONENTS program_options unit_test_framework REQUIRED)

enable_testing()
add_executable(ProgramOptions_test ProgramOptions_test.cpp)
target_link_libraries(ProgramOptions_test ${Boost_LIBRARIES})

add_test(ProgramOptions_test ProgramOptions_test)

add_executable(ParseProgramOptions ParseProgramOptions.cpp)
target_link_libraries(ParseProgramOptions ${Boost_LIBRARIES})

