cmake_minimum_required(VERSION 3.10)

# Set project name
project(HandEyeSync)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find Qt5 libraries (automatically selects the correct version)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Build and 3rd-party and examples
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/examples)

# Build source code
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/src/dockWidget)

# Add source files
set(SRC_FILES
    src/main.cpp
    src/MainWindow.cpp
)

set(QRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc")

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${QRC_FILE})

# Link the required Qt5 libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE DockWidgetLibs)

# Enable automatic MOC, UIC, and RCC processing for Qt5
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON          # Enable automatic MOC (Meta-Object Compiler)
    AUTORCC ON          # Enable automatic RCC (Resource Compiler)
    AUTOUIC ON
)

if(WIN32)
    # add window platform resource config macro definition
    include(cmake/WinResource.cmake)
    # Set the path to the ICO file for the application icon
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons/app-logo.ico)  

    # Create Windows version resource
    create_win32_resource_version(
        TARGET ${PROJECT_NAME}
        FILENAME ${PROJECT_NAME}
        ICON ${ICON_FILE}
        VERSION "1.0.0"           # Application version
        DESCRIPTION "HandEyeSync is a hand-eye calibration software."
        COMPANY_NAME "Shanghai Fanuc Robotics Co., Ltd." # Company name
        COPYRIGHT "Copyright (c) 2024, Shanghai Fanuc Robotics Co. All rights reserved."
    )
endif()
