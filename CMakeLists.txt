cmake_minimum_required(VERSION 4.3)
project(http C)

set(CMAKE_C_STANDARD 11)

add_executable(http src/main.c
        include/phttp.h
        src/phttp.c
        src/proto.c
        include/proto.h
)

link_directories(http PUBLIC include/)

# Trying to find Python3 headers
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Linking python libs
target_include_directories(http PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(http PRIVATE Python3::Python)