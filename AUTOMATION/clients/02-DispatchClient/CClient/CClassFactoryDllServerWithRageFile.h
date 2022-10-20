#pragma once

#undef INTERFACE
#define INTERFACE IMyMath
DECLARE_INTERFACE_(IMyMath, IDispatch) {
	//Iunknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID, void**) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	//ISum methods
	STDMETHOD(SumOfTwoIntegers)(THIS_ int, int, int*) PURE;
	STDMETHOD(SubstractionOfTwoIntegers)(THIS_ int, int, int*) PURE;

	STDMETHOD(GetTypeInfoCount)(THIS_ UINT*) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo**) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID,LPOLESTR*,UINT,LCID,DISPID*) PURE;
    STDMETHOD(Invoke)(THIS_ DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*)PURE;

	STDMETHOD(InitInstance)(THIS)PURE;
};

//"{0xbb7d9fb3,0x3751,0x4d53,0xb7,0x3a,0xe1,0x34,0x2f,0x2b,0x89,0x6e}"
const CLSID CLSID_IMyMath =
 {0x1a0d64dd, 0xac61, 0x4c9d, 0x8a, 0xb7, 0x18, 0xc0, 0x68, 0xf5, 0x41, 0xae};

//"{522f2e94-aad0-4477-90b4-2881ee3a5fd5}"
const IID IID_IMyMath = 
{0x512a46e1, 0x5c89, 0x4a76, 0xb1, 0xcd, 0x91, 0x99, 0x54, 0x58, 0xd, 0x9f};


