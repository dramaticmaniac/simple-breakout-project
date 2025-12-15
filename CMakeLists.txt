cmake_minimum_required(VERSION 3.22)
project(breakout)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3  CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(
    breakout
        breakout.cpp
        game.h
        sprite.h
        sprite.cpp
        assets.h
        assets.cpp
        graphics.h
        graphics.cpp
        level.h
        level.cpp
        ball.h
        ball.cpp
        paddle.h
        paddle.cpp
        colors.h
)
target_link_libraries(breakout PRIVATE raylib glfw)
