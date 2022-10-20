#include<windows.h>
#include<stdio.h>
#include "AutomationServer.h"

class CMyMath :public IMyMath
{
    private:
        long m_cRef;
        ITypeInfo *m_pITypeInfo = NULL;
    
    public:
        CMyMath();
        ~CMyMath();

        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        HRESULT __stdcall GetTypeInfoCount(UINT*);
        HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**);
        HRESULT __stdcall GetIDsOfNames(REFIID,LPOLESTR*,UINT,LCID,DISPID*);
        HRESULT __stdcall Invoke(DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*);

        HRESULT __stdcall SumOfTwoIntegers(int , int, int*);
        HRESULT __stdcall SubstractionOfTwoIntegers(int, int, int*);

        HRESULT InitInstance();

};


class CMyMathClassFactory : public IClassFactory
{
    private:
        long m_cRef;
    
    public:
        CMyMathClassFactory();

        ~CMyMathClassFactory();

        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void **);
        HRESULT __stdcall LockServer(BOOL);

};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

const GUID LIBID_AutomationServer = 
// {86CC2986-5196-4C5D-87C7-67082D92B5F6}
{0x86cc2986, 0x5196, 0x4c5d, 0x87, 0xc7, 0x67, 0x8, 0x2d, 0x92, 0xb5, 0xf6};

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        
    }
    return (true);
}

CMyMath::CMyMath()
{
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);

    m_pITypeInfo = NULL;
}

CMyMath::~CMyMath()
{
    InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CMyMath::QueryInterface(REFIID riid, void** ppv)
{
    if(riid == IID_IUnknown)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else if(riid == IID_IDispatch)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else if(riid == IID_IMyMath)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else{
        *ppv = NULL;
        return (E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return(S_OK);
}



ULONG CMyMath::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG CMyMath::Release()
{
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        m_pITypeInfo->Release();
        m_pITypeInfo = NULL;
        delete(this);
        return (0);
    }
    return (m_cRef);
}

HRESULT CMyMath::GetTypeInfoCount(UINT* pCountTypeInfo)
{
    *pCountTypeInfo = 1;

    return(S_OK);
}

HRESULT CMyMath::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)
{
    *ppITypeInfo = NULL;
    if(iTypeInfo != 0)
        return (DISP_E_BADINDEX);
    
    m_pITypeInfo->AddRef();
    *ppITypeInfo = m_pITypeInfo;
    return(S_OK);
}

HRESULT CMyMath::GetIDsOfNames(REFIID riid, LPOLESTR *rgsznames , UINT cNames , LCID lcid , DISPID *rgDispID )
{
    return(DispGetIDsOfNames(m_pITypeInfo,rgsznames,cNames,rgDispID));


}

HRESULT CMyMath::Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams ,VARIANT *pVarResult,EXCEPINFO *pExepInfo,UINT *puArgErr)
{
    HRESULT hr;

    hr = DispInvoke(
        this,
        m_pITypeInfo,
        dispIdMember,
        wFlags,
        pDispParams,
        pVarResult,
        pExepInfo,
        puArgErr
    );

    return(hr);
}

HRESULT CMyMath::SumOfTwoIntegers(int n1, int n2, int* pSum)
{
    *pSum = n1 + n2;
    return(S_OK);
}

HRESULT CMyMath::SubstractionOfTwoIntegers(int n1, int n2, int* pSub)
{
    *pSub = n1 - n2;
    return(S_OK);
}

HRESULT CMyMath::InitInstance()
{
    void ComErrorDescriptionString(HWND, HRESULT);

    HRESULT hr;
    ITypeLib* pITypeLib = NULL;

    if(m_pITypeInfo == NULL)
    {
        hr = LoadRegTypeLib(LIBID_AutomationServer, 1, 0, 0x00, &pITypeLib);

        if(FAILED(hr))
        {
            ComErrorDescriptionString(NULL,hr);
            return(hr);
        }

        hr = pITypeLib->GetTypeInfoOfGuid(IID_IMyMath,&m_pITypeInfo);

        if(FAILED(hr))
        {
            ComErrorDescriptionString(NULL,hr);
            pITypeLib->Release();
            return(hr);
        }

        pITypeLib->Release();

        
    }
    return(S_OK);
}

