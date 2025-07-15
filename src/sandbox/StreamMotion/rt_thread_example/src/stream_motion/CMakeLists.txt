# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
find_package(spdlog REQUIRED)

# Create stream_motion library
add_library(stream_motion STATIC stream_motion.cpp)

target_link_libraries(stream_motion PRIVATE spdlog::spdlog)

# Specify include directories for the library
target_include_directories(stream_motion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add required libraries for Windows
if(WIN32)
    target_link_libraries(stream_motion PRIVATE ws2_32)
endif()

if(TRUE)
    # Add executable
    add_executable(test_stream_motion test.cpp)

    # Link stream_motion library and spdlog library
    target_link_libraries(test_stream_motion PRIVATE stream_motion spdlog::spdlog)
endif()
