#pragma once
#ifndef __FILE_SLIMENANO_MATH_H
#define __FILE_SLIMENANO_MATH_H
#include <slimenano-api.h>
#include <slm/slm-vector.h>
#include <slm/slm-matrix.h>

namespace slimenano::math {

    SLIMENANO_API void version();

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

#endif //!__FILE_SLIMENANO_MATH_H