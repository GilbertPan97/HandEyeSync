cmake_minimum_required(VERSION 3.10)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(Calibration main.cpp HandEyeCalib.cpp)

target_include_directories(Calibration PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EIGEN3_INCLUDE_DIR})

target_link_libraries(Calibration PUBLIC ${OpenCV_LIBS})

set_target_properties(Calibration PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      CXX_EXTENSIONS OFF)

