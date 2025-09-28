set(SRC_FILES
    DataProcessor.cpp
    PsAlgorithm.cpp
    PsCalibrator.cpp
    utils.cpp
)

set(HEADER_FILES
    DataProcessor.h
    PsAlgorithm.h
    PsCalibrator.h
    utils.h
    common.h
)

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})

add_library(ProfScanCalibrator STATIC ${HEADER_FILES} ${SRC_FILES})

# find opencv package: used for ProfScanCalibrator
find_package(OpenCV REQUIRED)
target_link_libraries(ProfScanCalibrator PUBLIC ${OpenCV_LIBS})

# find Eigen package: used for ProfScanCalibrator
find_package(Eigen3 REQUIRED)
target_include_directories(ProfScanCalibrator PUBLIC ${EIGEN3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(ProfScanCalibrator PRIVATE /bigobj)

if(ON)
    # Try to find Boost first
    find_package(Boost 1.78 QUIET)
    find_package(VTK QUIET)
    find_package(PCL 1.8 QUIET)

    # Only build demos if Boost, VTK, and PCL are found
    if(Boost_FOUND AND VTK_FOUND AND PCL_FOUND)
        message(STATUS "Boost, VTK, and PCL found, building demos")

        # ProfScanCalibratorTest
        add_executable(ProfScanCalibratorTest demo/demo_ball_scan.cpp)
        add_dependencies(ProfScanCalibratorTest FanucParserLibs)
        target_link_libraries(ProfScanCalibratorTest PRIVATE 
            ProfScanCalibrator 
            FanucParserLibs
            VTK::CommonCore 
            VTK::CommonDataModel 
            VTK::IOImage 
            VTK::RenderingCore
            VTK::FiltersCore
            VTK::RenderingOpenGL2
            VTK::InteractionStyle
            VTK::RenderingAnnotation
            ${PCL_LIBRARIES}
            ${Boost_LIBRARIES}
        )
        target_include_directories(ProfScanCalibratorTest PRIVATE 
            ${PCL_INCLUDE_DIRS} 
            ${Boost_INCLUDE_DIRS}
        )

        # fit_sphere
        add_executable(fit_sphere demo/sphere_center.cpp)
        target_link_libraries(fit_sphere PRIVATE ${PCL_LIBRARIES} ${Boost_LIBRARIES})
        target_include_directories(fit_sphere PRIVATE ${PCL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
        add_definitions(${PCL_DEFINITIONS})

    else()
        message(WARNING "Boost, VTK, or PCL not found, skipping demo compilation")
    endif()
endif()

install(TARGETS ProfScanCalibrator
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${HEADER_FILES}
    DESTINATION include/ProfScanCalibrator)