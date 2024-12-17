cmake_minimum_required(VERSION 3.5)

# Set project name
project(HandEyeSync)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find Qt5 libraries (automatically selects the correct version)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

subdirs(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/examples)

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
target_link_libraries(${PROJECT_NAME} PRIVATE QtAdvancedDocking SARibbonBar)

# Enable automatic MOC, UIC, and RCC processing for Qt5
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON          # Enable automatic MOC (Meta-Object Compiler)
    AUTORCC ON          # Enable automatic RCC (Resource Compiler)
    AUTOUIC ON
)
