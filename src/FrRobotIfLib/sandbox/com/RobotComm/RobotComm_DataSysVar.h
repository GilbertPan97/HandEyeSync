#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataSysVar {
public:
    DataSysVar();
    DataSysVar(LPDISPATCH pDispatch);
    ~DataSysVar();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValue(VARIANT* Value);
    CString GetSysVarName();
    BOOL SetValue(const VARIANT& Value);
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
