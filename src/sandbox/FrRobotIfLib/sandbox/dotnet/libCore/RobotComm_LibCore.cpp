#include "RobotComm_LibCore.h"

namespace RobotComm {

LibCore::LibCore() {
}

LibCore::LibCore(LPDISPATCH pDispatch) {
    Initialize(pDispatch);
}

LibCore::~LibCore() {
    m_pDispatch = NULL;
}

void LibCore::Initialize(LPDISPATCH pDispatch) {
    m_pDispatch = pDispatch;
}

CString LibCore::GetHostName() {
    if (!m_pDispatch) {
        return _T("");
    }

    CString result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
        result = vResult.bstrVal;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetHostName(LPCTSTR propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_BSTR;
    arg.bstrVal = SysAllocString(reinterpret_cast<LPCOLESTR>(propVal));
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

long LibCore::GetTimeOutValue() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetTimeOutValue(long propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_I4;
    arg.lVal = propVal;
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

long LibCore::GetPortNumber() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetPortNumber(long propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_I4;
    arg.lVal = propVal;
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

long LibCore::GetMajor() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetMajor(long propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_I4;
    arg.lVal = propVal;
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

long LibCore::GetMinor() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetMinor(long propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_I4;
    arg.lVal = propVal;
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

long LibCore::GetRevision() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {0};
    VARIANT vResult;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void LibCore::SetRevision(long propVal) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {0};
    VARIANT arg;
    DISPID dispidPut = DISPID_PROPERTYPUT;
    EXCEPINFO excepInfo = {0};
    UINT argerr = 0;

    VariantInit(&arg);
    arg.vt = VT_I4;
    arg.lVal = propVal;
    dp.rgvarg = &arg;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = &dispidPut;
    dp.cNamedArgs = 1;

    m_pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&arg);
}

} // namespace RobotComm
