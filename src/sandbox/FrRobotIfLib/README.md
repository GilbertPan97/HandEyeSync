# RobotComm库 - 机器人通信组件

## 概述

RobotComm是一个通过COM技术实现的机器人通信库，实现与FANUC机器人的通信功能。该库使用ATL和CComPtr处理COM对象生命周期，提供稳定可靠的接口。

## 特点

- 使用ATL和CComPtr实现，无MFC依赖
- 保留与原FRRJIf.dll和FRRobotIFLib.dll完全兼容的接口
- 支持所有原始功能，包括：
  - 机器人连接和状态监控
  - 读写各类寄存器和I/O点
  - 位置和关节数据操作
  - 报警处理

## 目录结构

- `RobotComm.h` - 主头文件，提供整个库的入口点
- `RobotComm/` - 基于frrjif.h生成的组件
- `libCore/` - 基于frrobotiflib.h生成的组件

## 构建项目

### 使用CMake构建

1. 确保系统安装了CMake (3.14+) 和 Visual Studio 2019/2022

2. 配置和构建项目:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

3. 或者使用CMake预设:

   对于Visual Studio 2022:
   ```
   cmake --preset Release
   cmake --build --preset Release
   ```

   对于Visual Studio 2019:
   ```
   cmake --preset Release-VS2019
   cmake --build --preset Release-VS2019
   ```

### 可用的CMake预设

| 预设名称 | 说明 | Visual Studio版本 |
|---------|------|-----------------|
| Debug | 调试构建 | VS 2022 |
| Release | 发布构建 | VS 2022 |
| DevWithSample | 开发构建(含示例) | VS 2022 |
| Debug-VS2019 | 调试构建 | VS 2019 |
| Release-VS2019 | 发布构建 | VS 2019 |
| DevWithSample-VS2019 | 开发构建(含示例) | VS 2019 |

## 在其他项目中使用

### 在CMake项目中使用

```cmake
# 添加RobotComm库
add_subdirectory(path/to/RobotComm)

# 链接到您的目标
target_link_libraries(YourProject PRIVATE RobotComm)
```

### 在Visual Studio项目中使用

1. 构建RobotComm库
2. 将生成的lib文件和头文件添加到您的项目中
3. 确保您的项目中正确引用了COM库

## 使用示例

```cpp
#include "RobotComm.h"

int main() {
    // 初始化COM库
    RobotComm::Initialize();

    // 创建机器人控制实例
    RobotComm::Core robotCore;

    // 连接到机器人
    if (robotCore.Connect(_T("192.168.1.1"))) {
        // 获取数据表
        CComPtr<IDispatch> pDispatchTable = robotCore.GetDataTable();
        RobotComm::DataTable dataTable(pDispatchTable);
        
        // 创建LibCore访问接口
        CComPtr<IDispatch> pDispatchLib = robotCore.GetLib();
        RobotComm::LibCore libCore(pDispatchLib);
        
        // 读取SDO数据
        short buf[5] = {0};
        libCore.ReadSDO(1, buf, 5);
        
        // 断开连接
        robotCore.Disconnect();
    }

    // 在应用程序结束时释放COM库
    RobotComm::Uninitialize();
    
    return 0;
}
```

## 注意事项

- 必须在使用前调用`RobotComm::Initialize()`初始化COM库
- 使用完毕后应调用`RobotComm::Uninitialize()`释放COM库
- 确保系统中已正确注册COM组件(FRRJIf.dll和FRRobotIFLib.dll)
- 所有字符串使用TCHAR，支持Unicode

## 项目生成说明

本项目的代码是通过Python脚本自动从FANUC机器人接口示例代码生成的：

- robotcomm_generator.py - 从frrjif.h/cpp生成Core、DataTable等类
- libcore_generator.py - 从frrobotiflib.h/cpp生成LibCore类 