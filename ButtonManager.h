/**
 * Header file, acts as interface that requires the ButtonManager encapsulating
 * class to adhere to the following set methods and variable types
 */
#ifndef Basic_Win64_Application_BUTTONMANAGER_H
#define Basic_Win64_Application_BUTTONMANAGER_H
#endif //Basic_Win64_Application_BUTTONMANAGER_H

#pragma once
#include <Windows.h>

class ButtonManager
{
public:
    ButtonManager(HWND parentHwnd, HINSTANCE hInstance, int x, int y, int widthDivisor, int heightDivisor,const WCHAR *buttonName, COLORREF bgColor,
                  COLORREF textColor, COLORREF borderColor, int fontSize, LPCWSTR fontFamily, HMENU__ *buttonId);
    void SetSizeAndPosition(int x, int y, int width, int height);
    void ComputeResize(int updWinWidth,int updWinHeight);
    void DestroyButton();
    COLORREF GetBgColor();
    COLORREF GetTextColor();
    COLORREF GetBorderColor();
    int GetWidth();
    int GetHeight();
private:
    HWND hButton;
    int buttonX, buttonY, widthDivisor, heightDivisor, buttonWidth, buttonHeight, fontSize;
    COLORREF bgColor, textColor, borderColor;
    bool wasResized;
};

