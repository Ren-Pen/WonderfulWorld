#include <slm.h>
#include <iostream>

SLIMENANO_API void slimenano::slm_version() {
    using namespace std;
    cout << "Open source math library by slimenano" << endl;
    cout << "Version: " << SLIMENANO_MATH_VERSION << endl;
}