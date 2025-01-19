#include <slm.h>
#include <iostream>

SLIMENANO_API void slimenano::math::version() {
    using namespace std;
    cout << "Open source math library by slimenano" << endl;
    cout << "Version: " << __SLIMENANO_MATH_VERSION << endl;
}