#include <slui.h>
#include <iostream>

SLIMENANO_API void slimenano::ui::version() {
    using namespace std;
    cout << "Open source UI library by slimenano" << endl;
    cout << "Version: " << __SLIMENANO_UI_VERSION << endl;
}