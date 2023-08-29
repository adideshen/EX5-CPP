cmake_minimum_required(VERSION 3.16)
project(ex5_adideshen)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_adideshen
        Movie.cpp
        Movie.h
        main.cpp
#        print_main.cpp
#        test.cpp
        RecommenderSystem.h
        RecommenderSystem.cpp
        RecommenderSystemLoader.h
        RecommenderSystemLoader.cpp
        RSUser.cpp
        RSUser.h
        RSUser.cpp
        RSUsersLoader.h
        RSUsersLoader.cpp)
