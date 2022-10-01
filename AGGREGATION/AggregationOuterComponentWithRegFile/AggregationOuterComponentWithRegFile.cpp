#include<windows.h>

#include "AggregationInnerComponentWithRegFile.h"
#include "AggregationOuterComponentWithRegFile.h"

class CSumSubtract : public ISum , ISubtract
{
    private:
        long m_cRef;
        IUnknown* pIUnknownInner;
        IMultiplication *mPIMultiplication;
        IDivision *mPIDivision;
    
    public:
        CSumSubtract();
        ~CSumSubtract();

        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        HRESULT __stdcall SumofTwoIntegers(int, int, int*);

        HRESULT __stdcall SubtractionOfTwoIntegers(int, int ,int*);

        HRESULT __stdcall InitializeInnerComponent(void);
};

class CSumSubtractClassFactory : public IClassFactory
{
    private:
        long m_cRef;

    public:
        CSumSubtractClassFactory(void);

        ~CSumSubtractClassFactory(void);

        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        HRESULT __stdcall CreateInstance(IUnknown*,REFIID,void**);
        HRESULT __stdcall LockServer(BOOL);      
};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

BOOL WINAPI DllMain(HINSTANCE dInstance, DWORD dwReason, LPVOID Reserved)
{
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        
    }

    return(true);
}


CSumSubtract::CSumSubtract()
{
    pIUnknownInner = NULL;
    mPIMultiplication = NULL;
    mPIDivision = NULL;
    m_cRef = 1;

    InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract()
{
    InterlockedDecrement(&glNumberOfActiveComponents);

    if(pIUnknownInner)
    {
        pIUnknownInner->Release();
        pIUnknownInner = NULL;
    }

    if(mPIDivision)
    {
        mPIDivision->Release();
        mPIDivision = NULL;
    }

    if(mPIMultiplication)
    {
        mPIMultiplication->Release();
        mPIMultiplication = NULL;
    }
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
    if(riid == IID_IUnknown)
        *ppv = static_cast<ISum*>(this);
    else if(riid == IID_ISum)
        *ppv = static_cast<ISum*>(this);
    else if(riid == IID_ISubstract)
        *ppv = static_cast<ISubtract*>(this);
    else if(riid == IID_IMultiplication)
        pIUnknownInner->QueryInterface(riid,ppv);
    else if(riid == IID_IDivision)
        pIUnknownInner->QueryInterface(riid,ppv);
    else{
        *ppv = NULL;
        return(E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return(S_OK);
}

ULONG CSumSubtract::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG CSumSubtract::Release()
{
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete(this);
        return(0);
    }

    return(m_cRef);
}


HRESULT CSumSubtract::SumofTwoIntegers(int n1, int n2, int* sum)
{
    *sum = n1 + n2;
    return(S_OK);
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int n1, int n2 ,int* sub)
{
    *sub = n1 - n2;
    return(S_OK);
}

HRESULT CSumSubtract::InitializeInnerComponent(void)
{
    HRESULT hr;

    hr = CoCreateInstance(CLSID_MultiplicationDivision,
                        reinterpret_cast<IUnknown*>(this),
                        CLSCTX_INPROC_SERVER,
                        IID_IUnknown,
                        (void**)&pIUnknownInner );

    if(FAILED(hr))
    {
        MessageBox(NULL,TEXT("Unable to fetch IUnknown Interface from Inner component"),TEXT("ERROR"),MB_OK);
        return(E_FAIL);
    }

    hr = pIUnknownInner->QueryInterface(IID_IMultiplication,(void**)&mPIMultiplication);

    if(FAILED(hr))
    {
        MessageBox(NULL,TEXT("IMultiplication Interface cannot be Obtained from Inner Component"),TEXT("ERROR"),MB_OK );
        pIUnknownInner->Release();
        pIUnknownInner = NULL;
        return(E_FAIL);

    }

    hr = pIUnknownInner->QueryInterface(IID_IDivision,(void**)&mPIDivision);
    if(FAILED(hr))
    {
        MessageBox(NULL,TEXT("IDivision Interface cannot be Obtained from Inner Component"),TEXT("ERROR"),MB_OK );
        pIUnknownInner->Release();
        pIUnknownInner = NULL;
        return(E_FAIL);
    }

    return(S_OK);
}

//CLASSFACTORY implementation
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
    m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{

}

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    if(riid == IID_IUnknown)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else if(riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else
    {
        *ppv = NULL;
        return(E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv) ->AddRef();
    return(S_OK);
}

ULONG CSumSubtractClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG CSumSubtractClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete(this);
        return 0;
    }
    return(m_cRef);
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown* pUnkOuter,REFIID riid,void** ppv)
{
    CSumSubtract* pCSumSubtract = NULL;
    HRESULT hr;
    
    if(pUnkOuter != NULL)
        MessageBox(NULL,TEXT("HERE"),TEXT("ERROR"),MB_OK);
        return(CLASS_E_NOAGGREGATION);
    pCSumSubtract = new CSumSubtract;
    if(pCSumSubtract == NULL)
        return(E_OUTOFMEMORY);
    hr = pCSumSubtract->InitializeInnerComponent();
    if(FAILED(hr))
    {
        MessageBox(NULL,TEXT("Unable to initialize inner component"),TEXT("ERROR"),MB_OK);
        pCSumSubtract->Release();
        return(hr);
    }
    hr = pCSumSubtract->QueryInterface(riid,ppv);
    pCSumSubtract->Release();
    return(hr);


}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
    if(fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return(S_OK);
}


HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    CSumSubtractClassFactory* pCSumSubtractClassFactory = NULL;
    HRESULT hr;

    if(rclsid != CLSID_SumSubstract)
    {
        return(CLASS_E_CLASSNOTAVAILABLE);
    }
    pCSumSubtractClassFactory = new CSumSubtractClassFactory();

    if(pCSumSubtractClassFactory == NULL)
        return(E_OUTOFMEMORY);
    
    hr =pCSumSubtractClassFactory->QueryInterface(riid,ppv);
    pCSumSubtractClassFactory->Release();
    return(hr);
}


HRESULT __stdcall DllCanUnloadNow(void)
{
    if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return(S_OK);
    else
        return(S_FALSE);
}

//link.exe AggregationOuterComponentWithRegFile.obj /DLL /DEF:AggregationOuterComponentWithRegFile.def User32.lib GDI32.lib OLE32.lib OLEAUT32.lib /SUBSYSTEM:WINDOWS
