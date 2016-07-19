cmake_minimum_required(VERSION 2.8)

project(final)
include_directories(include)
set(SRC_EXE src/main.c src/arguments.c src/daemonize.c)

add_executable(final ${SRC_EXE})
