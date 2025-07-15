# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if using MSVC compiler and ensure C++17 is enabled (for Visual Studio)
if(MSVC)
    # Enable C++17 for MSVC
    add_compile_options(/std:c++17)
endif()

# Add the source and header files for the library
set(LIB_SOURCES
    tcp_server.cpp
    message_parser.cpp
    message.cpp
)

set(LIB_HEADERS
    tcp_server.h
    message_parser.h
    message.h
)

# Create a library (static or shared)
add_library(ISightServer STATIC ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(ISightServer PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
    target_link_libraries(ISightServer ws2_32)
endif()

add_executable(ISightServerTest main.cpp)
target_link_libraries(ISightServerTest PRIVATE ISightServer)