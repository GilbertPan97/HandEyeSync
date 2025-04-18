#include "RobotComm.h"
#include <iostream>
#include <comdef.h>  // 添加COM错误处理支持
#include <windows.h>  // 添加此行以支持Sleep函数
#include <vector>     // 添加vector支持

// 简单的错误处理宏
#define CHECK_SUCCESS(expr, msg) \
    if (!(expr)) { \
        std::wcout << msg << std::endl; \
        return 1; \
    }

// 添加刷新数据的函数（与OnButtonRefresh类似）
void RefreshRobotData(RobotComm::DataTable& dataTable, RobotComm::LibCore& libCore, 
                     RobotComm::DataNumReg& numReg, RobotComm::DataNumReg& numReg2,
                     RobotComm::DataSysVar& sysVar, RobotComm::DataCurPos& curPos) {
    
    std::wcout << L"\n========== Robot Data Refresh ==========\n" << std::endl;
    
    // 刷新数据表获取最新数据
    if (!dataTable.Refresh()) {
        std::wcout << L"Failed to refresh data table!" << std::endl;
        return;
    }
    // 添加等待时间，确保数据传输完成
    Sleep(500);
    
    // 读取系统变量
    VARIANT varValue;
    VariantInit(&varValue);
    
    std::wcout << L"--- System Variables ---" << std::endl;
    if (sysVar.GetValue(&varValue)) {
        std::wcout << sysVar.GetSysVarName() << L" = ";
        if (varValue.vt == VT_I4) {
            std::wcout << varValue.lVal << std::endl;
        } else if (varValue.vt == VT_R4) {
            std::wcout << varValue.fltVal << std::endl;
        } else {
            std::wcout << L"(Unknown type: " << varValue.vt << L")" << std::endl;
        }
    } else {
        std::wcout << sysVar.GetSysVarName() << L" : Error" << std::endl;
    }
    VariantClear(&varValue);
    VariantInit(&varValue);  // 确保清理后重新初始化
    
    // 读取当前位置
    float X, Y, Z, W, P, R, E1, E2, E3;
    short C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC;
    float J1, J2, J3, J4, J5, J6, J7, J8, J9;
    short ValidJ;
    
    std::wcout << L"\n--- Current Position ---" << std::endl;
    if (curPos.GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC)) {
        std::wcout << L"UF = " << UF << L", UT = " << UT << std::endl;
        std::wcout << X << L" " << Y << L" " << Z << L" " << W << L" " << P << L" " << R << L" " << E1 << L" " << E2 << L" " << E3 << std::endl;
        std::wcout << (C1 ? L"F" : L"N") << (C2 ? L"L" : L"R") << (C3 ? L"U" : L"D") << (C4 ? L"T" : L"B") << L" ";
        std::wcout << C5 << L" " << C6 << L" " << C7 << std::endl;
    } else {
        std::wcout << L"Current Position Error" << std::endl;
    }
    
    if (curPos.GetValueJoint(&J1, &J2, &J3, &J4, &J5, &J6, &J7, &J8, &J9, &UT, &ValidJ)) {
        if (ValidJ) {
            std::wcout << J1 << L" " << J2 << L" " << J3 << L" " << J4 << L" " << J5 << L" " << J6 << L" " << J7 << L" " << J8 << L" " << J9 << std::endl;
        }
    } else {
        std::wcout << L"Current Position Joint Error" << std::endl;
    }
    
    // 读取数值寄存器
    std::wcout << L"\n--- Numeric Registers ---" << std::endl;
    
    // 整数型寄存器
    VariantInit(&varValue);
    for (int ii = numReg.GetStartIndex(); ii <= numReg.GetEndIndex(); ii++) {
        if (numReg.GetValue(ii, &varValue)) {
            if (varValue.vt == VT_I4) {
                std::wcout << L"R[" << ii << L"] = " << varValue.lVal << std::endl;
            } else if (varValue.vt == VT_R4) {
                std::wcout << L"R[" << ii << L"] = " << varValue.fltVal << std::endl;
            } else if (varValue.vt == VT_BSTR) {
                // 处理字符串（如果有）
            } else {
                std::wcout << L"R[" << ii << L"] = (Unknown type)" << std::endl;
            }
        } else {
            std::wcout << L"R[" << ii << L"] : Error" << std::endl;
        }
        VariantClear(&varValue);
        VariantInit(&varValue);
    }
    
    // 浮点型寄存器
    for (int ii = numReg2.GetStartIndex(); ii <= numReg2.GetEndIndex(); ii++) {
        if (numReg2.GetValue(ii, &varValue)) {
            if (varValue.vt == VT_R4) {
                std::wcout << L"R[" << ii << L"] = " << varValue.fltVal << std::endl;
            } else if (varValue.vt == VT_I4) {
                std::wcout << L"R[" << ii << L"] = " << varValue.lVal << std::endl;
            } else if (varValue.vt == VT_BSTR) {
                // 处理字符串（如果有）
            } else {
                std::wcout << L"R[" << ii << L"] = (Unknown type)" << std::endl;
            }
        } else {
            std::wcout << L"R[" << ii << L"] : Error" << std::endl;
        }
        VariantClear(&varValue);
        VariantInit(&varValue);
    }
    
    // I/O部分
    std::wcout << L"\n--- I/O ---" << std::endl;
    std::wcout << L"I/O reading not implemented" << std::endl;
    
    std::wcout << L"\n======================================\n" << std::endl;
}

