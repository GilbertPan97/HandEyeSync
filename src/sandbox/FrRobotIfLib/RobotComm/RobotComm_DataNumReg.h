#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataNumReg {
public:
    DataNumReg();
    DataNumReg(LPDISPATCH pDispatch);
    ~DataNumReg();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    long GetStartIndex();
    long GetEndIndex();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValue(long Index, VARIANT* Value);
    BOOL SetValue(long Index, const VARIANT& Value);
    BOOL SetValuesInt(long Index, long* Value, long Count);
    BOOL SetValuesReal(long Index, float* Value, long Count);
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
