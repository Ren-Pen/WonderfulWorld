#pragma once
#ifndef __FILE_SLIMENANO_API_H
#    define __FILE_SLIMENANO_API_H
#    include <iostream>
#    include <type_traits>
#    include <concepts>

#    ifdef _MSC_VER
#        ifdef _SLIMENANO_LIBRARY
#            define SLIMENANO_API __declspec(dllexport)
#        else
#            define SLIMENANO_API __declspec(dllimport)
#        endif //!_SLIMENANO_LIBRARY
#    elif __GNUC__
#        ifdef _SLIMENANO_LIBRARY
#            define SLIMENANO_API __attribute__((visibility("default")))
#        else
#            define SLIMENANO_API
#        endif //!_SLIMENANO_LIBRARY
#    else
#        define SLIMENANO_API
#        pragma warning Unknown dynamic link import / export semantics.
#    endif

#    define __SLIMENANO_LIMIT_EPSILON 1e-6

#    define __SLIMENANO_MATH_VERSION "1.0"
#    define __SLIMENANO_UI_VERSION "1.0"
#    define __SLIMENANO_EVENT_VERSION "1.0"

#    ifndef __SLIMENANO_LOG_OUTPUT_STREAM
#        define __SLIMENANO_LOG_OUTPUT_STREAM std::cout
#    endif

#    define __SLIMENANO_LIBRARIES_DECLARED(libName)                                                                    \
        namespace slimenano::libName {                                                                                 \
            SLIMENANO_API void version();                                                                              \
        }

#    define __SLIMENANO_LIBRARIES_VERSION(libName, versionStr)                                                         \
        namespace slimenano::libName {                                                                                 \
            SLIMENANO_API void version() {                                                                             \
                __SLIMENANO_LOG_OUTPUT_STREAM << "Open source " << #libName << " library by slimenano" << "\n";        \
                __SLIMENANO_LOG_OUTPUT_STREAM << "Version: " << versionStr << "\n";                                    \
            }                                                                                                          \
        }

namespace slimenano {

    template <typename T>
    concept Arithmetic = requires(T a, T b) {
        requires std::is_arithmetic_v<T> || requires {
            { a + b } -> std::same_as<T>;
            { a - b } -> std::same_as<T>;
            { a * b } -> std::same_as<T>;
            { a / b } -> std::same_as<T>;
        };
        requires std::is_convertible_v<int, T>;
        requires std::is_convertible_v<double, T>;
    };

    template <typename T>
    concept Stream = std::is_convertible_v<T, std::ostream &>;
;

    namespace internal {
        template <class T> struct empty_t {};
        template <bool b, class T> using maybe = std::conditional_t<b, T, empty_t<T>>;
        template <typename T> static inline constexpr T _get_logger_stream() { return __SLIMENANO_LOG_OUTPUT_STREAM; }

    } // namespace internal

} // namespace slimenano

#endif //!__FILE_SLIMENANO_API_H