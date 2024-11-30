#pragma once
#ifndef __FILE_SLIMENANO_MATH_VEC_H
#define __FILE_SLIMENANO_MATH_VEC_H
#include <cstddef>
#include <array>
#include <algorithm>
#include <functional>
#include <utility>
#include <numeric>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <slimenano-api.h>

namespace slimenano {

    template <typename T, std::size_t N> class base_vector;

    namespace internal {
        template <typename T> class base_vector_support_2 {
          public:
            base_vector<T, 2> rotate(T angle) const {
                auto& that = (*static_cast<base_vector<T, 2> *>(this));
                auto cos_theta = std::cos(angle);
                auto sin_theta = std::sin(angle);
                return base_vector<T, 2>(
                    static_cast<T>(that[0] * cos_theta - that[1] * sin_theta),
                    static_cast<T>(that[0] * sin_theta - that[1] * cos_theta));
            }

            T cross(const base_vector<T, 2> &other) const {
                auto that = (*static_cast<base_vector<T, 2> *>(this));

                return that[0] * other[1] - that[1] * other[0];
            }
        };

        template <typename T> class base_vector_support_3 {
          public:
            base_vector<T, 3> cross(const base_vector<T, 3> &other) const {
                auto& that = (*static_cast<base_vector<T, 3> *>(this));

                return base_vector<T, 3>(
                    that[1] * other[2] - that[2] * other[1],
                    that[2] * other[0] - that[0] * other[2],
                    that[0] * other[1] - that[1] * other[0]);
            }
        };

        template <typename T, size_t N>
        class base_vector_support
            : public maybe<N == 2, base_vector_support_2<T>>,
              public maybe<N == 3, base_vector_support_3<T>> {};

        template <typename T, size_t N> class base_vector_properties_2 {
          public:
            T &y() { return (*static_cast<base_vector<T, N> *>(this))[1]; }
            const T &y() const {
                return (*static_cast<base_vector<T, N> *>(this))[1];
            }
            T &g() { return (*static_cast<base_vector<T, N> *>(this))[1]; }
            const T &g() const {
                return (*static_cast<base_vector<T, N> *>(this))[1];
            }
        };

        template <typename T, size_t N> class base_vector_properties_3 {
          public:
            T &z() { return (*static_cast<base_vector<T, N> *>(this))[2]; }
            const T &z() const {
                return (*static_cast<base_vector<T, N> *>(this))[2];
            }
            T &b() { return (*static_cast<base_vector<T, N> *>(this))[2]; }
            const T &b() const {
                return (*static_cast<base_vector<T, N> *>(this))[2];
            }
        };

        template <typename T, size_t N> class base_vector_properties_4 {
          public:
            T &w() { return (*static_cast<base_vector<T, N> *>(this))[3]; }
            const T &w() const {
                return (*static_cast<base_vector<T, N> *>(this))[3];
            }
            T &a() { return (*static_cast<base_vector<T, N> *>(this))[3]; }
            const T &a() const {
                return (*static_cast<base_vector<T, N> *>(this))[3];
            }
        };

        template <typename T, size_t N>
        class base_vector_properties
            : public maybe<N >= 2, base_vector_properties_2<T, N>>,
              public maybe<N >= 3, base_vector_properties_3<T, N>>,
              public maybe<N >= 4, base_vector_properties_4<T, N>> {};

    } // namespace internal

    template <typename T, std::size_t N>
    class base_vector : public internal::base_vector_support<T, N>,
                        public internal::base_vector_properties<T, N> {

        template <typename, std::size_t> friend class base_vector;

      private:
        std::array<T, N> data;

      public:
        constexpr std::size_t size() const { return N; }

      public:
        base_vector() { data.fill(static_cast<T>(0)); }

        base_vector(const std::array<T, N> &array) {
            std::copy(array.begin(), array.end(), data.begin());
        }

        base_vector(const base_vector &other) {
            std::copy(other.data.begin(), other.data.end(), data.begin());
        }

        base_vector &operator=(const base_vector &other) {
            if (this != &other) {
                std::copy(other.data.begin(), other.data.end(), data.begin());
            }
            return *this;
        }

        base_vector &operator=(const std::array<T, N> &array) {
            if (&this->data != &array) {
                std::copy(array.begin(), array.end(), data.begin());
            }
            return *this;
        }

        base_vector(base_vector &&other) noexcept
            : data(std::move(other.data)) {}

        base_vector &operator=(const base_vector &&other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
            }
            return *this;
        }