CMyMathClassFactory::CMyMathClassFactory()
{
    m_cRef = 1;

}

CMyMathClassFactory::~CMyMathClassFactory()
{

}

//Implementaion of IUnkowns methods
HRESULT CMyMathClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    if(riid == IID_IUnknown)
        *ppv = static_cast<IClassFactory*>(this);
    else if(riid == IID_IClassFactory)
        *ppv =static_cast<IClassFactory*>(this);
    else{
        *ppv = NULL;
        return (E_NOINTERFACE);
    }
    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return (S_OK);
}

ULONG CMyMathClassFactory::AddRef(void)
{
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CMyMathClassFactory::Release(void)
{
    InterlockedDecrement(&m_cRef);
    return(m_cRef);
}

HRESULT CMyMathClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    CMyMath* pMyMath = NULL;
    HRESULT hr;
    if(pUnkOuter != NULL)
        return(CLASS_E_NOAGGREGATION);
    MessageBox(NULL,TEXT("IN createInstance"),TEXT("AGGREGATION"),MB_OK);
    pMyMath = new CMyMath();
    if(pMyMath == NULL)
        return(E_OUTOFMEMORY);
    MessageBox(NULL,TEXT("IN createInstance2"),TEXT("AGGREGATION"),MB_OK);
    pMyMath->InitInstance();
    hr = pMyMath->QueryInterface(riid,(void**)ppv);
    pMyMath->Release();
    return(hr);

}

HRESULT CMyMathClassFactory::LockServer(BOOL flock)
{
    if(flock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return (S_OK);
}



extern "C" HRESULT __stdcall  DllGetClassObject(REFCLSID rclsid, REFIID riid, void**ppv)
{
    CMyMathClassFactory* pCMyMathClassFactory = NULL;
    HRESULT hr;

    if(rclsid != CLSID_IMyMath)
        return(CLASS_E_CLASSNOTAVAILABLE);
    
    pCMyMathClassFactory = new CMyMathClassFactory();
    if(pCMyMathClassFactory == NULL)
        return(E_OUTOFMEMORY);
    
    hr =pCMyMathClassFactory->QueryInterface(riid,ppv);
    pCMyMathClassFactory ->Release();
    return (hr);


}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
    if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return(S_OK);
    else
        return(S_FALSE);
}



void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
    TCHAR* szErrorMessage = NULL;
    TCHAR str[255];

    if(FACILITY_WINDOWS == HRESULT_FACILITY(hr))
    {
        hr = HRESULT_CODE(hr);

    }

    if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,hr,
    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&szErrorMessage,0,NULL) != 0)
    {
        swprintf_s(str,TEXT("%#x : %s"),hr,szErrorMessage);
        LocalFree(szErrorMessage);
    }
    else
    {
        swprintf_s(str,TEXT("Could not find error description\n %#x"),hr);
    }

    MessageBox(hwnd,str,TEXT("COM Error"),MB_OK);
}

//cl.exe /EHsc /c /D UNICODE AutomationServer.cpp
//link.exe AutomationServer.obj /DLL /DEF:AutomationServer.def User32.lib GDI32.lib OLE32.lib OLEAUT32.lib /SUBSYSTEM:WINDOWS

//midl.exe /h AutomationProxyStubHeader.h /iid AutomationProxiStubGuids.c /dlldata AutomationProxyStubDllData.c /proxy AutomationProxyStub.c AutomationServerTypeLib.idl

//cl.exe /c /EHsc /D REGISTER_PROXY_DLL AutomationProxyStub.c AutomationProxyStubDllData.c AutomationProxiStubGuids.c

//link.exe AutomationProxyStub.obj AutomationProxiStubGuids.obj AutomationProxyStubDllData.obj /DLL /DEF:AutomationProxyStub.defole32.lib oleaut32.lib advapi32.lib rpcns4.lib rpcrt4.lib /SUBSYSTEM:WINDOWS