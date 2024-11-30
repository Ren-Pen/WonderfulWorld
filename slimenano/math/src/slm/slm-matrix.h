#pragma once
#ifndef __FILE_SLIMENANO_MATH_MTX_H
#define __FILE_SLIMENANO_MATH_MTX_H
#include <cstddef>
#include <array>
#include <initializer_list>
#include <algorithm>
#include <ostream>

#include <slimenano-api.h>
#include <slm/slm-vector.h>

namespace slimenano {
    template <typename T, std::size_t Rows, std::size_t Cols> class base_matrix;
    namespace internal {
        template <typename T, std::size_t N>
        class base_matrix_support_identity {
          public:
            static base_matrix<T, N, N> identity() {
                base_matrix<T, N, N> result;
                for (std::size_t i = 0; i < N; ++i) {
                    result(i, i) = static_cast<T>(1);
                }
                return result;
            }
        };

        template <typename T> class base_matrix_support_determinant_2 {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 2, 2> *>(this));
                return that(0, 0) * that(1, 1) - that(0, 1) * that(1, 0);
            }
        };

        template <typename T> class base_matrix_support_determinant_3 {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 3, 3> *>(this));
                return that(0, 0) *
                           (that(1, 1) * that(2, 2) - that(1, 2) * that(2, 1)) -
                       that(0, 1) *
                           (that(1, 0) * that(2, 2) - that(1, 2) * that(2, 0)) +
                       that(0, 2) *
                           (that(1, 0) * that(2, 1) - that(1, 1) * that(2, 0));
            }
        };

        template <typename T, std::size_t N>
        class base_matrix_support_determinant_n {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 2, 2> *>(this));
                T det = static_cast<T>(0);
                for (std::size_t j = 0; j < N; ++j) {
                    base_matrix<T, N - 1, N - 1> minor;
                    for (std::size_t i = 1; i < N; ++i) {
                        std::size_t minor_col = 0;
                        for (std::size_t k = 0; k < N; ++k) {
                            if (k != j) {
                                minor[i - 1][minor_col] = that(i, k);
                                ++minor_col;
                            }
                        }
                    }
                    det += (j % 2 == 0 ? 1 : -1) * that(0, j) *
                           minor.determinant();
                }
                return det;
            }
        };

        template <typename T, std::size_t N>
        class base_matrix_support_determinant
            : public maybe<N == 2, base_matrix_support_determinant_2<T>>,
              public maybe<N == 3, base_matrix_support_determinant_3<T>>,
              public maybe<N >= 4, base_matrix_support_determinant_n<T, N>> {};

        template <typename T, std::size_t Rows, std::size_t Cols>
        class base_matrix_support
            : public maybe<Rows == Cols, base_matrix_support_identity<T, Rows>>,
              public maybe<Rows == Cols, base_matrix_support_determinant<T, Rows>> {};
    } // namespace internal

    template <typename T, std::size_t Rows, std::size_t Cols>
    class base_matrix : public internal::base_matrix_support<T, Rows, Cols> {
        template <typename, std::size_t, std::size_t> friend class base_matrix;

      public:
        using matrix_row = std::array<T, Cols>;

      private:
        std::array<matrix_row, Rows> data;

      public:
        constexpr std::size_t rows() const { return Rows; }
        constexpr std::size_t cols() const { return Cols; }

      public:
        base_matrix() {
            for (matrix_row &row : data) {
                row.fill(static_cast<T>(0));
            }
        }

        base_matrix(const std::array<std::array<T, Cols>, Rows> &array)
            : data(array) {}

        base_matrix(
            const std::initializer_list<std::initializer_list<T>> init) {
            const std::size_t minSize = init.size() < Rows ? init.size() : Rows;
            for (std::size_t i = 0; i < minSize; i++) {
                auto &initRow = *(init.begin() + i);
                auto &row = data[i];
                const std::size_t minColSize =
                    initRow.size() < Cols ? initRow.size() : Cols;
                std::copy_n(initRow.begin(), minColSize, row.begin());
                if (minColSize < Cols) {
                    std::fill(row.begin() + minColSize, row.end(),
                              static_cast<T>(0));
                }
            }
            if (minSize < Rows) {
                for (std::size_t i = minSize; i < Rows; i++) {
                    data[i].fill(static_cast<T>(0));
                }
            }
        }

        base_matrix(const std::initializer_list<base_vector<T, Cols>> init) {
            const std::size_t minSize = init.size() < Rows ? init.size() : Rows;
            for (std::size_t i = 0; i < minSize; i++) {
                for (std::size_t j = 0; j < Cols; j++) {
                    data[i][j] = (*(init.begin() + i))[j];
                }
            }
            if (minSize < Rows) {
                for (size_t i = minSize; i < Rows; i++) {
                    data[i].fill(static_cast<T>(0));
                }
            }
        }

        base_matrix(const base_matrix &other) {
            std::copy(other.data.begin(), other.data.end(), data.begin());
        }

        base_matrix &operator=(const base_matrix &other) {
            std::copy(other.data.begin(), other.data.end(), data.begin());
        }

        base_matrix(const base_matrix &&other) { data = std::move(other.data); }

        base_matrix &operator=(const base_matrix &&other) {
            data = std::move(other.data);
        }

        template <std::size_t OtherRows, std::size_t OtherCols>
        base_matrix(const base_matrix<T, OtherRows, OtherCols> &other) {
            constexpr std::size_t minRowSize =
                OtherRows < Rows ? OtherRows : Rows;
            constexpr std::size_t minColSize =
                OtherCols < Cols ? OtherCols : Cols;
            for (std::size_t i = 0; i < minRowSize; i++) {
                std::copy_n(other[i].begin(), minColSize, data[i].begin());

                if constexpr (minColSize < Cols) {
                    std::fill(data[i].begin() + minColSize, data[i].end(),
                              static_cast<T>(0));
                }
            }

            if constexpr (minRowSize < Rows) {
                std::for_each(
                    data.begin() + minRowSize, data.end(),
                    [](matrix_row &row) { row.fill(static_cast<T>(0)); });
            }
        }

      public:
        T &operator()(const std::size_t &row, const std::size_t &col) {
            return data[row][col];
        }

        const T &operator()(const std::size_t &row,
                            const std::size_t &col) const {
            return data[row][col];
        }

        matrix_row &operator[](const std::size_t &row) { return data[row]; }

        const matrix_row &operator[](const std::size_t &row) const {
            return data[row];
        }

      public:
        base_matrix operator+(const base_matrix &other) const {
            base_matrix result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result(i, j) = data[i][j] + other(i, j);
                }
            }
            return result;
        }

        base_matrix operator-(const base_matrix &other) const {
            base_matrix result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result(i, j) = data[i][j] - other(i, j);
                }
            }
            return result;
        }

        base_matrix operator*(const T &scalar) const {
            base_matrix result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result(i, j) = data[i][j] * scalar;
                }
            }
            return result;
        }

        base_matrix &operator+=(const base_matrix &other) {
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    data[i][j] += other(i, j);
                }
            }
            return *this;
        }

        base_matrix &operator-=(const base_matrix &other) {
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    data[i][j] -= other(i, j);
                }
            }
            return *this;
        }

        base_matrix operator*=(const T &scalar) {
            base_matrix result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    data[i][j] *= scalar;
                }
            }
            return result;
        }

      public:
        template <std::size_t OtherCols>
        base_matrix<T, Rows, OtherCols>
        operator*(const base_matrix<T, Cols, OtherCols> &other) const {
            base_matrix<T, Rows, OtherCols> result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < OtherCols; ++j) {
                    T sum = static_cast<T>(0);
                    for (std::size_t k = 0; k < Cols; ++k) {
                        sum += data[i][k] * other(k, j);
                    }
                    result(i, j) = sum;
                }
            }
            return result;
        }

        base_matrix<T, Cols, Rows> transpose() const {
            base_matrix<T, Cols, Rows> result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result(j, i) = data[i][j];
                }
            }
            return result;
        }
    };

    template <typename T, std::size_t Rows, std::size_t Cols>
    std::ostream &operator<<(std::ostream &os,
                             const base_matrix<T, Rows, Cols> &matrix) {
        for (std::size_t i = 0; i < Rows; i++) {
            for (std::size_t j = 0; j < Cols; j++) {
                os << matrix[i][j];
                if (j != Cols - 1) {
                    os << " ";
                }
            }
            if (i != Rows - 1) {
                os << "\n";
            }
        }
        return os;
    }

} // namespace slimenano

#endif