cmake_minimum_required(VERSION 3.5)
project(CodeTP2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Graphe.cpp
    Graphe.h
    Principal.cpp
    ReseauAerien.cpp
    ReseauAerien.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ReseauAerien.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(CodeTP2 ${SOURCE_FILES})