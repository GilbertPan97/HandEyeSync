# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if using MSVC compiler and ensure C++17 is enabled
if(MSVC)
    # Enable C++17 for MSVC
    add_compile_options(/std:c++17)

    # Enable MFC support (Use MFC in a Shared DLL)
    add_definitions(-D_AFXDLL)
    
    # Ensure /MD is used for runtime library linking
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
endif()

# Add the source and header files for the library
set(LIB_SOURCES
    frrjif.cpp
    frrobotiflib.cpp
    StdAfx.cpp
)

set(LIB_HEADERS
    frrjif.h
    frrobotiflib.h
    StdAfx.h
)

# Create a library (static or shared)
add_library(FrRobotIfLib STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Include the current directory for headers
target_include_directories(FrRobotIfLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Link against MFC libraries (only for MSVC)
if(MSVC)
    target_link_libraries(FrRobotIfLib PUBLIC mfc140u)  # 对应 VS 2015 及以上
endif()
