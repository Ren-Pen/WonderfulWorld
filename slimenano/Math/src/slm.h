#pragma once
#ifndef FILE_SLIMENANO_MATH_H
#define FILE_SLIMENANO_MATH_H
#include <slimenano-api.h>
#include <slm/slm-vector.h>
#include <slm/slm-matrix.h>

SLIMENANO_LIBRARIES_DECLARED(math);

namespace slimenano::math {

    using vector2f = base_vector<float, 2>;
    using vector3f = base_vector<float, 3>;
    using vector4f = base_vector<float, 4>;

    using vector2d = base_vector<double, 2>;
    using vector3d = base_vector<double, 3>;
    using vector4d = base_vector<double, 4>;

    using matrix2f = base_matrix<float, 2, 2>;
    using matrix3f = base_matrix<float, 3, 3>;
    using matrix4f = base_matrix<float, 4, 4>;

    using matrix2d = base_matrix<double, 2, 2>;
    using matrix3d = base_matrix<double, 3, 3>;
    using matrix4d = base_matrix<double, 4, 4>;

} // namespace slimenano

#endif //!FILE_SLIMENANO_MATH_H