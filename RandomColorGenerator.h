#ifndef Basic_Win64_Application_RANDOMCOLORGENERATOR_H
#define Basic_Win64_Application_RANDOMCOLORGENERATOR_H
#endif //Basic_Win64_Application_RANDOMCOLORGENERATOR_H

#include <Windows.h>
#include <random>

class RandomColorGenerator{
public:
    RandomColorGenerator();
    COLORREF GetRndBg();
private:
    std::random_device rd;
    std::uniform_int_distribution<int> dis;
    std::mt19937 gen;
    int red, green, blue;
};