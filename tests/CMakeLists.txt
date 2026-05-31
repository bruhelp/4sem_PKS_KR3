file(GLOB_RECURSE TEST_SOURCES
    *.cpp
)

add_executable(
    ElevatorTests
    ${TEST_SOURCES}
)

target_include_directories(
    ElevatorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    ElevatorTests
    PRIVATE
    ElevatorLib
    gtest
    gtest_main
    Threads::Threads
)

include(GoogleTest)

gtest_discover_tests(
    ElevatorTests
)