//header files
#include<windows.h>

#include"CClassFactoryDllServerWithRageFile.h"

//global functions declarations
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//global variable declarations

//Entry point function
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    //local variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[]=TEXT("WINDEV");
    HRESULT hr;

    //code
    //COM Library Initialization
    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        MessageBox(NULL, TEXT("COM Initialization Failed"), TEXT("ERROR"), MB_OK);
        exit(0);
    }

    //wnd class
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW|CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance,IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

    //registraton of WNDCLASS
    RegisterClassEx (&wndclass);

    //creating the window
    hwnd = CreateWindow(szAppName,TEXT("VDM:Window"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);

    //SHOW THE WINDOW
    ShowWindow (hwnd,iCmdShow);

    //Update Wondow
    UpdateWindow(hwnd);

    //massage loop
    while(GetMessage(&msg,NULL,0,0))
    {
        //translate masage
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //COM UnInitialization
    CoUninitialize();
    return((int) msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    //variable Declaration
    IDispatch* pIDispatch = NULL;
    //it is also called as binary/basic String
    OLECHAR* szFunctionName1 = L"SumOfTwoIntegers";
    OLECHAR* szFunctionName2 = L"SubstractionOfTwoIntegers";
    VARIANT vArg[2];
    VARIANT vResult;
    DISPID dispid;
    DISPPARAMS params;
    HRESULT hr;
    int num1, num2, sum, subtract;
    TCHAR str[255];

    switch (iMsg)
    {
    case WM_CREATE:
        hr = CoCreateInstance(&CLSID_IMyMath, NULL, CLSCTX_INPROC_SERVER, &IID_IDispatch, (void**)&pIDispatch);
        if (FAILED(hr)) {
            MessageBox(hwnd, TEXT("ISUM Can not be obtained"), TEXT("ERROR"), MB_OK);
            DestroyWindow(hwnd);
        }
        num1 = 55;
        num2 = 45;
        //pISum->lpVtbl->sumOfTwoIntegers(pISum, n1, n2, &sum);
         hr = pIDispatch->lpVtbl->GetIDsOfNames(pIDispatch,IID_NULL,
            &szFunctionName1,1,GetUserDefaultLCID(),&dispid);

        if(FAILED(hr))
        {
            MessageBox(hwnd,TEXT("Get IDs of names failed to give ID of SumOfTwoIntegers "),TEXT("Error"),MB_OK);
            pIDispatch->lpVtbl->Release(pIDispatch);
            pIDispatch = NULL;
            DestroyWindow(hwnd);
        }

        VariantInit(vArg);
        vArg[0].vt = VT_INT;
        vArg[0].intVal = num2;
        vArg[1].vt = VT_INT;
        vArg[1].intVal = num1;
        VariantInit(&vResult);

        params.rgvarg = vArg;
        params.cArgs = 2;
        params.rgdispidNamedArgs = NULL;
        params.cNamedArgs = 0;
        
        pIDispatch->lpVtbl->Invoke(pIDispatch,
        dispid,
            IID_NULL,
            GetUserDefaultLCID(),
            DISPATCH_METHOD,
            &params,
            &vResult,
            NULL,
            NULL);

        sum = vResult.lVal;
        wsprintf(str, TEXT("SUM of %d and %d is %d"), num1, num2, sum);
        MessageBox(hwnd, str, TEXT("SUM"), MB_OK);

        //pISum->lpVtbl->QueryInterface(pISum, &IID_ISubtract, (void**)&pISubtract);
        
        //pISubtract->lpVtbl->substractionOfTwoIntegers(pISubtract, n1, n2, &subtract);
        wsprintf(str, TEXT("Subtraction of %d and %d is %d"), num1, num2, subtract);
        MessageBox(hwnd, str, TEXT("subtract"), MB_OK);

        //safe release
        // if (pISubtract) {
        //     pISubtract->lpVtbl->Release(pISubtract);
        //     pISubtract = NULL;
        // }
        // if (pISum) {
        //     pISum->lpVtbl->Release(pISum);
        //     pISum = NULL;
        // }
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        //PostQuitMessage(0);
        PostMessage(hwnd,WM_QUIT,0,0L);
        break;   
    default:
        break;
    }

    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}



//cl.exe /c /EHsc Window.c
//rc.exe Window.rc
//link.exe Window.obj User32.lib GDI32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS
