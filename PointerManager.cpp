#include <vector>
#include "PointerManager.h"
#include "ButtonManager.h"

// Global Variable of Vector type to store pointers 
std::vector<ButtonManager*> pointerArr;

// Function to retrieve Button Pointers when called, then store into the Global variable
void PointerManager::SetButtonPointers(const std::vector<ButtonManager*>& pointerArray) {
    for (ButtonManager* pointer : pointerArray) {
        pointerArr.push_back(pointer);
    }
}

// Function returns the Pointers by returning Global variable
std::vector<ButtonManager*> PointerManager::GetButtonPointers() {
    return pointerArr;
}