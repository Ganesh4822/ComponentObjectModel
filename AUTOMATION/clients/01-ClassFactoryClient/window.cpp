#include<Windows.h>

#include "AutomationServer.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//global variable
IMyMath *pIMymath = NULL;
//ISubtract *pISubtract = NULL;

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
    wndclass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

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


LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
    void SafeInterfaceRealease();

    HRESULT hr;

    int num1,num2,sum,sub;

    TCHAR str[256];

    switch(imsg)
    {
        case WM_CREATE :
            hr = CoCreateInstance(CLSID_IMyMath,NULL,CLSCTX_INPROC_SERVER,IID_IMyMath,(void**)&pIMymath);
            
            wsprintf(str,TEXT("hr is %p"),hr);

            MessageBox(hwnd,str,TEXT("Error"),MB_OK);
            if(FAILED(hr))
            {
                MessageBox(hwnd,TEXT("IMYMath cannot be obtained"),TEXT("Error"),MB_OK);
                DestroyWindow(hwnd);

            }

            num1 = 55;
            num2 = 100;

            pIMymath->SumOfTwoIntegers(num1,num2,&sum);

            wsprintf(str,TEXT("sum of %d amd %d is %d"),num1,num2,sum);
            MessageBox(hwnd,str,TEXT("RESULT"),MB_OK);

            pIMymath->SubstractionOfTwoIntegers(num1,num2,&sub);

            // hr = pISum->QueryInterface(IID_ISubtract,(void**)pISubtract);
            // if(FAILED(hr))
            // {
            //     MessageBox(hwnd,TEXT("Isubtract cannot be obtained"),TEXT("Error"),MB_OK);
            //     DestroyWindow(hwnd);

            // }

            // pISum->Release();
            // pISum = NULL;

            // num1 =100;
            // num2 = 90;

            // pISubtract ->substractionOfTwoIntegers(num1,num2,&isub);

            wsprintf(str,TEXT("sub of %d amd %d is %d"),num1,num2,sub);
            MessageBox(hwnd,str,TEXT("SUBRESULT"),MB_OK);
            pIMymath->Release();
            pIMymath = NULL;

            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
        break;

    }
    return(DefWindowProc(hwnd,imsg,wParam,lParam));
}

//link.exe Window.obj User32.lib GDI32.lib OLE32.lib OLEAUT32.lib /SUBSYSTEM:WINDOWS