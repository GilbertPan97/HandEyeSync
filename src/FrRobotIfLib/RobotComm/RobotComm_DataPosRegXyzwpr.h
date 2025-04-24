#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataPosRegXyzwpr {
public:
    DataPosRegXyzwpr();
    DataPosRegXyzwpr(LPDISPATCH pDispatch);
    ~DataPosRegXyzwpr();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    long GetGroup();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL SetValueXyzwpr2(long Index, float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7);
    BOOL Update();
    long GetStartIndex();
    long GetEndIndex();
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();
    void Reset();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
