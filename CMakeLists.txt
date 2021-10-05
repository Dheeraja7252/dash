cmake_minimum_required(VERSION 3.16)
project(C_Shell C)

set(CMAKE_C_STANDARD 99)

add_executable(C_Shell main.c src/cd.c src/cd.h src/charVector.c src/charVector.h src/echo.c src/echo.h src/history.c src/history.h src/jobVector.c src/jobVector.h src/ls.c src/ls.h src/parse.c src/parse.h src/pinfo.c src/pinfo.h src/prompt.c src/prompt.h src/repeat.c src/repeat.h src/systemCommands.c src/systemCommands.h src/utils.c src/utils.h)
