cmake_minimum_required(VERSION 3.26)
project(MazeGenerator C)

set(CMAKE_C_STANDARD 11)

MATH(EXPR stack_size "16 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

add_executable(MazeGenerator main.c
        maze.c
        maze.h
        rng.c
        rng.h
        maze_API.h
        common.h
)
