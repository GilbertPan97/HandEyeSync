#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataPosReg {
public:
    DataPosReg();
    DataPosReg(LPDISPATCH pDispatch);
    ~DataPosReg();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    long GetGroup();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValueXyzwpr(long Index, float* X, float* Y, float* Z, float* W, float* P, float* R, float* E1, float* E2, float* E3, short* C1, short* C2, short* C3, short* C4, short* C5, short* C6, short* C7, short* UF, short* UT, short* validc);
    BOOL GetValueJoint(long Index, float* J1, float* J2, float* J3, float* J4, float* J5, float* J6, float* J7, float* J8, float* J9, short* UT, short* ValidJ);
    BOOL SetValueJoint2(long Index, float J1, float J2, float J3, float J4, float J5, float J6, float J7, float J8, float J9, short UF, short UT);
    BOOL SetValueXyzwpr2(long Index, float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7, short UF, short UT);
    long GetStartIndex();
    long GetEndIndex();
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
