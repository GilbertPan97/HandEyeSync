set(TARGET_NAME0 atCamCapt)

# import at camera driver lib
string(REPLACE "\\" "/" CX_SDK_ROOT_ENV $ENV{CX_SDK_ROOT_64})
set(CX_SDK_ROOT ${CX_SDK_ROOT_ENV})
message(STATUS "CX camera sdk root path: ${CX_SDK_ROOT}")

# Test if CMake project is built as stand-alone or if it is included from another top level CMake project.
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	# use, i.e. don't skip the full RPATH for the build tree
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

	# when building, don't use the install RPATH already
	# (but later on when installing)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

	SET(CMAKE_INSTALL_RPATH "/opt/cxSupportPackage/cxSDK/cxBaseLib/lib/:/opt/cxSupportPackage/cxSDK/cxCamLib/lib/:/opt/cxSupportPackage/cxSDK/cx3dLib/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/opencv-3.4.2/${AT_PLATFORM_SUBPATH}_shared_vtk_static/lib/")

	# add the automatically determined parts of the RPATH
	# which point to directories outside the build tree to the install RPATH
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# prerequisite CxBaseLib sub-modules and Thirdparty libs
find_package(CxBaseLib REQUIRED HINTS ${CX_SDK_ROOT}/cxBaseLib/lib/cmake)
find_package(CxCamLib REQUIRED HINTS ${CX_SDK_ROOT}/cxCamLib/lib/cmake)
find_package(Cx3dLib REQUIRED HINTS ${CX_SDK_ROOT}/cx3dLib/lib/cmake)
link_libraries(AT::Cx3dLib AT::CxBaseLib AT::CxCamLib)

find_package(OpenCV REQUIRED HINTS ${THIRD_PARTY})
link_libraries(opencv_core opencv_calib3d opencv_highgui
    	opencv_imgcodecs opencv_imgproc opencv_features2d opencv_viz)

# add source file to target
set(CAM_SRC_FILES
    cx_3d_common.cpp
    cx_3d_common2.cpp
    cx_cam_common.cpp)

set(CAM_HEAD_FILES
    cx_3d_common.h
    cx_cam_common.h)

add_executable(${TARGET_NAME0} ./demo/main.cpp ${CAM_SRC_FILES} ${CAM_HEAD_FILES})

target_include_directories(${TARGET_NAME0} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Specify compiler definitions and flags valid for this target.
target_compile_definitions(${TARGET_NAME0} PRIVATE -D_CONSOLE -DAT_NO_AUTOMATIC_LIBS)
target_compile_options(${TARGET_NAME0} PRIVATE ${AT_ADDITIONAL_COMPILER_FLAGS})

set_target_properties(${TARGET_NAME0} PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${EXPORT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${EXPORT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${EXPORT_DIR}"
)

