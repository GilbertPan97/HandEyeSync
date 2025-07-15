#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataTask {
public:
    DataTask();
    DataTask(LPDISPATCH pDispatch);
    ~DataTask();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    long GetIndex();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValue(BSTR* ProgName, short* LineNumber, short* State, BSTR* ParentProgName);
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
