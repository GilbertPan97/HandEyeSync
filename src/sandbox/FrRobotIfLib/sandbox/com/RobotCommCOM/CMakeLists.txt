cmake_minimum_required(VERSION 3.14)

project(RobotCommCOM LANGUAGES CXX)

# Force 32-bit build
if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

set(CMAKE_CXX_STANDARD 17)

# Compile IDL
set(IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/RobotComm.idl)
set(TLB_FILE ${CMAKE_CURRENT_BINARY_DIR}/RobotComm.tlb)

add_custom_command(
    OUTPUT ${TLB_FILE}
    COMMAND midl ${IDL_FILE}
    DEPENDS ${IDL_FILE}
    COMMENT "Compiling IDL..."
)

add_library(RobotCommCOM SHARED
    RobotCommCOM.cpp
    RobotCommCOM.def
    RobotCommCOM.rc
    ${TLB_FILE}
)

target_include_directories(RobotCommCOM PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(RobotCommCOM PRIVATE ole32 oleaut32 uuid)

set_target_properties(RobotCommCOM PROPERTIES
    OUTPUT_NAME "RobotCommCOM"
    PREFIX ""
)