// 显示命令行帮助
void ShowHelp() {
    std::wcout << L"RobotComm Sample Usage" << std::endl;
    std::wcout << L"---------------------" << std::endl;
    std::wcout << L"Available commands:" << std::endl;
    std::wcout << L"  -h, --help     : Show this help" << std::endl;
    std::wcout << L"  -a, --ip <addr>: Connect to robot at specified IP address" << std::endl;
    std::wcout << L"  -s, --sysvar   : Read system variables" << std::endl;
    std::wcout << L"  -p, --position : Read current position" << std::endl;
    std::wcout << L"  -r, --register : Read numeric registers" << std::endl;
    std::wcout << L"  -f, --refresh  : Refresh all data" << std::endl;
    std::wcout << L"  -all           : Execute all operations" << std::endl;
    std::wcout << L"If no operation is specified, all operations will be performed." << std::endl;
}

// 添加测试LibCore的get接口函数
void TestLibCoreGetters(RobotComm::LibCore& libCore) {
    std::wcout << L"\n========== LibCore Properties ==========\n" << std::endl;
    
    // 获取并打印主机名
    CString hostName = libCore.GetHostName();
    std::wcout << L"HostName: " << (LPCTSTR)hostName << std::endl;
    
    // 获取并打印超时值
    long timeOutValue = libCore.GetTimeOutValue();
    std::wcout << L"TimeOutValue: " << timeOutValue << std::endl;
    
    // 获取并打印端口号
    long portNumber = libCore.GetPortNumber();
    std::wcout << L"PortNumber: " << portNumber << std::endl;
    
    // 获取并打印版本信息
    long major = libCore.GetMajor();
    long minor = libCore.GetMinor();
    long revision = libCore.GetRevision();
    std::wcout << L"Version: " << major << L"." << minor << L"." << revision << std::endl;
    
    std::wcout << L"\n======================================\n" << std::endl;
}

