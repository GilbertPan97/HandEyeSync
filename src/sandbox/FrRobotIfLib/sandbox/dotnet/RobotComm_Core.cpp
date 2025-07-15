#include "RobotComm_Core.h"

#include <iostream>

using namespace System;
using namespace System::Diagnostics;

namespace RobotComm {

Core::Core() {
    Initialize();
}

// Core::Core(LPDISPATCH pDispatch) {
//     m_pDispatch = NULL;
//     Initialize(pDispatch);
// }

Core::~Core() {
    if(m_pDispatch) {
        m_pDispatch->Release();
        m_pDispatch = NULL;
    }
}

// void Core::Initialize(LPDISPATCH pDispatch) {
//     if(m_pDispatch) {
//         m_pDispatch->Release();
//         m_pDispatch = NULL;
//     }
//     if(pDispatch) {
//         m_pDispatch = pDispatch;
//         m_pDispatch->AddRef();
//     }
// }

BOOL Core::Initialize() {
    try {
        // 设置编码格式
        bridgeCommon::setEncode("Shift-Jis");

        // 初始化核心功能模块
        auto encode = bridgeCommon::getEncode();
        auto core = gcnew FRRJIf::Core(encode);

        // 初始化 bridgeCore 和 bridgeLibCore
        pCore = gcnew bridgeCore(core);
        pLibCore = gcnew bridgeLibCore(pCore->get_Snpx());

        return TRUE;
    } catch (Exception^ ex) {
        // 可以记录日志或打印错误信息
        System::Console::WriteLine("Initialization failed: {0}", ex->Message);
        return FALSE;
    }
}

// long Core::GetObjectID() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x6803002b, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

BOOL Core::Connect(LPCTSTR HostName) {
    std::cout << "================ connect ====================" << std::endl;
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result = FALSE;
//     DISPPARAMS dp = {};
//     VARIANT args[1];
//     VariantInit(&args[0]);

//     // 参数按照倒序传递
//     args[0].vt = VT_BSTR;
//     args[0].bstrVal = SysAllocString(reinterpret_cast<LPCOLESTR>(HostName));

//     dp.rgvarg = args;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030006, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     }

//     VariantClear(&args[0]);
//     VariantClear(&vResult);
//     return result;

    int intRes;
    intRes = pCore->Connect(gcnew String(HostName));
    return intRes;
}

// BOOL Core::Disconnect() {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result = FALSE;
//     DISPPARAMS dp = {};
//     // 无参数
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030007, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// BOOL Core::ClearAlarm(long vlngType) {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result = FALSE;
//     DISPPARAMS dp = {};
//     VARIANT args[1];
//     VariantInit(&args[0]);

//     // 参数按照倒序传递
//     args[0].vt = VT_I4;
//     args[0].lVal = vlngType;

//     dp.rgvarg = args;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030028, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     }

//     VariantClear(&args[0]);
//     VariantClear(&vResult);
//     return result;
// }

// LPDISPATCH Core::GetDataTable() {
//     if (!m_pDispatch) {
//         return NULL;
//     }

//     LPDISPATCH result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030004, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
//         result = vResult.pdispVal;
//         result->AddRef();  // 增加引用计数
//     } else {
//         result = NULL;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// LPDISPATCH Core::GetDataTable2() {
//     if (!m_pDispatch) {
//         return NULL;
//     }

//     LPDISPATCH result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030023, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
//         result = vResult.pdispVal;
//         result->AddRef();  // 增加引用计数
//     } else {
//         result = NULL;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// BOOL Core::GetProtectAvailable() {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030003, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     } else {
//         result = FALSE;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// long Core::GetProtectTrialRemainDays() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030002, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// CString Core::GetProtectStatus() {
//     if (!m_pDispatch) {
//         return _T("");
//     }

//     CString result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030001, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
//         result = vResult.bstrVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// long Core::GetProtectErrorNumber() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030000, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// CString Core::GetProtectPCID() {
//     if (!m_pDispatch) {
//         return _T("");
//     }

//     CString result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x6803001e, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
//         result = vResult.bstrVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// BOOL Core::ProtectSetPassword(LPCTSTR strPassword) {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result = FALSE;
//     DISPPARAMS dp = {};
//     VARIANT args[1];
//     VariantInit(&args[0]);

//     // 参数按照倒序传递
//     args[0].vt = VT_BSTR;
//     args[0].bstrVal = SysAllocString(reinterpret_cast<LPCOLESTR>(strPassword));

//     dp.rgvarg = args;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030021, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     }

//     VariantClear(&args[0]);
//     VariantClear(&vResult);
//     return result;
// }

// long Core::GetTimeOutValue() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x6803001d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// void Core::SetTimeOutValue(long nNewValue) {
//     if (!m_pDispatch) {
//         return;
//     }

//     DISPPARAMS dp = {};
//     VARIANT arg;
//     DISPID dispidPut = DISPID_PROPERTYPUT;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&arg);
//     arg.vt = VT_I4;
//     arg.lVal = nNewValue;
//     dp.rgvarg = &arg;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = &dispidPut;
//     dp.cNamedArgs = 1;

//     m_pDispatch->Invoke(0x6803001d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

//     VariantClear(&arg);
// }

// long Core::GetMlngDataTableCount() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030022, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// long Core::GetPortNumber() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030024, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// void Core::SetPortNumber(long nNewValue) {
//     if (!m_pDispatch) {
//         return;
//     }

//     DISPPARAMS dp = {};
//     VARIANT arg;
//     DISPID dispidPut = DISPID_PROPERTYPUT;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&arg);
//     arg.vt = VT_I4;
//     arg.lVal = nNewValue;
//     dp.rgvarg = &arg;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = &dispidPut;
//     dp.cNamedArgs = 1;

//     m_pDispatch->Invoke(0x68030024, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

//     VariantClear(&arg);
// }

// short Core::GetClientID() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     short result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030025, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I2) {
//         result = vResult.iVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// void Core::SetClientID(short nNewValue) {
//     if (!m_pDispatch) {
//         return;
//     }

//     DISPPARAMS dp = {};
//     VARIANT arg;
//     DISPID dispidPut = DISPID_PROPERTYPUT;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&arg);
//     arg.vt = VT_I2;
//     arg.iVal = nNewValue;
//     dp.rgvarg = &arg;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = &dispidPut;
//     dp.cNamedArgs = 1;

//     m_pDispatch->Invoke(0x68030025, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

//     VariantClear(&arg);
// }

// BOOL Core::GetIsConnected() {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030026, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     } else {
//         result = FALSE;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// CString Core::GetDebugMessage() {
//     if (!m_pDispatch) {
//         return _T("");
//     }

//     CString result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030029, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
//         result = vResult.bstrVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// CString Core::GetDebugMessages() {
//     if (!m_pDispatch) {
//         return _T("");
//     }

//     CString result;
//     DISPPARAMS dp = {};
//     // 无参数
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030030, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
//         result = vResult.bstrVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// long Core::GetConnectState() {
//     if (!m_pDispatch) {
//         return 0;
//     }

//     long result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030031, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_I4) {
//         result = vResult.lVal;
//     } else {
//         result = 0;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// CString Core::GetTypeName() {
//     if (!m_pDispatch) {
//         return _T("");
//     }

//     CString result;
//     DISPPARAMS dp = {};
//     // 无参数
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x60030036, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BSTR) {
//         result = vResult.bstrVal;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// void Core::AddUserMessage(LPCTSTR vstrMessage) {
//     if (!m_pDispatch) {
//         return;
//     }

//     DISPPARAMS dp = {};
//     VARIANT args[1];
//     VariantInit(&args[0]);

//     // 参数按照倒序传递
//     args[0].vt = VT_BSTR;
//     args[0].bstrVal = SysAllocString(reinterpret_cast<LPCOLESTR>(vstrMessage));

//     dp.rgvarg = args;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     m_pDispatch->Invoke(0x60030037, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, NULL, &excepInfo, &argerr);

//     VariantClear(&args[0]);
// }

// BOOL Core::GetDebugLog() {
//     if (!m_pDispatch) {
//         return FALSE;
//     }

//     BOOL result;
//     DISPPARAMS dp = {};
//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x68030038, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_BOOL) {
//         result = vResult.boolVal;
//     } else {
//         result = FALSE;
//     }

//     VariantClear(&vResult);
//     return result;
// }

// void Core::SetDebugLog(BOOL bNewValue) {
//     if (!m_pDispatch) {
//         return;
//     }

//     DISPPARAMS dp = {};
//     VARIANT arg;
//     DISPID dispidPut = DISPID_PROPERTYPUT;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&arg);
//     arg.vt = VT_BOOL;
//     arg.boolVal = bNewValue;
//     dp.rgvarg = &arg;
//     dp.cArgs = 1;
//     dp.rgdispidNamedArgs = &dispidPut;
//     dp.cNamedArgs = 1;

//     m_pDispatch->Invoke(0x68030038, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, &excepInfo, &argerr);

//     VariantClear(&arg);
// }

// LPDISPATCH Core::GetLib() {
//     if (!m_pDispatch) {
//         return NULL;
//     }

//     LPDISPATCH result = NULL;
//     DISPPARAMS dp = {};
//     // 无参数
//     dp.rgdispidNamedArgs = NULL;
//     dp.cNamedArgs = 0;

//     VARIANT vResult;
//     EXCEPINFO excepInfo = {};
//     UINT argerr = 0;

//     VariantInit(&vResult);
//     HRESULT hr = m_pDispatch->Invoke(0x6003003d, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &vResult, &excepInfo, &argerr);

//     if (SUCCEEDED(hr) && vResult.vt == VT_DISPATCH) {
//         result = vResult.pdispVal;
//         result->AddRef();  // 增加引用计数
//     }

//     VariantClear(&vResult);
//     return result;
// }

} // namespace RobotComm
