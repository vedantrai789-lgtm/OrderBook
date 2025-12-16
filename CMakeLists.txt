cmake_minimum_required(VERSION 3.10)
project(OrderBookProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(engine main.cpp OrderBook.cpp)

add_executable(benchmark benchmark.cpp OrderBook.cpp)
