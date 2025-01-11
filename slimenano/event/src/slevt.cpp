#include <slevt.h>
#include <iostream>

SLIMENANO_API void slimenano::event::version() {
    using namespace std;
    cout << "Open source event library by slimenano" << endl;
    cout << "Version: " << __SLIMENANO_EVENT_VERSION << endl;
}