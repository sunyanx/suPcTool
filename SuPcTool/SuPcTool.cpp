﻿#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("suPcTool");
    //MessageBox(NULL, TEXT("作者正在努力制作哦，请稍等..."), TEXT("suPcTool"), MB_OK);
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("The exe need Windows NT !"), szAppName, MB_OK);
        return 0;
    }

    hwnd = CreateWindow(szAppName,
        TEXT("su电脑工具箱"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);


    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    static size_t cxClient, cyClient;
    static TCHAR szBufferDisplayResolution[128];//显示器分辨率
    static TCHAR welcome[] = TEXT("su电脑工具箱正在开发中...点击窗口任意位置即可禁用UAC...如果不成功请以管理员身份运行");
    static size_t LineInterval;
    switch (message)
    {

    case WM_CREATE:
        //创建窗口的时候获取字符信息
        hdc = GetDC(hwnd);
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        LineInterval = tm.tmHeight;
        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);

        cxClient = GetSystemMetrics(SM_CXSCREEN);
        cyClient = GetSystemMetrics(SM_CYSCREEN);

        wsprintf(szBufferDisplayResolution, TEXT("当前显示器分辨率是：%d*%d"), cxClient, cyClient);
        SetTextAlign(hdc, TA_CENTER);

        TextOut(hdc, rect.right / 2, rect.bottom / 2 - LineInterval, welcome, lstrlen(welcome));

        TextOut(hdc, rect.right / 2, rect.bottom / 2, szBufferDisplayResolution, lstrlen(szBufferDisplayResolution));

        EndPaint(hwnd, &ps);
        return 0;
    case WM_LBUTTONUP:
        MessageBox(hwnd, TEXT("确定禁用UAC？"), TEXT("确认"), MB_OK);
        system("reg add HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v EnableLUA /t REG_DWORD /d 0 /f");
        return 0;
    case WM_CLOSE:
        MessageBox(hwnd, TEXT("确认关闭suPcTool？"), TEXT("确认"), MB_OK);
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}