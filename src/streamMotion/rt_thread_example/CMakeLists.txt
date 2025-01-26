cmake_minimum_required(VERSION 3.10)
project(rt_thread_example CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(rt_thread_example src/rt_thread_example.cpp src/stream_motion/stream_motion.cpp)

target_include_directories(rt_thread_example PRIVATE src/stream_motion)

target_link_libraries(rt_thread_example pthread rt)

add_subdirectory(src)

# gcc -o rt_thread_example src/rt_thread_example.c -pthread -lrt
# sudo ./build/rt_thread_example
