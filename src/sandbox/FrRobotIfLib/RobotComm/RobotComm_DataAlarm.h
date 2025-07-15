#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class DataAlarm {
public:
    DataAlarm();
    DataAlarm(LPDISPATCH pDispatch);
    ~DataAlarm();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 方法和属性
    long GetDataType();
    LPDISPATCH GetDataTable();
    BOOL GetValid();
    BOOL GetValue(long Count, short* AlarmID, short* AlarmNumber, short* CauseAlarmID, short* CauseAlarmNumber, short* Severity, short* Year, short* Month, short* Day, short* Hour, short* Minute, short* Second, BSTR* AlarmMessage, BSTR* CauseAlarmMessage, BSTR* SeverityMessage);
    long GetObjectID();
    void Kill();
    CString GetTypeName();
    void AddUserMessage(LPCTSTR vstrMessage);
    BOOL GetDebugLog();

private:
    IDispatch* m_pDispatch;
};

} // namespace RobotComm
