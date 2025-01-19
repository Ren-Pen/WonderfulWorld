#pragma once
#ifndef FILE_SLIMENANO_MATH_MTX_H
    #define FILE_SLIMENANO_MATH_MTX_H
    #include <cstddef>
    #include <array>
    #include <initializer_list>
    #include <algorithm>
    #include <ostream>

    #include <slimenano-api.h>
    #include <slm/slm-vector.h>

namespace slimenano::math {
    template <Arithmetic T, std::size_t Rows, std::size_t Cols> class base_matrix;
    namespace internal {
        template <Arithmetic T, std::size_t N> class base_matrix_support_identity {
          public:
            static base_matrix<T, N, N> identity() {
                base_matrix<T, N, N> result;
                for (std::size_t i = 0; i < N; ++i) {
                    result(i, i) = static_cast<T>(1);
                }
                return result;
            }
        };

        template <Arithmetic T, std::size_t N> class base_matrix_support_LUP {
          public:
            bool LUP_decomposition(base_matrix<T, N, N> &L, base_matrix<T, N, N> &U, base_matrix<T, N, N> &P) const {
                using mat = base_matrix<T, N, N>;
                P = base_matrix_support_identity<T, N>::identity();
                L = base_matrix_support_identity<T, N>::identity();
                U = mat(*static_cast<const mat *>(this));

                for (std::size_t j = 0; j < N; ++j) {
                    T max_pivot = U(j, j);
                    std::size_t pivot_row = j;
                    for (std::size_t p_i = j + 1; p_i < N; ++p_i) {
                        if (std::abs(U(p_i, j)) > std::abs(max_pivot)) {
                            max_pivot = U(p_i, j);
                            pivot_row = p_i;
                        }
                    }
                    if (std::abs(max_pivot) < static_cast<T>(SLIMENANO_LIMIT_EPSILON)) {
                        return false;
                    }
                    if (pivot_row != j) {
                        U.swap_row(j, pivot_row);
                        P.swap_row(j, pivot_row);
                        if (j > 0) {
                            L.swap_row(j, pivot_row, 0, j);
                        }
                    }
                }

                for (std::size_t j = 0; j < N - 1; ++j) {
                    if (std::abs(U(j, j)) < static_cast<T>(SLIMENANO_LIMIT_EPSILON)) {
                        return false;
                    }

                    for (std::size_t i = j + 1; i < N; ++i) {

                        L(i, j) = U(i, j) / U(j, j);

                        for (std::size_t k = j; k < N; ++k) {
                            U(i, k) -= L(i, j) * U(j, k);
                        }
                    }
                }

                return true;
            }
        };

        template <Arithmetic T> class base_matrix_support_determinant_1 {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 1, 1> *>(this));
                return that(0, 0);
            }

