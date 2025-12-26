cmake_minimum_required(VERSION 3.10)

project(OrderBookProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(engine main.cpp OrderBook.cpp)

add_executable(benchmark benchmark.cpp OrderBook.cpp)

add_executable(server Server_main.cpp OrderBook.cpp)

find_package(GTest REQUIRED)

add_executable(run_tests tests/OrderBookTest.cpp OrderBook.cpp)

target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

enable_testing()

add_test(NAME OrderBookTests COMMAND run_tests)
