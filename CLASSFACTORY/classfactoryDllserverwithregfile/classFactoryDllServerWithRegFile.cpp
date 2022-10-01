#include<windows.h>
#include "classFactoryDllServerWithRegFile.h"

//class declaration

class CSumSubtract : public ISum,ISubtract
{
    private:
        long m_cRef;

    public:
        //contructor
        CSumSubtract(void);

        //destructor
        ~CSumSubtract(void);

        //IUnknown Specific methods
        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //ISum
        HRESULT __stdcall sumOfTwoIntegers(int, int, int*);

        //Isumtract
        HRESULT __stdcall substractionOfTwoIntegers(int, int, int*);        
};

class CSumSubtractClassFactory : public IClassFactory
{
    private:
        long m_cRef;
    
    public:
        CSumSubtractClassFactory();
        ~CSumSubtractClassFactory();

        //IUnknownSpecific methods

        HRESULT __stdcall QueryInterface(REFIID,void**);
        ULONG __stdcall AddRef();
        ULONG __stdcall Release();

        //ICLASSFACTORY methods

        HRESULT __stdcall CreateInstance(IUnknown*, REFIID,void**);
        HRESULT __stdcall LockServer(BOOL);

};

//global variable declaration 
long glNumberOfActiveComponents = 0; // Number of active componenets
long glNumberOfServerLocks = 0; // NUmber of locks on this server

//DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    MessageBox(NULL,TEXT("IN DLLMAIN"),TEXT("ClassFactory"),MB_OK);
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH : 
            MessageBox(NULL,TEXT("IN DLLMAIN"),TEXT("ClassFactory"),MB_OK);
            break;
        case DLL_PROCESS_DETACH :
            break;
        
    }

    return (true);
}  


//Implementation of CSumSubtract Methods
CSumSubtract::CSumSubtract(void)
{
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
    InterlockedDecrement(&glNumberOfActiveComponents);

}

//Implementation of CSUmSubtract's IUnknown's Methods
HRESULT CSumSubtract::QueryInterface(REFIID riid, void ** ppv)
{
    if(riid == IID_IUnknown)
        *ppv = static_cast<ISum*>(this);

    else if(riid == IID_ISum)
    {
        *ppv = static_cast<ISum*>(this);
        MessageBox(NULL,TEXT("IN QueryINterface Isum obtained"),TEXT("AGGREGATION"),MB_OK);
    }
        
    else if(riid == IID_ISubtract)
        *ppv  = static_cast<ISubtract*>(this);
    else
        {
            *ppv = NULL;
            return(E_NOINTERFACE);
        }
    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return(S_OK);
}

ULONG CSumSubtract::AddRef(void)
{
    InterlockedIncrement(&m_cRef);
    return(m_cRef);
}

ULONG CSumSubtract::Release(void)
{
    InterlockedDecrement(&m_cRef);
    return(m_cRef);
}

//ISum methods implemetation

HRESULT CSumSubtract::sumOfTwoIntegers(int num1 , int num2 , int* sum)
{
    *sum = num1+num2;
    return(S_OK);
}

//ISubtract Methods Implementation

HRESULT CSumSubtract::substractionOfTwoIntegers(int num1, int num2, int* sub)
{
    *sub = num1 - num2;
    return (S_OK);
}

//Implementation of Class Factory methods

CSumSubtractClassFactory::CSumSubtractClassFactory()
{
    m_cRef = 1;

}

CSumSubtractClassFactory::~CSumSubtractClassFactory()
{

}

//Implementaion of IUnkowns methods
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void** ppv)
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

ULONG CSumSubtractClassFactory::AddRef(void)
{
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CSumSubtractClassFactory::Release(void)
{
    InterlockedDecrement(&m_cRef);
    return(m_cRef);
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    CSumSubtract* pCSumSubstract = NULL;
    HRESULT hr;
    if(pUnkOuter != NULL)
        return(CLASS_E_NOAGGREGATION);
    MessageBox(NULL,TEXT("IN createInstance"),TEXT("AGGREGATION"),MB_OK);
    pCSumSubstract = new CSumSubtract();
    if(pCSumSubstract == NULL)
        return(E_OUTOFMEMORY);
    MessageBox(NULL,TEXT("IN createInstance2"),TEXT("AGGREGATION"),MB_OK);
    hr = pCSumSubstract->QueryInterface(riid,(void**)ppv);
    pCSumSubstract->Release();
    return(hr);

}

HRESULT CSumSubtractClassFactory::LockServer(BOOL flock)
{
    if(flock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return (S_OK);
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void**ppv)
{
    CSumSubtractClassFactory* pCSumSubtractClassFactory = NULL;
    HRESULT hr;

    if(rclsid != CLSID_SumSubtract)
        return(CLASS_E_CLASSNOTAVAILABLE);
    
    pCSumSubtractClassFactory = new CSumSubtractClassFactory();
    if(pCSumSubtractClassFactory == NULL)
        return(E_OUTOFMEMORY);
    
    hr =pCSumSubtractClassFactory->QueryInterface(riid,ppv);
    pCSumSubtractClassFactory ->Release();
    return (hr);


}

HRESULT __stdcall DllCanUnloadNow(void)
{
    if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return(S_OK);
    else
        return(S_FALSE);
}
//link.exe ClassFactoryDllServerWithRegFile.obj /DLL /DEF:ClassFactoryDllServerWithRegFile.def User32.lib GDI32.lib OLE32.lib OLEAUT32.lib /SUBSYSTEM:WINDOWS
