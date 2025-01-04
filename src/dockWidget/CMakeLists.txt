# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable C++17 (or adjust according to your requirement)
set(CMAKE_CXX_STANDARD 17)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenCV REQUIRED)

# Add the source and header files for the library
set(LIB_SOURCES
    DockWidgetViewer.cpp
    DockWidgetLogger.cpp
    DockWidgetBrowser.cpp
    DockWidgetProperty.cpp
)

set(LIB_HEADERS
    DockWidgetViewer.h
    DockWidgetLogger.h
    DockWidgetBrowser.h
    DockWidgetProperty.h
)

# Create a library (static or shared)
add_library(DockWidgetLibs STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Link Qt5 Widgets and ads to the library
target_link_libraries(DockWidgetLibs PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets QtAdvancedDocking QCustomPlot ${OpenCV_LIBS})

target_include_directories(DockWidgetLibs PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(DockWidgetLibs PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
