#include "RobotCommCOM.h"
#include "../RobotComm/RobotComm.h"

// Global module count
long g_cDllRef = 0;

// COM object implementation
class RobotCommObject : public IRobotComm {
    ULONG m_refCount;

public:
    RobotCommObject() : m_refCount(1) {
        InterlockedIncrement(&g_cDllRef);
    }

    ~RobotCommObject() {
        InterlockedDecrement(&g_cDllRef);
    }

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (riid == IID_IUnknown || riid == IID_IDispatch || riid == __uuidof(IRobotComm)) {
            *ppv = static_cast<IRobotComm*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override {
        return InterlockedIncrement(&m_refCount);
    }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ulRefCount = InterlockedDecrement(&m_refCount);
        if (ulRefCount == 0) {
            delete this;
        }
        return ulRefCount;
    }

    // IDispatch
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo) override {
        *pctinfo = 0;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo**) override {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*) override {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*) override {
        return E_NOTIMPL;
    }

    // IRobotComm
    HRESULT STDMETHODCALLTYPE GetVersion(BSTR* version) override {
        *version = SysAllocString(L"RobotComm v2.0 (via COM)");
        return S_OK;
    }
};

// Class factory implementation
class RobotCommClassFactory : public IClassFactory {
    ULONG m_refCount;

public:
    RobotCommClassFactory() : m_refCount(1) {}

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (riid == IID_IUnknown || riid == IID_IClassFactory) {
            *ppv = static_cast<IClassFactory*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override {
        return InterlockedIncrement(&m_refCount);
    }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ulRefCount = InterlockedDecrement(&m_refCount);
        if (ulRefCount == 0) {
            delete this;
        }
        return ulRefCount;
    }

    // IClassFactory
    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) override {
        if (pUnkOuter != nullptr)
            return CLASS_E_NOAGGREGATION;

        RobotCommObject* pObject = new RobotCommObject();
        HRESULT hr = pObject->QueryInterface(riid, ppv);
        pObject->Release();
        return hr;
    }

    HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) override {
        if (fLock)
            InterlockedIncrement(&g_cDllRef);
        else
            InterlockedDecrement(&g_cDllRef);
        return S_OK;
    }
};

// DLL exported functions
STDAPI DllCanUnloadNow() {
    return (g_cDllRef == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    if (rclsid == CLSID_RobotComm) {
        RobotCommClassFactory* factory = new RobotCommClassFactory();
        HRESULT hr = factory->QueryInterface(riid, ppv);
        factory->Release();
        return hr;
    }
    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllRegisterServer() {
    return S_OK; // 实际可调用 regsvr32 自动注册
}

STDAPI DllUnregisterServer() {
    return S_OK;
}
