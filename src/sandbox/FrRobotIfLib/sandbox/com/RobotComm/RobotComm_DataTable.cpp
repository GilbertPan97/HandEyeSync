#include "RobotComm_DataTable.h"

namespace RobotComm {

DataTable::DataTable() {
    m_pDispatch = NULL;
}

DataTable::DataTable(LPDISPATCH pDispatch) {
    m_pDispatch = NULL;
    Initialize(pDispatch);
}

DataTable::~DataTable() {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
}

void DataTable::Initialize(LPDISPATCH pDispatch) {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
    if(pDispatch) {
        m_pDispatch = pDispatch;
        m_pDispatch->AddRef();
    }
}

BOOL DataTable::Clear() {
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
    HRESULT hr = m_pDispatch->Invoke(0x60030003, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataTable::Refresh() {
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
    HRESULT hr = m_pDispatch->Invoke(0x60030004, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&vResult);
    return result;
}

long DataTable::Count() {
    if (!m_pDispatch) {
        return 0;
    }

    long result = 0;
    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030005, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    }

    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::Item(const VARIANT& Index) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[1];
    VariantInit(&args[0]);

    // 参数按照倒序传递
    VariantCopy(&args[0], const_cast<VARIANT*>(&Index));

    dp.rgvarg = args;
    dp.cArgs = 1;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030006, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddNumReg(long DataType, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[3];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);

    // 参数按照倒序传递
    args[2].vt = VT_I4;
    args[2].lVal = DataType;
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 3;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030007, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddPosReg(long DataType, long Group, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[4];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);

    // 参数按照倒序传递
    args[3].vt = VT_I4;
    args[3].lVal = DataType;
    args[2].vt = VT_I4;
    args[2].lVal = Group;
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 4;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030008, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddPosRegXyzwpr(long DataType, long Group, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[4];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);

    // 参数按照倒序传递
    args[3].vt = VT_I4;
    args[3].lVal = DataType;
    args[2].vt = VT_I4;
    args[2].lVal = Group;
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 4;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030035, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddPosRegMG(long DataType, LPCTSTR Group, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[4];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);

    // 参数按照倒序传递
    args[3].vt = VT_I4;
    args[3].lVal = DataType;
    args[2].vt = VT_BSTR;
    args[2].bstrVal = SysAllocString(Group);
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 4;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030048, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddCurPos(long DataType, long Group) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[2];
    VariantInit(&args[0]);
    VariantInit(&args[1]);

    // 参数按照倒序传递
    args[1].vt = VT_I4;
    args[1].lVal = DataType;
    args[0].vt = VT_I4;
    args[0].lVal = Group;

    dp.rgvarg = args;
    dp.cArgs = 2;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030009, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddCurPosUF(long DataType, long Group, long UF) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[3];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);

    // 参数按照倒序传递
    args[2].vt = VT_I4;
    args[2].lVal = DataType;
    args[1].vt = VT_I4;
    args[1].lVal = Group;
    args[0].vt = VT_I4;
    args[0].lVal = UF;

    dp.rgvarg = args;
    dp.cArgs = 3;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030036, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddTask(long DataType, long Index) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[2];
    VariantInit(&args[0]);
    VariantInit(&args[1]);

    // 参数按照倒序传递
    args[1].vt = VT_I4;
    args[1].lVal = DataType;
    args[0].vt = VT_I4;
    args[0].lVal = Index;

    dp.rgvarg = args;
    dp.cArgs = 2;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000a, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddSysVarPos(long DataType, LPCTSTR SysVarName) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[2];
    VariantInit(&args[0]);
    VariantInit(&args[1]);

    // 参数按照倒序传递
    args[1].vt = VT_I4;
    args[1].lVal = DataType;
    args[0].vt = VT_BSTR;
    args[0].bstrVal = SysAllocString(SysVarName);

    dp.rgvarg = args;
    dp.cArgs = 2;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000b, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddSysVar(long DataType, LPCTSTR SysVarName) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[2];
    VariantInit(&args[0]);
    VariantInit(&args[1]);

    // 参数按照倒序传递
    args[1].vt = VT_I4;
    args[1].lVal = DataType;
    args[0].vt = VT_BSTR;
    args[0].bstrVal = SysAllocString(SysVarName);

    dp.rgvarg = args;
    dp.cArgs = 2;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000c, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddAlarm(long DataType, long AlarmCount, long AlarmMessageMode) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[3];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);