        base_vector(const std::initializer_list<T>& init) {
            const std::size_t M = init.size();
            const std::size_t minSize = (N < M) ? N : M;
            std::copy_n(init.begin(), minSize, data.begin());
            if (N > M) {
                std::fill(data.begin() + M, data.end(), static_cast<T>(0));
            }
        }

        template <std::size_t M> base_vector(const base_vector<T, M> &other) {
            constexpr std::size_t minSize = (N < M) ? N : M;
            std::copy_n(other.data.begin(), minSize, data.begin());
            if constexpr (N > M) {
                std::fill(data.begin() + M, data.end(), static_cast<T>(0));
            }
        }

      public:
        T &operator[](const std::size_t &index) { return data[index]; }

        const T &operator[](const std::size_t &index) const {
            return data[index];
        }

        T &x() { return (*this)[0]; }

        const T &x() const { return (*this)[0]; }

        T &r() { return (*this)[0]; }
        const T &r() const { return (*this)[0]; }

      private:
        template <typename BinaryOperation>
        base_vector binary_op(const base_vector &other,
                              BinaryOperation op) const {
            base_vector result;
            std::transform(data.begin(), data.end(), other.data.begin(),
                           result.data.begin(), op);
            return result;
        }

        template <typename BinaryOperation>
        base_vector &binary_self_op(const base_vector &other,
                                    BinaryOperation op) {
            std::transform(data.begin(), data.end(), other.data.begin(),
                           data.begin(), op);
            return *this;
        }

        template <typename UnaryOperation>
        base_vector &unary_op(UnaryOperation op) {
            std::transform(data.begin(), data.end(), data.begin(), op);
            return *this;
        }

      public:
        base_vector operator+(const base_vector &other) const {
            return binary_op(other, std::plus<T>());
        }

        base_vector &operator+=(const base_vector &other) {
            return binary_self_op(other, std::plus<T>());
        }

        base_vector operator-(const base_vector &other) const {
            return binary_op(other, std::minus<T>());
        }

        base_vector &operator-=(const base_vector &other) {
            return binary_self_op(other, std::minus<T>());
        }

        base_vector operator*(T scalar) const {
            return binary_op(*this, [scalar](T a) { return a * scalar; });
        }

        base_vector operator*=(T scalar) const {
            return unary_op([scalar](T a) { return a * scalar; });
        }

        base_vector operator/(T scalar) const {
            return binary_op(*this, [scalar](T a) { return a / scalar; });
        }

        base_vector operator/=(T scalar) const {
            return unary_op([scalar](T a) { return a / scalar; });
        }

        base_vector operator-() const {
            base_vector result;
            std::transform(data.begin(), data.end(), result.data.begin(),
                           std::negate<T>());
            return result;
        }

        base_vector normalized() const {
            T len = this->length();
            if (len == 0)
                return base_vector();
            return (*this) / len;
        }

      public:
        T dot(const base_vector &other) const {
            return std::inner_product(data.begin(), data.end(),
                                      other.data.begin(), 0);
        }

        T sqrt_length() const { return this->dot(*this); }

        T length() const {
            return static_cast<T>(std::sqrt(this->sqrt_length()));
        }

        T angle(const base_vector &other) const {
            T l1 = length();
            T l2 = other.length();
            auto cos_theta = dot(other) / (l1 * l2);
            return static_cast<T>(std::acos(std::clamp(cos_theta, -1, 1)));
        }

        T distance(const base_vector &other) const {
            return ((*this) - other).length();
        }

        base_vector projection(const base_vector &other) const {
            T dot_product = this->dot(other);
            T sqrt_length = other.sqrt_length();
            base_vector result;
            std::transform(
                other.data.begin(), other.data.end(), result.data.begin(),
                [dot_product, sqrt_length](T a) {
                    return static_cast<T>((dot_product / sqrt_length) * a);
                });
            return result;
        }

        base_vector translate(const base_vector &offset) const {
            return (*this) + offset;
        }

        base_vector rejection(const base_vector &other) const {
            return *this - this->projection(other);
        }
    };

    template <typename T, std::size_t N>
    std::ostream &operator<<(std::ostream &os,
                             const base_vector<T, N> &vector) {
        for (std::size_t i = 0; i < N; i++) {
            os << vector[i];
            if (i != N - 1) {
                os << " ";
            }
        }
        return os;
    }

} // namespace slimenano

#endif