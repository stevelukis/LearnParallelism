cmake_minimum_required(VERSION 3.13)
project(LearnParallelism C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(ReloadedC main.c main.c)