# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable C++17 (or adjust according to your requirement)
set(CMAKE_CXX_STANDARD 17)

# Add the source and header files for the library
set(LIB_SOURCES
    lsParser.cpp
)

set(LIB_HEADERS
    lsParser.h
)

# Create a library (static or shared)
add_library(FanucParserLibs STATIC ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(FanucParserLibs PUBLIC ${CMAKE_CURRENT_LIST_DIR})