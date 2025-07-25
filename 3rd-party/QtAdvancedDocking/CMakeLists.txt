if (${QT_VERSION_MAJOR})
  message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
  find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
else()
  find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
if (UNIX AND NOT APPLE)
    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
endif()

if(NOT DEFINED VERSION_SHORT)
    set(VERSION_SHORT "1.0.0")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(BUILD_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()
set(ads_SRCS
    ads_globals.cpp
    DockAreaTabBar.cpp
    DockAreaTitleBar.cpp
    DockAreaWidget.cpp
    DockContainerWidget.cpp
    DockManager.cpp
    DockOverlay.cpp
    DockSplitter.cpp
    DockWidget.cpp
    DockWidgetTab.cpp
    DockingStateReader.cpp
    DockFocusController.cpp
    ElidingLabel.cpp
    FloatingDockContainer.cpp
    FloatingDragPreview.cpp
    IconProvider.cpp
    DockComponentsFactory.cpp
    AutoHideSideBar.cpp
    AutoHideTab.cpp
    AutoHideDockContainer.cpp
    PushButton.cpp
    ResizeHandle.cpp
    ads.qrc
)
set(ads_HEADERS
    ads_globals.h
    DockAreaTabBar.h
    DockAreaTitleBar.h
    DockAreaTitleBar_p.h
    DockAreaWidget.h
    DockContainerWidget.h
    DockManager.h
    DockOverlay.h
    DockSplitter.h
    DockWidget.h
    DockWidgetTab.h
    DockingStateReader.h
    DockFocusController.h
    ElidingLabel.h
    FloatingDockContainer.h
    FloatingDragPreview.h
    IconProvider.h
    DockComponentsFactory.h
    AutoHideSideBar.h
    AutoHideTab.h
    AutoHideDockContainer.h
    PushButton.h
    ResizeHandle.h
)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
if (UNIX AND NOT APPLE)
    set(ads_SRCS linux/FloatingWidgetTitleBar.cpp ${ads_SRCS})
    set(ads_HEADERS linux/FloatingWidgetTitleBar.h ${ads_HEADERS})
endif()

set(library_name "QtAdvancedDocking")
if(BUILD_STATIC)
    add_library(${library_name} STATIC ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions( ${library_name} PUBLIC ADS_STATIC)
else()
    add_library( ${library_name} SHARED ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions( ${library_name} PRIVATE ADS_SHARED_EXPORT)
endif()

add_library(ads::${library_name} ALIAS ${library_name})

target_link_libraries(${library_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core 
                                               Qt${QT_VERSION_MAJOR}::Gui 
                                               Qt${QT_VERSION_MAJOR}::Widgets)
if (UNIX AND NOT APPLE)
  if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
      find_package(X11 REQUIRED)
      target_link_libraries(${library_name} PUBLIC X11::xcb)
  else()
    target_link_libraries(${library_name} PUBLIC xcb)
  endif()
endif()
set_target_properties(${library_name} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    VERSION ${VERSION_SHORT}
    EXPORT_NAME ${library_name}
)

if(QT_VERSION_MAJOR STREQUAL "5")
    set_target_properties(${library_name} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)
elseif(QT_VERSION_MAJOR STREQUAL "6")
    set_target_properties(${library_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${library_name}ConfigVersion.cmake"
    VERSION ${VERSION_SHORT}
    COMPATIBILITY SameMajorVersion
)

install(FILES ${ads_HEADERS}
    DESTINATION include/${library_name}
    COMPONENT headers
)

install(TARGETS ${library_name}
    EXPORT adsTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/${library_name}
)

install(EXPORT adsTargets
    FILE adsTargets.cmake
    NAMESPACE ads::
    DESTINATION lib/cmake/${library_name}
)
install(FILES qtadvanceddockingConfig.cmake RENAME ${library_name}Config.cmake
    DESTINATION lib/cmake/${library_name}
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${library_name}ConfigVersion.cmake"
    DESTINATION lib/cmake/${library_name}
)

target_include_directories(${library_name} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)