#include "gui.h"
#include "Windows.h"

int WINAPI WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow
) {
    gui::init();
    
    return 0;
}