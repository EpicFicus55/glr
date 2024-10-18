cmake_minimum_required(VERSION 3.23.0)

project(glr)

add_executable(glr
    src/main.c
    )

find_package(OpenGL REQUIRED)