cmake_minimum_required(VERSION 3.15)
project(sdi-bench)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(sdi-bench
        main.cpp
        sdi-block.h
        sdi-db.cpp
        sdi-db.h
        sdi-entry.cpp
        sdi-entry.h
        sdi-index.cpp
        sdi-index.h
        sdi-types.h
        sdi.cpp
        sdi.h
        sort.h
        timer.cpp
        timer.h)
