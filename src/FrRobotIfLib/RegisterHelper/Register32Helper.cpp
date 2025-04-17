#include <windows.h>
#include <iostream>
#include <string>

bool RegisterDll(const std::string& regExePath, const std::string& dllPath) {
    // 创建注册命令
    std::string command = "\"" + regExePath + "\" /s \"" + dllPath + "\"";

    // 设置启动信息
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // 执行注册命令
    BOOL result = CreateProcessA(
        NULL,
        const_cast<char*>(command.c_str()),
        NULL, NULL, FALSE,
        CREATE_NO_WINDOW, // 隐藏窗口
        NULL, NULL,
        &si, &pi
    );

    if (!result) {
        std::cerr << "Failed to start regsvr32.exe. Error code: " << GetLastError() << std::endl;
        return false;
    }

    // 等待命令执行完毕
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    // 关闭进程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // 根据退出码判断是否成功
    if (exitCode == 0) {
        std::cout << "DLL registered successfully." << std::endl;
        return true;
    } else {
        std::cerr << "regsvr32.exe failed with exit code: " << exitCode << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    // // 检查输入参数个数
    // if (argc != 3) {
    //     std::cerr << "Usage: Register32BitDll.exe <RegExePath> <DllPath>" << std::endl;
    //     return 1;
    // }

    // std::string regExePath = argv[1];
    // std::string dllPath = argv[2];

    // 硬编码的路径
    std::string regExePath = "C:\\Windows\\SysWOW64\\regsvr32.exe"; // regsvr32 路径
    std::string dllPath = "C:\\Program Files (x86)\\FANUC\\FRRJIf\\FRRJIf.dll"; // DLL 路径

    // 调用注册函数
    if (RegisterDll(regExePath, dllPath)) {
        std::cout << "DLL registered successfully." << std::endl;
    } else {
        std::cout << "DLL registration failed." << std::endl;
    }

    // 防止窗口关闭，等待用户输入
    system("pause");

    return 0;
}
