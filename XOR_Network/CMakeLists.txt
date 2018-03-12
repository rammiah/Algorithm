cmake_minimum_required(VERSION 3.9)
project(XOR)

set(CMAKE_CXX_STANDARD 11)

add_executable(XOR main.cpp network/BPNet.cpp network/BPNet.h)