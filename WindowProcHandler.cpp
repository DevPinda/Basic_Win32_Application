#define UNICODE
#include "WindowProcHandler.h"
#include <iostream>
#include <Windows.h>
#include "ButtonManager.h"
#include "PointerManager.h"
#include "RandomColorGenerator.h"
#include <dwmapi.h>
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#include <vector>
#pragma comment(lib, "dwmapi.lib")

boolean isOpen = false;
auto randomNumberGenerator = new RandomColorGenerator();
ButtonManager* buttonManager;
ButtonManager* buttonManager2;
COLORREF bgColor = (20, 20, 20);
COLORREF textColor, borderColor;
int newWidth, newHeight, updWinWidth, updWinHeight, wmId, centerX, centerY, computeButtonX, computeButtonY, commandId;

// Child window process function
LRESULT WindowProcHandler::ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Switch statement to manage message codes for the child window
    switch (uMsg) {
        // Window message to create elements
        case WM_CREATE: {
            HWND hLabel = CreateWindowW(L"STATIC", L"Button clicked", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, nullptr, nullptr, nullptr);

            HFONT hFont = CreateFontW(32, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                      DEFAULT_PITCH | FF_DONTCARE, L"Futura");

            HWND hButton = CreateWindowW(L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                         0, 0, 75, 30, hwnd, (HMENU)1001, nullptr, nullptr);

            if (hFont) {
                SendMessage(hLabel, WM_SETFONT, (WPARAM) hFont, TRUE);
            }

            LONG_PTR buttonStyle = GetWindowLongPtr(hButton, GWL_STYLE);
            buttonStyle &= ~(BS_TYPEMASK);
            buttonStyle |= BS_OWNERDRAW;
            SetWindowLongPtr(hButton, GWL_STYLE, buttonStyle);

            break;
        }
        // Window message that deals with size events
        case WM_SIZE: {
            HWND hLabel = GetDlgItem(hwnd, 0);
            HWND hButton = GetDlgItem(hwnd, 1001);

            newWidth = LOWORD(lParam);
            newHeight = HIWORD(lParam);

            const wchar_t* labelText = L"Button clicked";
            SIZE textSize;
            HDC hdc = GetDC(hLabel);
            auto hFont = (HFONT)SendMessage(hLabel, WM_GETFONT, 0, 0);
            auto hOldFont = (HFONT)SelectObject(hdc, hFont);
            GetTextExtentPoint32W(hdc, labelText, wcslen(labelText), &textSize);
            SelectObject(hdc, hOldFont);
            ReleaseDC(hLabel, hdc);

            centerX = (newWidth - textSize.cx) / 2;
            centerY = (newHeight - textSize.cy) / 2;

            computeButtonX = (newWidth - 100) / 2;
            computeButtonY = (newHeight + textSize.cy) / 2;

            SetWindowPos(hLabel, nullptr, centerX, centerY - 37, textSize.cx, textSize.cy, SWP_NOZORDER);
            SetWindowPos(hButton, nullptr, computeButtonX, computeButtonY, 100, 45, SWP_NOZORDER);

            break;
        }
        // Window message to draw items
        case WM_DRAWITEM: {
            auto lpDrawItemStruct = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
            if (lpDrawItemStruct->CtlID == 1001) {

                HDC hdc = lpDrawItemStruct->hDC;
                RECT rc = lpDrawItemStruct->rcItem;
                UINT state = lpDrawItemStruct->itemState;
                const wchar_t* buttonText = L"OK";

                if (state & ODS_SELECTED) {
                    bgColor = RGB(200, 200, 200);
                    textColor = RGB(0, 0, 0);
                    borderColor = RGB(128, 128, 128);
                } else {
                    bgColor = RGB(45, 45, 45);
                    textColor = RGB(255, 255, 255);
                    borderColor = RGB(128, 128, 128);
                }

                HBRUSH hBrush = CreateSolidBrush(bgColor);
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);

                HBRUSH hBorderBrush = CreateSolidBrush(borderColor);
                FrameRect(hdc, &rc, hBorderBrush);
                DeleteObject(hBorderBrush);

                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, textColor);
                DrawText(hdc, buttonText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                return TRUE;
            }
            break;
        }
        // Window message to control colour static
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetBkColor(hdcStatic, RGB(30, 30, 30));
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        // Message code to trigger repaint to avoid graphical glitches when resizing window
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            break;
        }
        // Window messages in the context of commands
        case WM_COMMAND: {
            wmId = LOWORD(wParam);
            if (wmId == 1001) {
                DestroyWindow(hwnd);
            }
            break;
        }
        // Window message ran when window undergoes destruction
        case WM_DESTROY: {
            isOpen = false;
            DestroyWindow(hwnd);
            UnregisterClass(L"ChildWindowClass", nullptr);
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Main window process
LRESULT WindowProcHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    // Gets pointers for ButtonManager instances and set them to local variables
    std::vector<ButtonManager*> pointerArr = PointerManager::GetButtonPointers();

    if (!pointerArr.empty()) {
        buttonManager = pointerArr.at(0);
        buttonManager2 = pointerArr.at(1);
        pointerArr.clear();
    }

    // Switch statement to manage message codes for the child window
    switch (uMsg) {
        // Window message that deals with size events
        case WM_SIZE: {
            updWinWidth = LOWORD(lParam);
            updWinHeight = HIWORD(lParam);

            buttonManager->ComputeResize(updWinWidth, updWinHeight);
            buttonManager2->ComputeResize(updWinWidth, updWinHeight);

            newWidth = buttonManager->GetWidth();
            newHeight = buttonManager->GetHeight();

            buttonManager->SetSizeAndPosition(((updWinWidth - newWidth)/2)-(newWidth), (updWinHeight - newHeight) / 2,
                                              newWidth, newHeight);

            buttonManager2->SetSizeAndPosition(((updWinWidth - newWidth)/2)+(newWidth), (updWinHeight - newHeight) / 2,
                                              newWidth, newHeight);

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }
        // Window messages in the context of commands
        case WM_COMMAND: {
            commandId = LOWORD(wParam);
            if (commandId == 1 && isOpen == false) {
                    isOpen = true;
                    const wchar_t CHILD_CLASS_NAME[] = L"ChildWindowClass";
                    WNDCLASS childWndClass = {};
                    childWndClass.lpfnWndProc = WindowProcHandler::ChildWindowProc;
                    childWndClass.hInstance = GetModuleHandle(nullptr);
                    childWndClass.lpszClassName = CHILD_CLASS_NAME;
                    RegisterClass(&childWndClass);

                    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                    int childWidth = screenWidth / 5;
                    int childHeight = screenHeight / 6;
                    int xPos = (screenWidth - childWidth) / 2;
                    int yPos = (screenHeight - childHeight) / 2;

                    HWND hChildWnd = CreateWindowEx(0, CHILD_CLASS_NAME, L"Button Clicked", WS_OVERLAPPEDWINDOW,
                        xPos, yPos, childWidth, childHeight, hwnd, nullptr,
                        GetModuleHandle(nullptr), nullptr);

                    DWORD attributeValue = 1;
                    DwmSetWindowAttribute(hChildWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &attributeValue,
                        sizeof(attributeValue));

                    ShowWindow(hChildWnd, SW_SHOW);
                    UpdateWindow(hChildWnd);
                }
                else if (commandId == 2) {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    bgColor = randomNumberGenerator->GetRndBg();
                    InvalidateRect(hwnd, nullptr, TRUE);
                    HBRUSH hBrush = CreateSolidBrush(bgColor);
                    FillRect(hdc, &ps.rcPaint, hBrush);
                    DeleteObject(hBrush);

                    EndPaint(hwnd, &ps);
                }
            break;
        }
        // Window message to draw items
        case WM_DRAWITEM: {
            auto lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
            if (lpDrawItemStruct->CtlID == 1) {
                HDC hdc = lpDrawItemStruct->hDC;
                RECT rc = lpDrawItemStruct->rcItem;
                // UINT state = lpDrawItemStruct->itemState;
                const auto buttonText = L"Click Here";

                HBRUSH hBrush = CreateSolidBrush(buttonManager->GetBgColor());
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);

                HBRUSH hBorderBrush = CreateSolidBrush(buttonManager->GetBorderColor());
                FrameRect(hdc, &rc, hBorderBrush);
                DeleteObject(hBorderBrush);

                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, buttonManager->GetTextColor());
                DrawText(hdc, buttonText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                return TRUE;
            } else if (lpDrawItemStruct->CtlID == 2) {
                HDC hdc = lpDrawItemStruct->hDC;
                RECT rc = lpDrawItemStruct->rcItem;
                // UINT state = lpDrawItemStruct->itemState;
                const auto buttonText2 = L"Random Colour";

                HBRUSH hBrush = CreateSolidBrush(buttonManager2->GetBgColor());
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);

                HBRUSH hBorderBrush = CreateSolidBrush(buttonManager2->GetBorderColor());
                FrameRect(hdc, &rc, hBorderBrush);
                DeleteObject(hBorderBrush);

                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, buttonManager2->GetTextColor());
                DrawText(hdc, buttonText2, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                return TRUE;
            }
            break;
        }
        // Message code to trigger repaint to avoid graphical glitches when resizing window
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            break;
        }
        // Message code that represents when the window is tried to be closed
        case WM_CLOSE: {
            int result = MessageBoxW(hwnd, L"Do you want to close the window?", L"Confirmation",
                                     MB_YESNO | MB_ICONQUESTION);

            if (result == IDYES) {
                DestroyWindow(hwnd);
            }
            return 0;
        }
        // Window message ran when window undergoes destruction
        case WM_DESTROY: {
            if (randomNumberGenerator != nullptr) {
                delete randomNumberGenerator;
            }
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}