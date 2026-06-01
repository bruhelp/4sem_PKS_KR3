# ===== Unit-тесты =====
add_executable(ElevatorTests
    main.cpp
    model/CallTests.cpp
    core/ElevatorTests.cpp
    core/ElevatorSystemTests.cpp
    scheduler/FIFOStrategyTests.cpp
    scheduler/NearestCarStrategyTests.cpp
    scheduler/RoundRobinStrategyTests.cpp
    scheduler/SCANStrategyTests.cpp
    services/StatisticsCollectorTests.cpp
    services/ConfigTests.cpp
    services/EventLoggerTests.cpp
)
target_link_libraries(ElevatorTests PRIVATE ElevatorLib gtest gtest_main)
target_include_directories(ElevatorTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# ===== Интеграционные сценарии =====
add_executable(Scenario1 integration/Scenario1_SingleCall.cpp)
target_link_libraries(Scenario1 PRIVATE ElevatorLib)
target_include_directories(Scenario1 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(Scenario2 integration/Scenario2_Overflow.cpp)
target_link_libraries(Scenario2 PRIVATE ElevatorLib)
target_include_directories(Scenario2 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(Scenario3 integration/Scenario3_NearestCar.cpp)
target_link_libraries(Scenario3 PRIVATE ElevatorLib)
target_include_directories(Scenario3 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(Scenario4 integration/Scenario4_SCAN_Opposite.cpp)
target_link_libraries(Scenario4 PRIVATE ElevatorLib)
target_include_directories(Scenario4 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(Scenario5 integration/Scenario5_ConfigBoundaries.cpp)
target_link_libraries(Scenario5 PRIVATE ElevatorLib)
target_include_directories(Scenario5 PRIVATE ${CMAKE_SOURCE_DIR}/src)

enable_testing()
add_test(NAME UnitTests COMMAND ElevatorTests)
add_test(NAME Scenario1 COMMAND Scenario1)
add_test(NAME Scenario2 COMMAND Scenario2)
add_test(NAME Scenario3 COMMAND Scenario3)
add_test(NAME Scenario4 COMMAND Scenario4)
add_test(NAME Scenario5 COMMAND Scenario5)
add_test(NAME Scenario6 COMMAND Scenario6)