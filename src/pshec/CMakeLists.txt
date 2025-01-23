set(SRC_FILES
    DataProcessor.cpp
    algorithm.cpp
    calibrator.cpp
    utils.cpp
)

set(HEADER_FILES
    DataProcessor.h
    algorithm.h
    calibrator.h
    utils.h
    common.h
)

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_library(ProfScanCalibrator ${HEADER_FILES} ${SRC_FILES})

# find opencv package: used for ProfScanCalibrator
find_package(OpenCV REQUIRED)
target_link_libraries(ProfScanCalibrator PUBLIC ${OpenCV_LIBS})

# find Eigen package: used for ProfScanCalibrator
find_package(Eigen3 REQUIRED)
target_include_directories(ProfScanCalibrator PUBLIC ${EIGEN3_INCLUDE_DIRS})

target_compile_options(ProfScanCalibrator PRIVATE /bigobj)

install(TARGETS ProfScanCalibrator
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${HEADER_FILES}
    DESTINATION include/ProfScanCalibrator)