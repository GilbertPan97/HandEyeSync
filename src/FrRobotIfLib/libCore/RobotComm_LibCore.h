#pragma once
// Include necessary headers, but not RobotComm.h directly to avoid circular includes
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

namespace RobotComm {

class LibCore {
public:
    LibCore();
    LibCore(LPDISPATCH pDispatch);
    ~LibCore();

    // 初始化
    void Initialize(LPDISPATCH pDispatch);

    // 属性访问
    CString GetHostName();
    void SetHostName(LPCTSTR propVal);
    long GetTimeOutValue();
    void SetTimeOutValue(long propVal);
    long GetPortNumber();
    void SetPortNumber(long propVal);
    long GetMajor();
    void SetMajor(long propVal);
    long GetMinor();
    void SetMinor(long propVal);
    long GetRevision();
    void SetRevision(long propVal);

    // 通信方法

private:
    CComPtr<IDispatch> m_pDispatch;
};

} // namespace RobotComm
