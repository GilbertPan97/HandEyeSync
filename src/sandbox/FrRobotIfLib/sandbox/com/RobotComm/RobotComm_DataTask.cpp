#include "RobotComm_DataTask.h"

namespace RobotComm {

DataTask::DataTask() {
    m_pDispatch = NULL;
}

DataTask::DataTask(LPDISPATCH pDispatch) {
    m_pDispatch = NULL;
    Initialize(pDispatch);
}

DataTask::~DataTask() {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
}

void DataTask::Initialize(LPDISPATCH pDispatch) {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
    if(pDispatch) {
        m_pDispatch = pDispatch;
        m_pDispatch->AddRef();
    }
}

long DataTask::GetDataType() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030003, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

long DataTask::GetIndex() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030002, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTask::GetDataTable() {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030001, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    } else {
        result = NULL;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataTask::GetValid() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030000, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataTask::GetValue(BSTR* ProgName, short* LineNumber, short* State, BSTR* ParentProgName) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[4];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);

    // 参数按照倒序传递
    args[3].vt = VT_BSTR | VT_BYREF;
    args[3].pbstrVal = ProgName;
    args[2].vt = VT_I2 | VT_BYREF;
    args[2].piVal = LineNumber;
    args[1].vt = VT_I2 | VT_BYREF;
    args[1].piVal = State;
    args[0].vt = VT_BSTR | VT_BYREF;
    args[0].pbstrVal = ParentProgName;

    dp.rgvarg = args;
    dp.cArgs = 4;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030007, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&vResult);
    return result;
}

long DataTask::GetObjectID() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030008, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void DataTask::Kill() {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x6003000c, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

}

CString DataTask::GetTypeName() {
    if (!m_pDispatch) {
        return _T("");
    }

    CString result;
    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030012, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
        result = vResult.bstrVal;
    }

    VariantClear(&vResult);
    return result;
}

void DataTask::AddUserMessage(LPCTSTR vstrMessage) {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    VARIANT args[1];
    VariantInit(&args[0]);

    // 参数按照倒序传递
    args[0].vt = VT_BSTR;
    args[0].bstrVal = SysAllocString(vstrMessage);

    dp.rgvarg = args;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x60030013, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&args[0]);
}

BOOL DataTask::GetDebugLog() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030014, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

} // namespace RobotComm
