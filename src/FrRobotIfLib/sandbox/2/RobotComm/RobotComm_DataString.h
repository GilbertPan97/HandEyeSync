#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataString {
public:
    DataString();
    DataString(LPDISPATCH pDispatch);
    ~DataString();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    long GetStartIndex();
    long GetEndIndex();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValue(long Index, BSTR* Value);
    BOOL SetValue(long Index, LPCTSTR Value);
    BOOL Update();
    void Reset();
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
