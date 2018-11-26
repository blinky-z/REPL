cmake_minimum_required(VERSION 3.12)
project(Sole)

set(CMAKE_CXX_STANDARD 11)

add_executable(Sole
        sole.hpp sole.cxx sole.fwd.hpp sole.cpp
        )