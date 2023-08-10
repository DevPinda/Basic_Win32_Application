#include "ButtonManager.h"
#include <vector>

/**
 * Constructor
 * @param parentHwnd :: Parent Handle of the Window :- Arg needed to obtain the
 * root window handle for the function from main
 * @param hInstance :: handle to the instance
 * @param x :: integer x axis value of button
 * @param y :: integer y axis value of button
 */
ButtonManager::ButtonManager(HWND parentHwnd, HINSTANCE hInstance, int x, int y, int widthDivisor, int heightDivisor,const WCHAR *buttonName, COLORREF bgColor,
                             COLORREF textColor, COLORREF borderColor, int fontSize, LPCWSTR fontFamily, HMENU__ *buttonId)
        : hButton(nullptr), buttonX(x), buttonY(y), widthDivisor(widthDivisor), heightDivisor(heightDivisor), bgColor(bgColor), textColor(textColor), borderColor(borderColor),
        buttonWidth(), buttonHeight(), fontSize(fontSize), wasResized(false) {
    // Create Button as an instance of WindowExW
    hButton = CreateWindowExW(
            0,
            L"BUTTON",
            buttonName,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            buttonX,
            buttonY,
            buttonWidth,
            buttonHeight,
            parentHwnd,
            (HMENU)buttonId,
            hInstance,
            nullptr
    );

    // Create HFONT object to store the font's information for the button
    HFONT hFont = CreateFontW(this->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              DEFAULT_PITCH | FF_DONTCARE, fontFamily);

    // Upon finishing initialisation, this new message is sent to the message loop
    SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
}

/**
 * Sets the coords and dimensions of button
 * @param x :- sets x position of the button
 * @param y :- sets y position of the button
 * @param width :- sets width of the button
 * @param height :- sets height of the button
 */
void ButtonManager::SetSizeAndPosition(int x, int y, int width, int height) {
    if (hButton != nullptr) {
        buttonX = x;
        buttonY = y;

        // Sets coords and dimensions of the Button Window
        SetWindowPos(hButton, nullptr, buttonX, buttonY, width, height, SWP_NOZORDER);
    }
}

/**
 *  Function encapsulates the computation when resizing 
 *  @param updWinWidth :: Updated Window Width
 *  @param updWinHeight :: Updated Window Height
 */
void ButtonManager::ComputeResize(int updWinWidth, int updWinHeight) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int newWidth, newHeight;

    // Conditional check with Boolean Flag check upon call
    if (!wasResized) {
        buttonWidth = screenWidth / widthDivisor;
        buttonHeight = screenHeight / heightDivisor;
        wasResized = true;
    }
    else {
        newWidth = updWinWidth / widthDivisor;
        newHeight = updWinHeight / heightDivisor;

        // Sets Mininum constraints to dimensions
        const int minWidth = 200;
        const int minHeight = 50;
        buttonWidth = std::max(newWidth, minWidth);
        buttonHeight = std::max(newHeight, minHeight);
    }
}

COLORREF ButtonManager::GetBgColor()
{
    return bgColor;
}

COLORREF ButtonManager::GetTextColor()
{
    return textColor;
}

COLORREF ButtonManager::GetBorderColor()
{
    return borderColor;
}

int ButtonManager::GetWidth()
{
    return buttonWidth;
}

int ButtonManager::GetHeight()
{
    return buttonHeight;
}

// Destroys the Button Window
void ButtonManager::DestroyButton()
{
    DestroyWindow(hButton);
}