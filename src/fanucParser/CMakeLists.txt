# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable C++17 (or adjust according to your requirement)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

# Add the source and header files for the library
set(LIB_SOURCES
    FanucParser.cpp
    ProfileParser.cpp
)

set(LIB_HEADERS
    FanucParser.h
    ProfileParser.h
)

# Create a library (static or shared)
add_library(FanucParserLibs STATIC ${LIB_SOURCES} ${LIB_HEADERS})

target_link_libraries(FanucParserLibs PUBLIC ${OpenCV_LIBS})

target_include_directories(FanucParserLibs PUBLIC ${CMAKE_CURRENT_LIST_DIR})