            T determinant([[maybe_unused]] const base_matrix<T, 1, 1> &U,
                          [[maybe_unused]] const base_matrix<T, 1, 1> &P) const {
                return determinant();
            }
        };

        template <Arithmetic T> class base_matrix_support_determinant_2 {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 2, 2> *>(this));
                return that(0, 0) * that(1, 1) - that(0, 1) * that(1, 0);
            }

            T determinant([[maybe_unused]] const base_matrix<T, 2, 2> &U,
                          [[maybe_unused]] const base_matrix<T, 2, 2> &P) const {
                return determinant();
            }
        };

        template <Arithmetic T> class base_matrix_support_determinant_3 {
          public:
            T determinant() const {
                const auto &that = (*static_cast<const base_matrix<T, 3, 3> *>(this));
                return that(0, 0) * (that(1, 1) * that(2, 2) - that(1, 2) * that(2, 1)) -
                       that(0, 1) * (that(1, 0) * that(2, 2) - that(1, 2) * that(2, 0)) +
                       that(0, 2) * (that(1, 0) * that(2, 1) - that(1, 1) * that(2, 0));
            }

            T determinant([[maybe_unused]] const base_matrix<T, 3, 3> &U,
                          [[maybe_unused]] const base_matrix<T, 3, 3> &P) const {
                return determinant();
            }
        };

        template <Arithmetic T, std::size_t N> class base_matrix_support_determinant_n {
          public:
            T determinant() const {
                using mat = base_matrix<T, N, N>;
                using LUP = base_matrix_support_LUP<T, N>;

                mat L, U, P;
                bool result = static_cast<const LUP *>(static_cast<const mat *>(this))->LUP_decomposition(L, U, P);
                if (!result) {
                    return 0;
                }

                return determinant(U, P);
            }

            T determinant(const base_matrix<T, N, N> &U, const base_matrix<T, N, N> &P) const {

                size_t pivot_count = 0;
                for (std::size_t i = 0; i < N; ++i) {
                    if (P(i, i) != 1) {
                        ++pivot_count;
                    }
                }

                T det = static_cast<T>(1);
                for (std::size_t i = 0; i < N; ++i) {
                    det *= U(i, i);
                }

                if (pivot_count % 2 == 1) {
                    det = -det;
                }

                if (std::abs(det) < static_cast<T>(SLIMENANO_LIMIT_EPSILON)) {
                    return 0;
                }

                return det;
            }
        };

        template <Arithmetic T, std::size_t N>
        class base_matrix_support_determinant : public slimenano::internal::maybe<N == 1, base_matrix_support_determinant_1<T>>,
                                                public slimenano::internal::maybe<N == 2, base_matrix_support_determinant_2<T>>,
                                                public slimenano::internal::maybe<N == 3, base_matrix_support_determinant_3<T>>,
                                                public slimenano::internal::maybe<N >= 4, base_matrix_support_determinant_n<T, N>> {};

        template <Arithmetic T, std::size_t N> class base_matrix_support_inverse {
          public:
            base_matrix<T, N, N> inverse() const {
                using mat = base_matrix<T, N, N>;
                using LUP = base_matrix_support_LUP<T, N>;

                mat L, U, P;
                bool result = static_cast<const LUP *>(static_cast<const mat *>(this))->LUP_decomposition(L, U, P);

                if (!result) {
                    throw std::runtime_error("Matrix is singular");
                }

                return inverse(L, U, P);
            }

            base_matrix<T, N, N> inverse(const base_matrix<T, N, N> &L, const base_matrix<T, N, N> &U,
                                         const base_matrix<T, N, N> &P) const {
                using mat = base_matrix<T, N, N>;
                using DET = base_matrix_support_determinant<T, N>;

                T det = static_cast<const DET *>(static_cast<const mat *>(this))->determinant(U, P);

                if (det == 0) {
                    throw std::runtime_error("Matrix is singular");
                }

                mat result;

                for (std::size_t i = 0; i < N; ++i) {
                    T B[N]{0};
                    B[i] = 1;
                    for (int row = 0; row < N; ++row) {
                        B[row] = P(row, i);
                    }

                    T Y[N]{0};
                    for (std::size_t f_i = 0; f_i < N; ++f_i) {
                        T sum = static_cast<T>(0);
                        for (std::size_t f_j = 0; f_j < N; ++f_j) {
                            sum += L(f_i, f_j) * Y[f_j];
                        }
                        Y[f_i] = B[f_i] - sum;
                    }

                    T X[N]{0};
                    for (std::size_t b_i = 0; b_i < N; ++b_i) {
                        std::size_t b_ii = N - 1 - b_i;
                        T sum = static_cast<T>(0);
                        for (std::size_t b_j = N - 1; b_j > b_ii; --b_j) {
                            sum += U(b_ii, b_j) * X[b_j];
                        }
                        X[b_ii] = (Y[b_ii] - sum) / U(b_ii, b_ii);
                    }

                    for (std::size_t row = 0; row < N; ++row) {
                        result(row, i) = X[row];
                    }
                }

                return result;
            }
        };

        template <Arithmetic T, std::size_t Rows, std::size_t Cols>
        class base_matrix_support : public slimenano::internal::maybe<Rows == Cols, base_matrix_support_identity<T, Rows>>,
                                    public slimenano::internal::maybe<Rows == Cols, base_matrix_support_determinant<T, Rows>>,
                                    public slimenano::internal::maybe<Rows == Cols, base_matrix_support_LUP<T, Rows>>,
                                    public slimenano::internal::maybe<Rows == Cols, base_matrix_support_inverse<T, Rows>> {};
    } // namespace internal

    template <Arithmetic T, std::size_t Rows, std::size_t Cols>
    class base_matrix : public internal::base_matrix_support<T, Rows, Cols> {
        template <Arithmetic, std::size_t, std::size_t> friend class base_matrix;

      public:
        using matrix_row = std::array<T, Cols>;

      private:
        std::array<matrix_row, Rows> data;

      public:
        constexpr std::size_t rows() const { return Rows; }
        constexpr std::size_t cols() const { return Cols; }

      public:
        template <std::size_t OtherCols>
        static base_matrix<T, Rows, Cols + OtherCols> concat_horizontal(const base_matrix<T, Rows, Cols> &lhs,
                                                                        const base_matrix<T, Rows, OtherCols> &rhs) {
            base_matrix<T, Rows, Cols + OtherCols> result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result.data[i][j] = lhs.data[i][j];
                }
                for (std::size_t j = 0; j < OtherCols; ++j) {
                    result.data[i][Cols + j] = rhs.data[i][j];
                }
            }
            return result;
        }

        template <std::size_t OtherRows>
        static base_matrix<T, Rows + OtherRows, Cols> concat_vertical(const base_matrix<T, Rows, Cols> &lhs,
                                                                      const base_matrix<T, OtherRows, Cols> &rhs) {
            base_matrix<T, Rows + OtherRows, Cols> result;
            for (std::size_t i = 0; i < Rows; ++i) {
                result.data[i] = lhs.data[i];
            }
            for (std::size_t i = 0; i < OtherRows; ++i) {
                result.data[Rows + i] = rhs.data[i];
            }
            return result;
        }

        static base_matrix zero() { return base_matrix(); }

      public:
        base_matrix() {
            for (matrix_row &row : data) {
                row.fill(static_cast<T>(0));
            }
        }

        base_matrix(const std::array<std::array<T, Cols>, Rows> &array) : data(array) {}

        base_matrix(const std::initializer_list<std::initializer_list<T>> init) {
            const std::size_t minSize = init.size() < Rows ? init.size() : Rows;
            for (std::size_t i = 0; i < minSize; i++) {
                auto &initRow = *(init.begin() + i);
                auto &row = data[i];
                const std::size_t minColSize = initRow.size() < Cols ? initRow.size() : Cols;
                std::copy_n(initRow.begin(), minColSize, row.begin());
                if (minColSize < Cols) {
                    std::fill(row.begin() + minColSize, row.end(), static_cast<T>(0));
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

        base_matrix(const base_matrix &other) { std::copy(other.data.begin(), other.data.end(), data.begin()); }

        base_matrix &operator=(const base_matrix &other) {
            std::copy(other.data.begin(), other.data.end(), data.begin());
            return *this;
        }

        base_matrix(const base_matrix &&other) { data = std::move(other.data); }

        base_matrix &operator=(const base_matrix &&other) {
            data = std::move(other.data);
            return *this;
        }

        template <std::size_t OtherRows, std::size_t OtherCols>
        base_matrix(const base_matrix<T, OtherRows, OtherCols> &other) {
            constexpr std::size_t minRowSize = OtherRows < Rows ? OtherRows : Rows;
            constexpr std::size_t minColSize = OtherCols < Cols ? OtherCols : Cols;
            for (std::size_t i = 0; i < minRowSize; i++) {
                std::copy_n(other[i].begin(), minColSize, data[i].begin());

                if constexpr (minColSize < Cols) {
                    std::fill(data[i].begin() + minColSize, data[i].end(), static_cast<T>(0));
                }
            }

            if constexpr (minRowSize < Rows) {
                std::for_each(data.begin() + minRowSize, data.end(), [](matrix_row &row) {
                    row.fill(static_cast<T>(0));
                });
            }
        }

      public:
        T &operator()(const std::size_t &row, const std::size_t &col) {
            if (row >= Rows || col >= Cols) {
                throw std::out_of_range("Index out of range");
            }
            return data[row][col];
        }

        const T &operator()(const std::size_t &row, const std::size_t &col) const {
            if (row >= Rows || col >= Cols) {
                throw std::out_of_range("Index out of range");
            }
            return data[row][col];
        }

        matrix_row &operator[](const std::size_t &row) {
            if (row >= Rows) {
                throw std::out_of_range("Index out of range");
            }
            return data[row];
        }

        const matrix_row &operator[](const std::size_t &row) const {
            if (row >= Rows) {
                throw std::out_of_range("Index out of range");
            }
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
        base_matrix<T, Rows, OtherCols> operator*(const base_matrix<T, Cols, OtherCols> &other) const {
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

        base_vector<T, Rows> operator*(const base_vector<T, Cols> &other) const {
            base_vector<T, Rows> result;
            for (std::size_t i = 0; i < Rows; ++i) {
                T sum = static_cast<T>(0);
                for (std::size_t j = 0; j < Cols; ++j) {
                    sum += data[i][j] * other[j];
                }
                result[i] = sum;
            }
            return result;
        }

        base_matrix transpose() const {
            base_matrix result;
            for (std::size_t i = 0; i < Rows; ++i) {
                for (std::size_t j = 0; j < Cols; ++j) {
                    result(j, i) = data[i][j];
                }
            }
            return result;
        }

        base_matrix &swap_row(std::size_t a, std::size_t b) {
            if (a != b) {
                std::swap(data[a], data[b]);
            }
            return *this;
        }

        base_matrix &swap_row(std::size_t a, std::size_t b, std::size_t start, std::size_t end) {
            if (a == b)
                return *this;
            for (std::size_t i = start; i < end; ++i) {
                std::swap(data[a][i], data[b][i]);
            }
            return *this;
        }
    };

    template <Arithmetic T, std::size_t Rows, std::size_t Cols>
    std::ostream &operator<<(std::ostream &os, const base_matrix<T, Rows, Cols> &matrix) {
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

    template <Arithmetic T, std::size_t Rows, std::size_t Cols>
    base_vector<T, Rows> operator*(const base_vector<T, Cols> &vec, const base_matrix<T, Rows, Cols> &other) {
        return other * vec;
    }

} // namespace slimenano

#endif