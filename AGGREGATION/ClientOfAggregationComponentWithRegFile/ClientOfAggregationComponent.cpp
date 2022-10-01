#include<Windows.h>

#include "HeaderForClientAggregationWithRegFile.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//global variable
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;
IMultiplication* pIMultiplication = NULL;
IDivision *pIDivision  = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("ComClient");
    HRESULT hr;

    hr  = CoInitialize(NULL);

    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("Com Library initialization Failed"),TEXT("Program Error"),MB_OK);
        exit(0);
    }
    
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndclass.hInstance = hInstance;
    wndclass.lpszClassName = AppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(AppName,TEXT("client of com server"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);

    ShowWindow (hwnd,nCmdShow);

    UpdateWindow(hwnd);

    while(GetMessage(&msg,NULL,0,0))
    {
        //translate masage
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return((int) msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    void SafeInterfaceRelease(void);

    HRESULT hr;
    int num1,num2,iSum,iSubtract,iMul,iDiv;
    TCHAR str[256];

    switch(iMsg)
    {
        case WM_CREATE:
            hr = CoCreateInstance(CLSID_SumSubstract,NULL,CLSCTX_INPROC_SERVER,IID_ISum,(void**)&pISum);
            wsprintf(str,TEXT("hr is %p"),hr);

            MessageBox(hwnd,str,TEXT("Error"),MB_OK);
            if(FAILED(hr))
            {
                MessageBox(hwnd,TEXT("ISum cannot be Obtained"),TEXT("ERROR"),MB_OK);
                DestroyWindow(hwnd);
            }

            num1 = 77;
            num2 = 121;
            pISum->SumofTwoIntegers(num1,num2,&iSum);
            wsprintf(str,TEXT("sum of%d and %d is %d"),num1,num2,iSum);
            MessageBox(hwnd,str,TEXT("RESULT"),MB_OK);

            hr = pISum->QueryInterface(IID_ISubstract,(void**)&pISubtract);
            if(FAILED(hr))
            {
                MessageBox(hwnd,TEXT("ISubsract cannot be Obtained"),TEXT("ERROR"),MB_OK);
                DestroyWindow(hwnd);
            }

            pISum->Release();
            pISum = NULL;

            pISubtract->SubtractionOfTwoIntegers(num2,num1,&iSubtract);
            wsprintf(str,TEXT("substraction of%d and %d is %d"),num1,num2,iSubtract);
            MessageBox(hwnd,str,TEXT("RESULT"),MB_OK);

            hr = pISubtract->QueryInterface(IID_IMultiplication,(void**)&pIMultiplication);
            
            if(FAILED(hr))
            {
                MessageBox(hwnd,TEXT("IMultiplication cannot be Obtained"),TEXT("ERROR"),MB_OK);
                DestroyWindow(hwnd);
            }

            pISubtract->Release();
            pISubtract = NULL;

            hr = pIMultiplication->MultiplicationOfTwoIntegers(num1,num2,&iMul);
            wsprintf(str,TEXT("substraction of%d and %d is %d"),num1,num2,iMul);
            MessageBox(hwnd,str,TEXT("RESULT"),MB_OK);

            hr = pIMultiplication->QueryInterface(IID_IDivision,(void**)&pIDivision);
            if(FAILED(hr))
            {
                MessageBox(hwnd,TEXT("IDivision cannot be Obtained"),TEXT("ERROR"),MB_OK);
                DestroyWindow(hwnd);
            }

            pIMultiplication->Release();
            pIMultiplication = NULL;

            num1 = 100;
            num2 = 5;

            pIDivision->DivisionOfTwoIntegers(num1,num2,&iDiv);
            wsprintf(str,TEXT("substraction of%d and %d is %d"),num1,num2,iDiv);
            MessageBox(hwnd,str,TEXT("RESULT"),MB_OK);

            pIDivision->Release();
            pIDivision = NULL;


            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            SafeInterfaceRelease();
            PostQuitMessage(0);
            break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void SafeInterfaceRelease()
{
    if(pISum)
    {
        pISum->Release();
        pISum = NULL;
    }
    if(pISubtract)
    {
        pISubtract->Release();
        pISubtract=NULL;
    }

    if(pIMultiplication )
    {
        pIMultiplication->Release();
        pIMultiplication = NULL;
    }

    if(pIDivision )
    {
        pIDivision->Release();
        pIDivision = NULL;

    }
}

//link.exe ClientOfAggregationComponent.obj OLE32.lib OLEAUT32.lib User32.lib GDI32.lib /SUBSYSTEM:WINDOWS