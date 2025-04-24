#include "RobotComm_DataPosRegMG.h"

namespace RobotComm {

DataPosRegMG::DataPosRegMG() {
    m_pDispatch = NULL;
}

DataPosRegMG::DataPosRegMG(LPDISPATCH pDispatch) {
    m_pDispatch = NULL;
    Initialize(pDispatch);
}

DataPosRegMG::~DataPosRegMG() {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
}

void DataPosRegMG::Initialize(LPDISPATCH pDispatch) {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
    if(pDispatch) {
        m_pDispatch = pDispatch;
        m_pDispatch->AddRef();
    }
}

long DataPosRegMG::GetDataType() {
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

long DataPosRegMG::GetGroupCount() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030006, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataPosRegMG::GetDataTable() {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030005, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    } else {
        result = NULL;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataPosRegMG::GetValid() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030004, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataPosRegMG::SetValueJoint2(long Index, long Group, float J1, float J2, float J3, float J4, float J5, float J6, float J7, float J8, float J9) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[11];
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

    // 参数按照倒序传递
    args[10].vt = VT_I4;
    args[10].lVal = Index;
    args[9].vt = VT_I4;
    args[9].lVal = Group;
    args[8].vt = VT_R4;
    args[8].fltVal = J1;
    args[7].vt = VT_R4;
    args[7].fltVal = J2;
    args[6].vt = VT_R4;
    args[6].fltVal = J3;
    args[5].vt = VT_R4;
    args[5].fltVal = J4;
    args[4].vt = VT_R4;
    args[4].fltVal = J5;
    args[3].vt = VT_R4;
    args[3].fltVal = J6;
    args[2].vt = VT_R4;
    args[2].fltVal = J7;
    args[1].vt = VT_R4;
    args[1].fltVal = J8;
    args[0].vt = VT_R4;
    args[0].fltVal = J9;

    dp.rgvarg = args;
    dp.cArgs = 11;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

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
    VariantClear(&vResult);
    return result;
}

BOOL DataPosRegMG::SetValueXyzwpr2(long Index, long Group, float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[18];
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
    VariantInit(&args[15]);
    VariantInit(&args[16]);
    VariantInit(&args[17]);

    // 参数按照倒序传递
    args[17].vt = VT_I4;
    args[17].lVal = Index;
    args[16].vt = VT_I4;
    args[16].lVal = Group;
    args[15].vt = VT_R4;
    args[15].fltVal = X;
    args[14].vt = VT_R4;
    args[14].fltVal = Y;
    args[13].vt = VT_R4;
    args[13].fltVal = Z;
    args[12].vt = VT_R4;
    args[12].fltVal = W;
    args[11].vt = VT_R4;
    args[11].fltVal = P;
    args[10].vt = VT_R4;
    args[10].fltVal = R;
    args[9].vt = VT_R4;
    args[9].fltVal = E1;
    args[8].vt = VT_R4;
    args[8].fltVal = E2;
    args[7].vt = VT_R4;
    args[7].fltVal = E3;
    args[6].vt = VT_I2;
    args[6].iVal = C1;
    args[5].vt = VT_I2;
    args[5].iVal = C2;
    args[4].vt = VT_I2;
    args[4].iVal = C3;
    args[3].vt = VT_I2;
    args[3].iVal = C4;
    args[2].vt = VT_I2;
    args[2].iVal = C5;
    args[1].vt = VT_I2;
    args[1].iVal = C6;
    args[0].vt = VT_I2;
    args[0].iVal = C7;

    dp.rgvarg = args;
    dp.cArgs = 18;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000f, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

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
    VariantClear(&args[15]);
    VariantClear(&args[16]);
    VariantClear(&args[17]);
    VariantClear(&vResult);
    return result;
}

BOOL DataPosRegMG::Update() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030010, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&vResult);
    return result;
}

long DataPosRegMG::GetStartIndex() {
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

long DataPosRegMG::GetEndIndex() {
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

long DataPosRegMG::GetObjectID() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030001, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

void DataPosRegMG::Kill() {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x60030013, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

}

CString DataPosRegMG::GetTypeName() {
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
    HRESULT hr = m_pDispatch->Invoke(0x60030014, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
        result = vResult.bstrVal;
    }

    VariantClear(&vResult);
    return result;
}

void DataPosRegMG::AddUserMessage(LPCTSTR vstrMessage) {
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

    m_pDispatch->Invoke(0x60030015, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&args[0]);
}

BOOL DataPosRegMG::GetDebugLog() {
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

void DataPosRegMG::Reset() {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x60030016, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

}

BOOL DataPosRegMG::GetValueJoint(long Index, long Group, float* J1, float* J2, float* J3, float* J4, float* J5, float* J6, float* J7, float* J8, float* J9) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[11];
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

    // 参数按照倒序传递
    args[10].vt = VT_I4;
    args[10].lVal = Index;
    args[9].vt = VT_I4;
    args[9].lVal = Group;
    args[8].vt = VT_R4 | VT_BYREF;
    args[8].pfltVal = J1;
    args[7].vt = VT_R4 | VT_BYREF;
    args[7].pfltVal = J2;
    args[6].vt = VT_R4 | VT_BYREF;
    args[6].pfltVal = J3;
    args[5].vt = VT_R4 | VT_BYREF;
    args[5].pfltVal = J4;
    args[4].vt = VT_R4 | VT_BYREF;
    args[4].pfltVal = J5;
    args[3].vt = VT_R4 | VT_BYREF;
    args[3].pfltVal = J6;
    args[2].vt = VT_R4 | VT_BYREF;
    args[2].pfltVal = J7;
    args[1].vt = VT_R4 | VT_BYREF;
    args[1].pfltVal = J8;
    args[0].vt = VT_R4 | VT_BYREF;
    args[0].pfltVal = J9;

    dp.rgvarg = args;
    dp.cArgs = 11;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030018, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

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
    VariantClear(&vResult);
    return result;
}

BOOL DataPosRegMG::GetValueXyzwpr(long Index, long Group, float* X, float* Y, float* Z, float* W, float* P, float* R, float* E1, float* E2, float* E3, short* C1, short* C2, short* C3, short* C4, short* C5, short* C6, short* C7) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[18];
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
    VariantInit(&args[15]);
    VariantInit(&args[16]);
    VariantInit(&args[17]);

    // 参数按照倒序传递
    args[17].vt = VT_I4;
    args[17].lVal = Index;
    args[16].vt = VT_I4;
    args[16].lVal = Group;
    args[15].vt = VT_R4 | VT_BYREF;
    args[15].pfltVal = X;
    args[14].vt = VT_R4 | VT_BYREF;
    args[14].pfltVal = Y;
    args[13].vt = VT_R4 | VT_BYREF;
    args[13].pfltVal = Z;
    args[12].vt = VT_R4 | VT_BYREF;
    args[12].pfltVal = W;
    args[11].vt = VT_R4 | VT_BYREF;
    args[11].pfltVal = P;
    args[10].vt = VT_R4 | VT_BYREF;
    args[10].pfltVal = R;
    args[9].vt = VT_R4 | VT_BYREF;
    args[9].pfltVal = E1;
    args[8].vt = VT_R4 | VT_BYREF;
    args[8].pfltVal = E2;
    args[7].vt = VT_R4 | VT_BYREF;
    args[7].pfltVal = E3;
    args[6].vt = VT_I2 | VT_BYREF;
    args[6].piVal = C1;
    args[5].vt = VT_I2 | VT_BYREF;
    args[5].piVal = C2;
    args[4].vt = VT_I2 | VT_BYREF;
    args[4].piVal = C3;
    args[3].vt = VT_I2 | VT_BYREF;
    args[3].piVal = C4;
    args[2].vt = VT_I2 | VT_BYREF;
    args[2].piVal = C5;
    args[1].vt = VT_I2 | VT_BYREF;
    args[1].piVal = C6;
    args[0].vt = VT_I2 | VT_BYREF;
    args[0].piVal = C7;

    dp.rgvarg = args;
    dp.cArgs = 18;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030019, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

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
    VariantClear(&args[15]);
    VariantClear(&args[16]);
    VariantClear(&args[17]);
    VariantClear(&vResult);
    return result;
}

} // namespace RobotComm
