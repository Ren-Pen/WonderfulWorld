#include "slm/slm-matrix.h"
#include "slm/slm-vector.h"
#include <array>
#include <iostream>

#include <slm.h>

    class ABC {
        public:
        ABC operator+(const ABC& other) {
           return *this; 
        }
        ABC operator*(const ABC& other) {
           return *this;
        }
        ABC operator/(const ABC& other) {
            return *this;
        }
        ABC operator-(const ABC& other) {
            return *this;
        }
        ABC(int i){}
        ABC(double d){}
        ABC() = default;
        ABC(const ABC&) = default;
        ABC(const ABC&&){};
        ABC& operator=(const ABC&) = default;
        ABC& operator=(const ABC&&){
            return *this;
        };
    };

int main() {

    using namespace slimenano;

    vector3f v3(std::array<float, 3>({1, 2, 3}));

    vector2f vec2 = v3;

    slm_version();

    std::cout << "\n" << vec2 << std::endl;

    vector2f vec22 = {5, 6};

    matrix2f mat2 = {vec2, vec22};

    matrix2f idmat2 = matrix2f::identity();

    std::cout << "\n" << mat2 << std::endl;
    std::cout << "\n" << idmat2 << std::endl;

    std::cout << "\n" << mat2 + idmat2 << std::endl;

    vector2f vecm2 = mat2[0];
    std::cout << "\n" << vecm2 << std::endl;

    matrix3f mat3 = mat2;
    std::cout << "\n" << mat3 << std::endl;

    std::cout << "\n" << matrix4f(mat2) << std::endl;

    std::cout << "\n" << matrix2f(mat3 * 3) << std::endl;

    matrix3f dmat3({{4, 7, 2}, {3, 5, 1}, {1, 2, 3}});

    std::cout << "\n" << dmat3 << std::endl;

    std::cout << "\n" << dmat3.determinant() << std::endl;

    std::cout << "Matrix A:" << std::endl;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cout << dmat3(i, j) << " ";
        }
        std::cout << std::endl;
    }

    double det = dmat3.determinant();
    std::cout << "Determinant of A: " << det << std::endl;

    matrix3f A = dmat3;

    const float term1 = A(0, 0) * (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1));
    const float term2 = A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0));
    const float term3 = A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));
    std::cout << "Term 1: " << term1 << ", Term 2: " << -term2
              << ", Term 3: " << term3 << std::endl;
    std::cout << "Determinant: " << term1 - term2 + term3 << std::endl;

    matrix3f hm3 = matrix3f({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    std::cout << "\n"
              << matrix3f::concat_horizontal(hm3, matrix3f::identity())
              << std::endl;

    matrix3f swp_mat3 = matrix3f::identity();
    std::cout << "\n" << swp_mat3.swap_row(1, 2, 1, 3) << std::endl;

    matrix3f inv_mat3({{0, 6, 5}, {2, 3, 5}, {3, 3, 8}});
    matrix3f inv_mat3_inv = inv_mat3.inverse();

    std::cout << "\n" << inv_mat3_inv << std::endl;

    std::cout << "\n" << inv_mat3 * inv_mat3_inv << std::endl;

    matrix2f det_mat2({{5, 3}, {8, 7}});

    std::cout << "\n" << det_mat2.determinant() << std::endl;

    matrix4f det_mat4({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}});

    std::cout << "\n" << det_mat4.determinant() << std::endl;

    // std::cout << "\n" << det_mat4.inverse() << std::endl;

    base_matrix<float, 1, 1> m1({{1}});

    std::cout << "\n" << m1 << std::endl;

    std::cout << "\n" << m1.inverse() << std::endl;

    std::cout << "\n" << m1.determinant() << std::endl;

    matrix3f mmt3 = matrix3f({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    vector3f vm3 = vector3f({1, 2, 3});

    std::cout << "\n" << mmt3 * vm3 << std::endl;
    std::cout << "\n" << vm3 * mmt3 << std::endl;

    // std::cout << "\n" << m1(2, 2) << std::endl;

    base_vector<char, 3> vec;


    base_vector<ABC, 3> vA;

    return 0;
}