// 参考C# RobotDef.cs实现的数值寄存器读取函数
bool ReadNumericRegisters(RobotComm::DataTable& dataTable, std::vector<RobotComm::DataNumReg>& numRegs) {
    // 刷新数据表获取最新数据
    if (!dataTable.Refresh()) {
        std::wcout << L"Failed to refresh data table!" << std::endl;
        return false;
    }
    // 等待数据刷新完成
    Sleep(500);
    
    std::wcout << L"\n========== Numeric Registers ==========\n" << std::endl;
    
    VARIANT varValue;
    
    // 遍历所有寄存器对象
    for (size_t i = 0; i < numRegs.size(); i++) {
        auto& reg = numRegs[i];
        
        // 读取寄存器范围内的所有值
        for (int index = reg.GetStartIndex(); index <= reg.GetEndIndex(); index++) {
            // 初始化VARIANT
            VariantInit(&varValue);
            
            if (reg.GetValue(index, &varValue)) {
                std::wcout << L"R[" << index << L"] = ";
                if (varValue.vt == VT_I4) {
                    std::wcout << varValue.lVal << L" (INT)" << std::endl;
                } else if (varValue.vt == VT_R4) {
                    std::wcout << varValue.fltVal << L" (REAL)" << std::endl;
                } else if (varValue.vt == VT_BSTR) {
                    std::wcout << varValue.bstrVal << L" (STRING)" << std::endl;
                } else {
                    std::wcout << L"(Unknown type: " << varValue.vt << L")" << std::endl;
                }
            } else {
                std::wcout << L"R[" << index << L"] : Error reading value" << std::endl;
            }
            
            // 清理VARIANT
            VariantClear(&varValue);
        }
    }
    
    std::wcout << L"\n======================================\n" << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    // 默认IP地址
    TCHAR robotIP[256] = L"127.0.0.1";

    // 操作标志
    bool readSysVar = false;
    bool readPosition = false;
    bool readRegister = false;
    bool writeRegister = false;
    bool refreshAll = false;
    bool showHelp = false;
    
    // 处理命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            showHelp = true;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--ip") == 0) {
            if (i + 1 < argc) {
                #ifdef UNICODE
                    MultiByteToWideChar(CP_ACP, 0, argv[i+1], -1, robotIP, 256);
                #else
                    strcpy_s(robotIP, 256, argv[i+1]);
                #endif
                i++;
            }
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sysvar") == 0) {
            readSysVar = true;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--position") == 0) {
            readPosition = true;
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--register") == 0) {
            readRegister = true;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--refresh") == 0) {
            refreshAll = true;
        } else if (strcmp(argv[i], "-all") == 0) {
            readSysVar = readPosition = readRegister = refreshAll = true;
        }
    }
    
    // 修改默认操作设置
    if (showHelp || (!readSysVar && !readPosition && !readRegister && !refreshAll)) {
        if (showHelp) {
            ShowHelp();
            return 0;
        }
        // 默认只执行读取操作，不包含写入
        readSysVar = readPosition = readRegister = refreshAll = true;
        writeRegister = false;  // 明确禁用写入
    }
    
    // 输出平台信息
    std::wcout << L"Running on " << (sizeof(void*) == 8 ? L"64-bit" : L"32-bit") << L" platform" << std::endl;
    std::wcout << L"Attempting to connect to robot IP: " << robotIP << std::endl;
    
    // 初始化COM库
    HRESULT hr = RobotComm::Initialize();
    CHECK_SUCCESS(SUCCEEDED(hr), L"Error: Failed to initialize COM library");

    // 添加全局连接状态标志
    bool isConnected = false;

    try {
        // 创建机器人控制实例
        RobotComm::Core robotCore;
        
        // 初始化Core对象（创建COM实例）
        BOOL bInitialized = robotCore.Initialize();
        CHECK_SUCCESS(bInitialized, L"Error: Failed to initialize robot core");
        
        // 设置超时和其他参数
        robotCore.SetTimeOutValue(5000);  // 增加到5秒以提高连接可靠性
        robotCore.SetDebugLog(TRUE);     // 启用调试日志，便于问题排查

        // 先获取数据表，添加需要读取的数据项
        CComPtr<IDispatch> pDispatchTable = robotCore.GetDataTable();
        CHECK_SUCCESS(pDispatchTable != NULL, L"Error: Failed to get data table");
        
        RobotComm::DataTable dataTable(pDispatchTable);
        
        // 获取LibCore接口
        CComPtr<IDispatch> pDispatchLib = robotCore.GetLib();
        CHECK_SUCCESS(pDispatchLib != NULL, L"Error: Failed to get lib interface");
        
        RobotComm::LibCore libCore(pDispatchLib);
        
        // 提前添加所需的数据项
        std::wcout << L"Adding data items..." << std::endl;
        
        // 系统变量
        CComPtr<IDispatch> pSysVar = dataTable.AddSysVar(RobotComm::SYSVAR_INT, L"$FAST_CLOCK");
        CHECK_SUCCESS(pSysVar != NULL, L"Error: Failed to add system variable");
        
        // 当前位置
        CComPtr<IDispatch> pCurPos = dataTable.AddCurPos(RobotComm::CURPOS, 1);
        CHECK_SUCCESS(pCurPos != NULL, L"Error: Failed to add current position");
        
        // 添加整数型寄存器 (1-5)
        CComPtr<IDispatch> pDispNumReg = dataTable.AddNumReg(RobotComm::NUMREG_INT, 1, 5);
        CHECK_SUCCESS(pDispNumReg != NULL, L"Error: Failed to add integer numeric register");
        
        // 添加浮点型寄存器 (6-10)
        CComPtr<IDispatch> pDispNumReg2 = dataTable.AddNumReg(RobotComm::NUMREG_REAL, 6, 10);
        CHECK_SUCCESS(pDispNumReg2 != NULL, L"Error: Failed to add floating-point numeric register");
        
        // 创建报警列表对象 (历史报警, 最多5个)
        CComPtr<IDispatch> pAlarmList = dataTable.AddAlarm(RobotComm::ALARM_LIST, 5, 0);
        CHECK_SUCCESS(pAlarmList != NULL, L"Error: Failed to add alarm list");
        
        // 创建当前报警对象
        CComPtr<IDispatch> pAlarmCurrent = dataTable.AddAlarm(RobotComm::ALARM_CURRENT, 1, 0);
        CHECK_SUCCESS(pAlarmCurrent != NULL, L"Error: Failed to add current alarm");
        
        std::wcout << L"Data items added successfully" << std::endl;

        // 连接到机器人
        std::wcout << L"Connecting to robot at " << robotIP << L"..." << std::endl;
        
        BOOL bConnected = robotCore.Connect(robotIP);
        CHECK_SUCCESS(bConnected, L"Error: Failed to connect to robot");
        
        // 设置连接状态标志
        isConnected = true;
        std::wcout << L"Connected to robot!" << std::endl;
        
        // 测试LibCore的get接口
        TestLibCoreGetters(libCore);
        
        // 初始添加一次刷新，确保数据就绪
        std::wcout << L"Performing initial data refresh..." << std::endl;
        if (!dataTable.Refresh()) {
            std::wcout << L"Warning: Initial refresh failed" << std::endl;
        }
        // 增加等待时间，确保数据传输完成
        Sleep(1000);  // 增加到1秒等待时间

        // 根据用户选择读取系统变量
        if (readSysVar) {
            std::wcout << L"Reading system variables..." << std::endl;
            
            // 添加刷新操作，确保获取最新数据
            dataTable.Refresh();
            Sleep(1000);  // 增加等待时间
            
            // 获取系统变量值
            VARIANT varValue;
            VariantInit(&varValue);
            
            RobotComm::DataSysVar sysVar(pSysVar);
            if (sysVar.GetValue(&varValue)) {
                std::wcout << sysVar.GetSysVarName() << L" = ";
                if (varValue.vt == VT_I4) {
                    std::wcout << varValue.lVal << std::endl;
                } else if (varValue.vt == VT_R4) {
                    std::wcout << varValue.fltVal << std::endl;
                } else {
                    std::wcout << L"(Unknown type: " << varValue.vt << L")" << std::endl;
                }
            } else {
                std::wcout << L"Failed to read system variable value" << std::endl;
            }
            
            // 清理VARIANT
            VariantClear(&varValue);
            std::wcout << L"System variable read successfully" << std::endl;
        }

        // 读取当前位置
        if (readPosition) {
            std::wcout << L"Reading current position..." << std::endl;
            
            // 每次操作前都刷新数据表
            dataTable.Refresh();
            Sleep(1000);  // 增加等待时间
            
            // 读取位置
            RobotComm::DataCurPos curPos(pCurPos);
            float X, Y, Z, W, P, R, E1, E2, E3;
            short C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC;
            
            if (curPos.GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC)) {
                std::wcout << L"Position (XYZWPR): " << std::endl;
                std::wcout << L"UF = " << UF << L", UT = " << UT << std::endl;
                std::wcout << X << L" " << Y << L" " << Z << L" " << W << L" " << P << L" " << R << std::endl;
            } else {
                std::wcout << L"Failed to read current position" << std::endl;
            }
            
            std::wcout << L"Current position information read successfully" << std::endl;
        }
        
        // 添加报警读取功能
        std::wcout << L"Reading alarm information..." << std::endl;
        
        // 刷新数据表，确保获取最新报警数据
        dataTable.Refresh();
        Sleep(1000);  // 增加等待时间
        
        // 读取并显示报警信息
        RobotComm::DataAlarm alarmList(pAlarmList);
        RobotComm::DataAlarm alarmCurrent(pAlarmCurrent);
        // ReadAlarms(alarmList, alarmCurrent);
        
        std::wcout << L"Alarm information read successfully" << std::endl;
        
        // 读取数值寄存器
        if (readRegister) {
            std::wcout << L"Reading numeric registers..." << std::endl;

            // 创建寄存器容器 (参考C# RobotDef.cs中的List<DataNumReg> mobjReg)
            std::vector<RobotComm::DataNumReg> numRegs;
            
            numRegs.push_back(RobotComm::DataNumReg(pDispNumReg));
            numRegs.push_back(RobotComm::DataNumReg(pDispNumReg2));
            
            // 使用简化的读取函数读取所有注册的寄存器
            if (ReadNumericRegisters(dataTable, numRegs)) {
                std::wcout << L"Numeric registers read successfully" << std::endl;
            } else {
                std::wcout << L"Failed to read numeric registers" << std::endl;
            }
        }

        // 全面刷新并显示所有数据
        if (refreshAll) {
            std::wcout << L"Performing complete data refresh..." << std::endl;
            
            RobotComm::DataSysVar sysVar(pSysVar);
            RobotComm::DataCurPos curPos(pCurPos);
            RobotComm::DataNumReg numReg(pDispNumReg);
            RobotComm::DataNumReg numReg2(pDispNumReg2);
            
            RefreshRobotData(dataTable, libCore, numReg, numReg2, sysVar, curPos);
            
            std::wcout << L"Data refresh completed" << std::endl;
        }

        // 断开连接
        std::wcout << L"Disconnecting..." << std::endl;
        robotCore.Disconnect();
        std::wcout << L"Disconnected" << std::endl;
    }
    catch (_com_error& e) {
        std::wcout << L"COM Error: " << e.ErrorMessage() << std::endl;
    }
    catch (const std::exception& e) {
        std::wcout << L"Error: " << e.what() << std::endl;
    }

    // 在应用程序结束时释放COM库
    RobotComm::Uninitialize();
    
    std::wcout << L"Sample completed. Press any key to exit..." << std::endl;
    _getwch(); // 等待用户按键后退出
    return 0;
} 