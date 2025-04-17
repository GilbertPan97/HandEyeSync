# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project and its language
project(FrRobotIfLibProject LANGUAGES CXX)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable MFC and set runtime flags for MSVC
if(MSVC)
    # Enable MFC (use shared DLL version)
    add_definitions(-D_AFXDLL)
    set(CMAKE_MFC_FLAG 2)  # 0: off, 1: static, 2: shared DLL

    # Force use of multithreaded DLL runtime library
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
endif()

# Source files for the static library
set(LIB_SOURCES
    frrjif.cpp
    frrobotiflib.cpp
    StdAfx.cpp
    # frrjiftestDlg.cpp
    # frrjiftest.cpp
    # DlgProxy.cpp
    # InputBox.cpp
    FrRobotInterface.cpp
)

# Define the static library target
add_library(FrRobotInterface STATIC ${LIB_SOURCES})
# add_executable(FrRobotIfLib WIN32 ${LIB_SOURCES} ${LIB_HEADERS})

# Include current directory for header search paths
target_include_directories(FrRobotInterface PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Set MSVC-specific properties to support MFC
if(MSVC)
    set_target_properties(FrRobotInterface PROPERTIES
        COMPILE_FLAGS "/Zm500"                  # Increase precompiled header memory
        VS_GLOBAL_KEYWORD "MFCProj"             # Identify as MFC project in MSVC
        VS_GLOBAL_RuntimeLibrary "MultiThreadedDLL" # Use /MD runtime
        MFC_FLAG 2                               # Use MFC shared DLL
    )
endif()

# Optional: build a test executable for the library
option(BUILD_FRRJIF_TEST "Build the frrjifTest executable" ON)
if(BUILD_FRRJIF_TEST)
    add_executable(frrjifTest Test.cpp)
    target_link_libraries(frrjifTest PRIVATE FrRobotInterface)

    if(MSVC)
        # Ensure MFC is enabled for the test application as well
        set_target_properties(frrjifTest PROPERTIES MFC_FLAG 2)
    endif()
endif()
