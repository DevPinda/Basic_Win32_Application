#include "RandomColorGenerator.h"

// Constructor performs operations on gen and dis, also sets default values to RGB vars
RandomColorGenerator::RandomColorGenerator(): rd(), gen(rd()), dis(0,255), red(30), green(30), blue(30)  {
}

// Returns a randomly generated RGB value for the background
COLORREF RandomColorGenerator::GetRndBg(){
    red = dis(gen);
    green = dis(gen);
    blue = dis(gen);
    return RGB(red, green, blue);
}
