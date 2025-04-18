#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataSysVarPos {
public:
    DataSysVarPos();
    DataSysVarPos(LPDISPATCH pDispatch);
    ~DataSysVarPos();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValueXyzwpr(float* X, float* Y, float* Z, float* W, float* P, float* R, float* E1, float* E2, float* E3, short* C1, short* C2, short* C3, short* C4, short* C5, short* C6, short* C7, short* UF, short* UT, short* ValidC);
    BOOL GetValueJoint(float* J1, float* J2, float* J3, float* J4, float* J5, float* J6, float* J7, float* J8, float* J9, short* UT, short* ValidJ);
    CString GetSysVarName();
    BOOL SetValueJoint2(float J1, float J2, float J3, float J4, float J5, float J6, float J7, float J8, float J9, short UF, short UT);
    BOOL SetValueXyzwpr2(float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7, short UF, short UT);
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
