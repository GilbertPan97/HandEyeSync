#include "RobotComm.h"
#include <iostream>
#include <windows.h>  // 添加此行以支持Sleep函数
#include <vector>     // 添加vector支持

// // 简单的错误处理宏
// #define CHECK_SUCCESS(expr, msg) \
//     if (!(expr)) { \
//         std::wcout << msg << std::endl; \
//         return 1; \
//     }

int main(int argc, char* argv[]) {

    // 添加全局连接状态标志
    bool isConnected = false;

    // 创建机器人控制实例
    // gcroot<bridgeRobotIF::bridgeCore^> pCore;
    // RobotComm::Core robotCore;
    std::cout << "====================================" << std::endl;
    // int res = robotCore.Connect("127.0.0.1");
    // std::cout << "================" << res << "====================" << std::endl;
    
    return 0;
} 