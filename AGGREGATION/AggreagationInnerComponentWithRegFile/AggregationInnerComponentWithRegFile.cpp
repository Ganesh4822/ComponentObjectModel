#include<windows.h>
#include "AggregationInnerComponentWithRegFile.h"

interface INoAggregationIUnknown
{
    virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) = 0;
    virtual ULONG __stdcall Addref_NoAggregation() = 0;
    virtual ULONG __stdcall Release_NoAggreagation() = 0;
};

class CMultiplicationDivision : public INoAggregationIUnknown,IMultiplication , IDivision
{
    private:
        long m_cRef;
        IUnknown *m_pIUnkownOuter;

    public:
        CMultiplicationDivision(IUnknown*);
        ~CMultiplicationDivision();

        //INoAgrregationMethods
        HRESULT __stdcall QueryInterface_NoAggregation(REFIID,void**);
        ULONG __stdcall Addref_NoAggregation();
        ULONG __stdcall Release_NoAggreagation();

        //IUnkowns methods
        HRESULT __stdcall QueryInterface(REFIID,void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        //CMultiplicationDivision Methods

        HRESULT __stdcall MultiplicationOfTwoIntegers(int,int,int*);

        HRESULT __stdcall DivisionOfTwoIntegers(int,int,int*);
};


//classFactory class Declaration
class CMultiplicationDivisionClassFactory : public IClassFactory
{
    private:
        long m_cRef;
    
    public:
        CMultiplicationDivisionClassFactory();
        ~CMultiplicationDivisionClassFactory();

        HRESULT __stdcall QueryInterface(REFIID,void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        HRESULT __stdcall CreateInstance(IUnknown*,REFIID,void**);
        HRESULT __stdcall LockServer(BOOL);

};

//globalVariable

long glNumberofActiveComponents = 0;
long glNumberOfServerLocks = 0;

//WinMain

BOOL WINAPI DllMain(HINSTANCE dDll, DWORD dwReason, LPVOID Reserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        /* code */
        break;
    case DLL_PROCESS_DETACH:
        break;
    
    }

    return TRUE;
}


//Implementation of CMultiplicationDivision methods

//contructor
 CMultiplicationDivision::CMultiplicationDivision(IUnknown* pIunknownOuter)
 {
    m_cRef = 1;
    InterlockedIncrement(&glNumberofActiveComponents);

    if(pIunknownOuter != NULL)
        m_pIUnkownOuter = pIunknownOuter;
    else
        reinterpret_cast<IUnknown*>
        (static_cast<INoAggregationIUnknown*>(this));

 }

CMultiplicationDivision::~CMultiplicationDivision()
{
    InterlockedDecrement(&glNumberofActiveComponents);
}

       //INoAgrregationMethods
HRESULT  CMultiplicationDivision::QueryInterface_NoAggregation(REFIID riid,void** ppv)
{
    if(riid == IID_IUnknown)
        *ppv = static_cast<INoAggregationIUnknown*>(this);
    else if(riid == IID_IMultiplication)
        *ppv = static_cast<IMultiplication*>(this);
    else if(riid == IID_IDivision)
        *ppv = static_cast<IDivision*>(this);
    
    else{
        *ppv = NULL;
        return(E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return(S_OK);
}

ULONG  CMultiplicationDivision::Addref_NoAggregation()
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG  CMultiplicationDivision::Release_NoAggreagation()
{
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete(this);
        return 0;
    }
    return(m_cRef);
}

        //IUnkowns methods
HRESULT  CMultiplicationDivision::QueryInterface(REFIID riid,void** ppv)
{
    return(m_pIUnkownOuter->QueryInterface(riid,ppv));

}

ULONG  CMultiplicationDivision::AddRef()
{
    return(m_pIUnkownOuter->AddRef());
}

ULONG  CMultiplicationDivision::Release()
{
    return(m_pIUnkownOuter->Release());
}

        //CMultiplicationDivision Methods

HRESULT  CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1,int num2,int* mul)
{
    *mul = num1 *num2;
    return(S_OK);
}

HRESULT  CMultiplicationDivision::DivisionOfTwoIntegers(int n1,int n2,int* div)
{
    *div = n1 / n2;
    return(S_OK);
}


//CMultiplcatioDivisionClassFactory Implementation
CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory()
{
    m_cRef = 1;

}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory()
{

}

HRESULT  CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid,void** ppv)
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

ULONG  CMultiplicationDivisionClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG  CMultiplicationDivisionClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete(this);
        return(0);
    }
    return(m_cRef);
}

HRESULT  CMultiplicationDivisionClassFactory::CreateInstance(IUnknown *pUnkOuter,REFIID riid,void** ppv)
{
    CMultiplicationDivision* pCMultiplicatioDivision = NULL;
    HRESULT hr;

    if((pUnkOuter != NULL) && (riid != IID_IUnknown))
        return (CLASS_E_NOAGGREGATION);
    pCMultiplicatioDivision = new CMultiplicationDivision(pUnkOuter);
    if(pCMultiplicatioDivision == NULL)
        return(E_OUTOFMEMORY);
    hr = pCMultiplicatioDivision->QueryInterface_NoAggregation(riid,ppv);
    pCMultiplicatioDivision ->Release_NoAggreagation();

    return(S_OK);
}

HRESULT  CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
    if(fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return(S_OK);
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    CMultiplicationDivisionClassFactory *pCMultiplicationDivisionClassFactory = NULL;
    HRESULT hr;

    if(rclsid != CLSID_MultiplicationDivision)
        return(CLASS_E_CLASSNOTAVAILABLE);
    
    pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory();
    if(pCMultiplicationDivisionClassFactory == NULL)
        return(E_OUTOFMEMORY);
    hr = pCMultiplicationDivisionClassFactory->QueryInterface(riid,ppv);
    pCMultiplicationDivisionClassFactory->Release();

    return(S_OK);
}

HRESULT __stdcall DllCanUnloadNow(void)
{
    if((glNumberofActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return(S_OK);
    else
        return(S_FALSE);
}

//link.exe AggregationInnerComponentWithRegFile.obj /DLL /DEF:AggregationInnerComponentWithRegFile.def User32.lib GDI32.lib OLE32.lib OLEAUT32.lib /SUBSYSTEM:WINDOWS
