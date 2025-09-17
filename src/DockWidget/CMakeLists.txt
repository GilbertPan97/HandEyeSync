cmake_minimum_required(VERSION 3.10)
project(DockWidgetLibs)

set(CMAKE_CXX_STANDARD 17)

# --- Find Qt6 first, fallback to Qt5 ---
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
    set(QtCore_LIB Qt6::Core)
    set(QtGui_LIB Qt6::Gui)
    set(QtWidgets_LIB Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    message(STATUS "Using Qt5")
    set(QT_VERSION_MAJOR 5)
    set(QtCore_LIB Qt5::Core)
    set(QtGui_LIB Qt5::Gui)
    set(QtWidgets_LIB Qt5::Widgets)
endif()

# --- Find OpenCV ---
find_package(OpenCV REQUIRED)

# --- Source & headers ---
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

set(QRC_FILE "${CMAKE_SOURCE_DIR}/resources.qrc")

# --- Create static library ---
add_library(DockWidgetLibs STATIC ${LIB_SOURCES} ${LIB_HEADERS} ${QRC_FILE})

# --- Link libraries ---
target_link_libraries(DockWidgetLibs PUBLIC
    ${QtCore_LIB}
    ${QtGui_LIB}
    ${QtWidgets_LIB}
    QtAdvancedDocking
    QCustomPlot
    ${OpenCV_LIBS}
)

target_include_directories(DockWidgetLibs PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# --- Enable Qt auto-tools ---
set_target_properties(DockWidgetLibs PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
