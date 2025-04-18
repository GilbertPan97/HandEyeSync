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
    src/RobotComm_Version.cpp
    src/RobotComm_Core.cpp
    src/RobotComm_DataTable.cpp
    src/RobotComm_DataNumReg.cpp
    src/RobotComm_DataPosReg.cpp
    src/RobotComm_DataPosRegXyzwpr.cpp
    src/RobotComm_DataPosRegMG.cpp
    src/RobotComm_DataCurPos.cpp
    src/RobotComm_DataTask.cpp
    src/RobotComm_DataSysVar.cpp
    src/RobotComm_DataSysVarPos.cpp
    src/RobotComm_DataAlarm.cpp
    src/RobotComm_DataString.cpp
    src/libCore/RobotComm_LibCore.cpp
)

# Header files
set(ROBOTCOMM_HEADERS
    src/RobotComm.h
    src/RobotComm_Constants.h
    src/RobotComm_Core.h
    src/RobotComm_DataTable.h
    src/RobotComm_DataNumReg.h
    src/RobotComm_DataPosReg.h
    src/RobotComm_DataPosRegXyzwpr.h
    src/RobotComm_DataPosRegMG.h
    src/RobotComm_DataCurPos.h
    src/RobotComm_DataTask.h
    src/RobotComm_DataSysVar.h
    src/RobotComm_DataSysVarPos.h
    src/RobotComm_DataAlarm.h
    src/RobotComm_DataString.h
    src/libCore/RobotComm_LibCore.h
)

# 设置 C++/CLI 项目的输出
add_library(RobotCommBridge SHARED ${ROBOTCOMM_SOURCES})

# 包含头文件目录
target_include_directories(RobotCommBridge PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(RobotCommBridge PRIVATE /AI"${CMAKE_SOURCE_DIR}/lib")

# 设置目标的 .NET 引用
set_property(TARGET RobotCommBridge PROPERTY VS_DOTNET_REFERENCES
    ${DOTNET_DLLS}
)

# # 如果你希望将 DLL 复制到输出目录（如 bin），可以使用自定义命令：
# add_custom_command(TARGET RobotCommBridge POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${CMAKE_SOURCE_DIR}/lib/bridgeRobotIF.dll
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bridgeRobotIF.dll
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     ${CMAKE_SOURCE_DIR}/lib/RobotInterfaceDotNet.dll
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RobotInterfaceDotNet.dll
# )