    // 参数按照倒序传递
    args[2].vt = VT_I4;
    args[2].lVal = DataType;
    args[1].vt = VT_I4;
    args[1].lVal = AlarmCount;
    args[0].vt = VT_I4;
    args[0].lVal = AlarmMessageMode;

    dp.rgvarg = args;
    dp.cArgs = 3;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x6003000d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&vResult);
    return result;
}

BOOL DataTable::GetLastDataFloat(long lngAddress, float* sngValue) {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result = FALSE;
    DISPPARAMS dp = {};
    VARIANT args[2];
    VariantInit(&args[0]);
    VariantInit(&args[1]);

    // 参数按照倒序传递
    args[1].vt = VT_I4;
    args[1].lVal = lngAddress;
    args[0].vt = VT_R4 | VT_BYREF;
    args[0].pfltVal = sngValue;

    dp.rgvarg = args;
    dp.cArgs = 2;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030012, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&vResult);
    return result;
}

BOOL DataTable::GetFixed() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030013, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

long DataTable::GetObjectID() {
    if (!m_pDispatch) {
        return 0;
    }

    long result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030014, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
        result = vResult.lVal;
    } else {
        result = 0;
    }

    VariantClear(&vResult);
    return result;
}

BOOL DataTable::GetValid() {
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

void DataTable::Kill() {
    if (!m_pDispatch) {
        return;
    }

    DISPPARAMS dp = {};
    // 无参数
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    m_pDispatch->Invoke(0x60030022, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

}

CString DataTable::GetTypeName() {
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
    HRESULT hr = m_pDispatch->Invoke(0x6003002f, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
        result = vResult.bstrVal;
    }

    VariantClear(&vResult);
    return result;
}

void DataTable::AddUserMessage(LPCTSTR vstrMessage) {
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

    m_pDispatch->Invoke(0x60030030, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

    VariantClear(&args[0]);
}

BOOL DataTable::GetDebugLog() {
    if (!m_pDispatch) {
        return FALSE;
    }

    BOOL result;
    DISPPARAMS dp = {};
    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x68030031, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
        result = vResult.boolVal;
    } else {
        result = FALSE;
    }

    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddString(long DataType, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[3];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);

    // 参数按照倒序传递
    args[2].vt = VT_I4;
    args[2].lVal = DataType;
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 3;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030043, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&vResult);
    return result;
}

LPDISPATCH DataTable::AddStringEx(long DataType, LPCTSTR DataSymbol, long StartIndex, long EndIndex) {
    if (!m_pDispatch) {
        return NULL;
    }

    LPDISPATCH result = NULL;
    DISPPARAMS dp = {};
    VARIANT args[4];
    VariantInit(&args[0]);
    VariantInit(&args[1]);
    VariantInit(&args[2]);
    VariantInit(&args[3]);

    // 参数按照倒序传递
    args[3].vt = VT_I4;
    args[3].lVal = DataType;
    args[2].vt = VT_BSTR;
    args[2].bstrVal = SysAllocString(DataSymbol);
    args[1].vt = VT_I4;
    args[1].lVal = StartIndex;
    args[0].vt = VT_I4;
    args[0].lVal = EndIndex;

    dp.rgvarg = args;
    dp.cArgs = 4;
    dp.rgdispidNamedArgs = NULL;
    dp.cNamedArgs = 0;

    VARIANT vResult;
    EXCEPINFO excepInfo = {};
    UINT argerr = 0;

    VariantInit(&vResult);
    HRESULT hr = m_pDispatch->Invoke(0x60030044, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

    if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
        result = vResult.pdispVal;
        result->AddRef();  // 增加引用计数
    }

    VariantClear(&args[0]);
    VariantClear(&args[1]);
    VariantClear(&args[2]);
    VariantClear(&args[3]);
    VariantClear(&vResult);
    return result;
}

} // namespace RobotComm
