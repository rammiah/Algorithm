cmake_minimum_required(VERSION 3.10)
project(filter)

set(CMAKE_CXX_STANDARD 14)

add_executable(filter main.cpp BloomFilter.cpp BloomFilter.h SimpleHash.cpp SimpleHash.h Hash.h)