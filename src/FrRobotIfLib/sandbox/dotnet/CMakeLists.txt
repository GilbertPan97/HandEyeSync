# 设置最低版本要求
cmake_minimum_required(VERSION 3.15)

# 项目信息
project(RobotCommBridge)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定 C++/CLI 需要使用的编译器标志
if(MSVC)
    # 启用 C++/CLI 编译
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /clr")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /AI\"${CMAKE_SOURCE_DIR}/lib\"")

    # 设置 .NET Framework 的路径
    set(DOTNET_FRAMEWORK_PATH "C:/Windows/Microsoft.NET/Framework/v4.8")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_GENERATOR_PLATFORM x64)
        message(STATUS "Target platform is set to x64")
    endif()
    
    # # 设置 C++/CLI 相关引用的程序集
    # # 不再使用 /reference 参数，这将通过托管代码自动处理
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /reference:${DOTNET_FRAMEWORK_PATH}/System.dll")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /reference:${DOTNET_FRAMEWORK_PATH}/System.Core.dll")
endif()

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 指定库文件的路径
set(DOTNET_DLLS
    "${CMAKE_SOURCE_DIR}/lib/bridgeRobotIF.dll"
    "${CMAKE_SOURCE_DIR}/lib/RobotInterfaceDotNet.dll"
)

# Source files
set(ROBOTCOMM_SOURCES
    dotnet/RobotComm_Version.cpp
    dotnet/RobotComm_Core.cpp
    dotnet/RobotComm_DataTable.cpp
    dotnet/RobotComm_DataNumReg.cpp
    dotnet/RobotComm_DataPosReg.cpp
    dotnet/RobotComm_DataPosRegXyzwpr.cpp
    dotnet/RobotComm_DataPosRegMG.cpp
    dotnet/RobotComm_DataCurPos.cpp
    dotnet/RobotComm_DataTask.cpp
    dotnet/RobotComm_DataSysVar.cpp
    dotnet/RobotComm_DataSysVarPos.cpp
    dotnet/RobotComm_DataAlarm.cpp
    dotnet/RobotComm_DataString.cpp
    dotnet/libCore/RobotComm_LibCore.cpp
)

# Header files
set(ROBOTCOMM_HEADERS
    dotnet/RobotComm.h
    dotnet/RobotComm_Constants.h
    dotnet/RobotComm_Core.h
    dotnet/RobotComm_DataTable.h
    dotnet/RobotComm_DataNumReg.h
    dotnet/RobotComm_DataPosReg.h
    dotnet/RobotComm_DataPosRegXyzwpr.h
    dotnet/RobotComm_DataPosRegMG.h
    dotnet/RobotComm_DataCurPos.h
    dotnet/RobotComm_DataTask.h
    dotnet/RobotComm_DataSysVar.h
    dotnet/RobotComm_DataSysVarPos.h
    dotnet/RobotComm_DataAlarm.h
    dotnet/RobotComm_DataString.h
    dotnet/libCore/RobotComm_LibCore.h
)

# 设置 C++/CLI 项目的输出
add_library(RobotCommBridge SHARED ${ROBOTCOMM_SOURCES})

if(MSVC)
    target_compile_options(RobotCommBridge PRIVATE /clr)
    target_compile_options(RobotCommBridge PRIVATE /AI"${CMAKE_SOURCE_DIR}/lib")
endif()

# 包含头文件目录
target_include_directories(RobotCommBridge PRIVATE ${CMAKE_SOURCE_DIR}/dotnet)

# 设置目标的 .NET 引用
set_property(TARGET RobotCommBridge PROPERTY VS_DOTNET_REFERENCES
    ${DOTNET_DLLS}
)

# Build sample program
option(BUILD_SAMPLE "Build sample application" ON)
if(BUILD_SAMPLE)
    add_executable(SampleUsage dotnet/SampleUsage.cpp)
    target_link_libraries(SampleUsage PRIVATE RobotCommBridge)
endif()

# # 如果你希望将 DLL 复制到输出目录（如 bin），可以使用自定义命令：
# add_custom_command(TARGET RobotCommBridge POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${CMAKE_SOURCE_DIR}/lib/bridgeRobotIF.dll
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bridgeRobotIF.dll
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${CMAKE_SOURCE_DIR}/lib/RobotInterfaceDotNet.dll
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RobotInterfaceDotNet.dll
# )
