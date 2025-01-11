#include <slui.h>
#include <iostream>

SLIMENANO_API void slimenano::slui_version() {
    using namespace std;
    cout << "Open source math library by slimenano" << endl;
    cout << "Version: " << SLIMENANO_UI_VERSION << endl;
}