#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataTable {
public:
    DataTable();
    DataTable(LPDISPATCH pDispatch);
    ~DataTable();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    BOOL Clear();
    BOOL Refresh();
    long Count();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH AddNumReg(long DataType, long StartIndex, long EndIndex);
    LPDISPATCH AddPosReg(long DataType, long Group, long StartIndex, long EndIndex);
    LPDISPATCH AddPosRegXyzwpr(long DataType, long Group, long StartIndex, long EndIndex);
    LPDISPATCH AddPosRegMG(long DataType, LPCTSTR Group, long StartIndex, long EndIndex);
    LPDISPATCH AddCurPos(long DataType, long Group);
    LPDISPATCH AddCurPosUF(long DataType, long Group, long UF);
    LPDISPATCH AddTask(long DataType, long Index);
    LPDISPATCH AddSysVarPos(long DataType, LPCTSTR SysVarName);
    LPDISPATCH AddSysVar(long DataType, LPCTSTR SysVarName);
    LPDISPATCH AddAlarm(long DataType, long AlarmCount, long AlarmMessageMode);
    BOOL GetLastDataFloat(long lngAddress, float* sngValue);
    BOOL GetFixed();
    long GetObjectID();
    BOOL GetValid();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();
    LPDISPATCH AddString(long DataType, long StartIndex, long EndIndex);
    LPDISPATCH AddStringEx(long DataType, LPCTSTR DataSymbol, long StartIndex, long EndIndex);

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
