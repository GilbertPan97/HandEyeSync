#include "RobotComm_DataAlarm.h"

namespace RobotComm {

DataAlarm::DataAlarm() {
    m_pDispatch = NULL;
}

DataAlarm::DataAlarm(LPDISPATCH pDispatch) {
    m_pDispatch = NULL;
    Initialize(pDispatch);
}

DataAlarm::~DataAlarm() {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
}

void DataAlarm::Initialize(LPDISPATCH pDispatch) {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
    if(pDispatch) {
        m_pDispatch = pDispatch;
        m_pDispatch->AddRef();
    }
}

long DataAlarm::GetDataType() {
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

LPDISPATCH DataAlarm::GetDataTable() {
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

BOOL DataAlarm::GetValid() {
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

BOOL DataAlarm::GetValue(long Count, short* AlarmID, short* AlarmNumber, short* CauseAlarmID, short* CauseAlarmNumber, short* Severity, short* Year, short* Month, short* Day, short* Hour, short* Minute, short* Second, BSTR* AlarmMessage, BSTR* CauseAlarmMessage, BSTR* SeverityMessage) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[15];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);
    VariantInit(&args[4]);
    VariantInit(&args[5]);
    VariantInit(&args[6]);
    VariantInit(&args[7]);
    VariantInit(&args[8]);
    VariantInit(&args[9]);
    VariantInit(&args[10]);
    VariantInit(&args[11]);
    VariantInit(&args[12]);
    VariantInit(&args[13]);
    VariantInit(&args[14]);

    // 参数按照倒序传递
    args[14].vt = VT_I4;
    args[14].lVal = Count;
    args[13].vt = VT_I2 | VT_BYREF;
    args[13].piVal = AlarmID;
    args[12].vt = VT_I2 | VT_BYREF;
    args[12].piVal = AlarmNumber;
    args[11].vt = VT_I2 | VT_BYREF;
    args[11].piVal = CauseAlarmID;
    args[10].vt = VT_I2 | VT_BYREF;
    args[10].piVal = CauseAlarmNumber;
    args[9].vt = VT_I2 | VT_BYREF;
    args[9].piVal = Severity;
    args[8].vt = VT_I2 | VT_BYREF;
    args[8].piVal = Year;
    args[7].vt = VT_I2 | VT_BYREF;
    args[7].piVal = Month;
    args[6].vt = VT_I2 | VT_BYREF;
    args[6].piVal = Day;
    args[5].vt = VT_I2 | VT_BYREF;
    args[5].piVal = Hour;
    args[4].vt = VT_I2 | VT_BYREF;
    args[4].piVal = Minute;
    args[3].vt = VT_I2 | VT_BYREF;
    args[3].piVal = Second;
    args[2].vt = VT_BSTR | VT_BYREF;
    args[2].pbstrVal = AlarmMessage;
    args[1].vt = VT_BSTR | VT_BYREF;
    args[1].pbstrVal = CauseAlarmMessage;
    args[0].vt = VT_BSTR | VT_BYREF;
    args[0].pbstrVal = SeverityMessage;

    dp.rgvarg = args;
    dp.cArgs = 15;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030006, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&args[4]);
    VariantClear(&args[5]);
    VariantClear(&args[6]);
    VariantClear(&args[7]);
    VariantClear(&args[8]);
    VariantClear(&args[9]);
    VariantClear(&args[10]);
    VariantClear(&args[11]);
    VariantClear(&args[12]);
    VariantClear(&args[13]);
    VariantClear(&args[14]);
    VariantClear(&vResult);
    return result;
}

long DataAlarm::GetObjectID() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030007, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void DataAlarm::Kill() {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x6003000d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

}

CString DataAlarm::GetTypeName() {
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
    HRESULT hr = m_pDispatch->Invoke(0x60030013, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
        result = vResult.bstrVal;
    }

    VariantClear(&vResult);
    return result;
}

void DataAlarm::AddUserMessage(LPCTSTR vstrMessage) {
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

    m_pDispatch->Invoke(0x60030014, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&args[0]);
}

BOOL DataAlarm::GetDebugLog() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030015, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

} // namespace RobotComm
