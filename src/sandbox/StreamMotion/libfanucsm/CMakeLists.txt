cmake_minimum_required(VERSION 3.16)
project(libfanucsm)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(spdlog REQUIRED)

include(TestBigEndian)
test_big_endian(BIG_ENDIAN)


include_directories(src)

add_executable(main main.cpp)
target_link_libraries(main pthread)

add_executable(test_thread test_thread.cpp src/raw_controller.cpp src/stream_motion_controller.cpp src/mysocket.cpp src/myendian.cpp src/trajectory.cpp)
target_link_libraries(test_thread  PRIVATE spdlog::spdlog pthread)

add_executable(test_socket test_socket.cpp src/mysocket.cpp)

add_executable(test_raw_controller test_raw_controller.cpp src/raw_controller.cpp src/mysocket.cpp src/myendian.cpp)
target_link_libraries(test_raw_controller PRIVATE spdlog::spdlog)


if(BIG_ENDIAN)
    target_compile_definitions(test_raw_controller PRIVATE HAVE_BIG_ENDIAN)
    target_compile_definitions(test_thread PRIVATE HAVE_BIG_ENDIAN)
else()
    target_compile_definitions(test_raw_controller  PRIVATE HAVE_LITTLE_ENDIAN)
    target_compile_definitions(test_thread  PRIVATE HAVE_LITTLE_ENDIAN)

endif()
