#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <gcroot.h>

#using <bridgeRobotIF.dll>
#using <RobotInterfaceDotNet.dll>

using namespace FRRJIf;
using namespace bridgeRobotIF;

namespace RobotComm {

class Core {
public:
    Core();
    Core(LPDISPATCH pDispatch);
    ~Core();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);
    BOOL Initialize(); // 添加无参数版本，用于直接创建COM对象

    // 方法和属性
    long GetObjectID();
    BOOL Connect(LPCTSTR HostName);
    BOOL Disconnect();
    BOOL ClearAlarm(long vlngType);
    LPDISPATCH GetDataTable();
    LPDISPATCH GetDataTable2();
    BOOL GetProtectAvailable();
    long GetProtectTrialRemainDays();
    CString GetProtectStatus();
    long GetProtectErrorNumber();
    CString GetProtectPCID();
    BOOL ProtectSetPassword(LPCTSTR strPassword);
    long GetTimeOutValue();
    void SetTimeOutValue(long nNewValue);
    long GetMlngDataTableCount();
    long GetPortNumber();
    void SetPortNumber(long nNewValue);
    short GetClientID();
    void SetClientID(short nNewValue);
    BOOL GetIsConnected();
    CString GetDebugMessage();
    CString GetDebugMessages();
    long GetConnectState();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();
    void SetDebugLog(BOOL bNewValue);
    LPDISPATCH GetLib();

public:
    gcroot<bridgeRobotIF::bridgeCore^> pCore;
    gcroot<bridgeRobotIF::bridgeLibCore^> pLibCore;

private:
    LPDISPATCH m_pDispatch;
};

} // namespace RobotComm
