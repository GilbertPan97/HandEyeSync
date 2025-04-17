// 文件：RobotCommCOM.h
#pragma once

#include <windows.h>
#include <Unknwn.h> // IUnknown
#include "RobotComm_h.h" // 自动生成的头文件（来自IDL）

// 包装类头文件
#include "RobotComm.h"

class RobotCommObject : public IRobotComm
{
public:
    RobotCommObject();
    virtual ~RobotCommObject();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IRobotComm
    STDMETHODIMP GetVersion(BSTR* version) override;

private:
    long refCount_;
};

// 类工厂
class RobotCommClassFactory : public IClassFactory
{
public:
    RobotCommClassFactory();
    virtual ~RobotCommClassFactory();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IClassFactory
    STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    STDMETHODIMP LockServer(BOOL fLock) override;

private:
    long refCount_;
};

// 导出函数
STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);
