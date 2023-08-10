#define UNICODE
#include <Windows.h>
#include "Resource.h"
#include "ButtonManager.h"
#include <dwmapi.h>
#include <vector>
#include "WindowProcHandler.h"
#include "PointerManager.h"
#pragma comment(lib, "dwmapi.lib")
// DWMAPI's Dark Mode Config
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20


// WinMain IS the "entry Point Function" to Windows API   !!!not the int main()!!!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Define variables, important during window init
    const wchar_t CLASS_NAME[] = L"MyWindowClass";
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Registers the Window Class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcHandler::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Creates handle to the root window instance
    HWND hwnd = CreateWindowExW(
            0,
            CLASS_NAME,
            L"",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            screenWidth,
            screenHeight,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // Creates the two button instances
    ButtonManager buttonManager(hwnd, hInstance, 0, 0, 5, 7, L"Click Here", RGB(35, 35, 35), RGB(255, 255, 255),RGB(255, 255, 255), 32, L"Helvetica", (HMENU)1);
    ButtonManager buttonManager2(hwnd, hInstance, 0, 0, 5, 7, L"Random Color",  RGB(35, 35, 35), RGB(255, 255, 255),RGB(255, 255, 255), 32, L"Helvetica", (HMENU)2);

    // Creates and adds pointers to custom pointer storage implementation in PointerManager.cpp
    std::vector<ButtonManager *> pointerArray;
    pointerArray.push_back(&buttonManager);
    pointerArray.push_back(&buttonManager2);

    // Sends Pointers then clears the local variable from stack
    PointerManager::SetButtonPointers(pointerArray);
    pointerArray.clear();

    auto hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    hIcon = nullptr;

    // Sets Window to Dark Mode through DWM
    DWORD attributeValue = 1;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &attributeValue, sizeof(attributeValue));

    // Display & Update Window
    ShowWindow(hwnd, SW_MAXIMIZE);
    SetWindowText(hwnd, L"Basic C++ Win32 Application");
    UpdateWindow(hwnd);

    // Create Message Event Loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clears Memory upon Program Exit
    DestroyWindow(hwnd);
    UnregisterClass(CLASS_NAME, hInstance);

    return 0;
}