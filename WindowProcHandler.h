#ifndef Basic_Win64_Application_WINDOWPROCHANDLER_H
#define Basic_Win64_Application_WINDOWPROCHANDLER_H
#pragma once
#include <Windows.h>
#include <iostream>
#include "ButtonManager.h"

class WindowProcHandler {
public:
    static LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif //Basic_Win64_Application_WINDOWPROCHANDLER_H
