#ifndef Basic_Win64_Application_POINTERMANAGER_H
#define Basic_Win64_Application_POINTERMANAGER_H
#endif //Basic_Win64_Application_POINTERMANAGER_H

#pragma once
#include <Windows.h>
#include "ButtonManager.h"
#include <vector>

class PointerManager
{
public:
    static void SetButtonPointers(const std::vector<ButtonManager *>& pointerArray);
    static std::vector<ButtonManager *> GetButtonPointers();
private:
};
