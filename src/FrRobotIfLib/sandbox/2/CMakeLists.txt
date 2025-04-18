cmake_minimum_required(VERSION 3.14)

# Project information
project(RobotComm
    VERSION 2.0.0
    DESCRIPTION "Robot communication library based on direct dispid calls"
    LANGUAGES CXX)

# Use a more reliable method to set 32-bit architecture
if(MSVC)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compilation options
add_definitions(-DUNICODE -D_UNICODE)

# Source files
set(ROBOTCOMM_SOURCES
    RobotComm/RobotComm_Version.cpp
    RobotComm/RobotComm_Core.cpp
    RobotComm/RobotComm_DataTable.cpp
    RobotComm/RobotComm_DataNumReg.cpp
    RobotComm/RobotComm_DataPosReg.cpp
    RobotComm/RobotComm_DataPosRegXyzwpr.cpp
    RobotComm/RobotComm_DataPosRegMG.cpp
    RobotComm/RobotComm_DataCurPos.cpp
    RobotComm/RobotComm_DataTask.cpp
    RobotComm/RobotComm_DataSysVar.cpp
    RobotComm/RobotComm_DataSysVarPos.cpp
    RobotComm/RobotComm_DataAlarm.cpp
    RobotComm/RobotComm_DataString.cpp
    RobotComm/libCore/RobotComm_LibCore.cpp
)

# Header files
set(ROBOTCOMM_HEADERS
    RobotComm/RobotComm.h
    RobotComm/RobotComm_Constants.h
    RobotComm/RobotComm_Core.h
    RobotComm/RobotComm_DataTable.h
    RobotComm/RobotComm_DataNumReg.h
    RobotComm/RobotComm_DataPosReg.h
    RobotComm/RobotComm_DataPosRegXyzwpr.h
    RobotComm/RobotComm_DataPosRegMG.h
    RobotComm/RobotComm_DataCurPos.h
    RobotComm/RobotComm_DataTask.h
    RobotComm/RobotComm_DataSysVar.h
    RobotComm/RobotComm_DataSysVarPos.h
    RobotComm/RobotComm_DataAlarm.h
    RobotComm/RobotComm_DataString.h
    RobotComm/libCore/RobotComm_LibCore.h
)

# Create static library
add_library(RobotComm STATIC ${ROBOTCOMM_SOURCES} ${ROBOTCOMM_HEADERS})

# Set include directories
target_include_directories(RobotComm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link necessary Windows libraries
target_link_libraries(RobotComm PRIVATE
    ole32
    oleaut32
    uuid
)

add_subdirectory(RobotCommCOM)

# Build sample program
option(BUILD_SAMPLE "Build sample application" ON)
if(BUILD_SAMPLE)
    add_executable(SampleUsage SampleUsage.cpp)
    target_link_libraries(SampleUsage PRIVATE RobotComm)
endif()

# Installation rules
install(TARGETS RobotComm
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(FILES ${ROBOTCOMM_HEADERS} DESTINATION include/RobotComm)

# Output configuration information
message(STATUS "Configuring RobotComm project:")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: 32-bit (Win32)")
message(STATUS "  Sample program: ${BUILD_SAMPLE}") 