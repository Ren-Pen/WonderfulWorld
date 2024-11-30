#pragma once
#ifndef __FILE_SLIMENANO_API_H
#define __FILE_SLIMENANO_API_H
#include <type_traits>

#ifdef _MSC_VER
#ifdef _SLIMENANO_LIBRARY
#define SLIMENANO_API __declspec(dllexport)
#else
#define SLIMENANO_API __declspec(dllimport)
#endif //!_SLIMENANO_LIBRARY
#elif __GNUC__
#ifdef _SLIMENANO_LIBRARY
#define SLIMENANO_API __attribute__((visibility("default")))
#else
#define SLIMENANO_API
#endif //!_SLIMENANO_LIBRARY
#else
#define SLIMENANO_API
#pragma warning Unknown dynamic link import / export semantics.
#endif

#endif //!__FILE_SLIMENANO_API_H

namespace slimenano {
namespace internal {
template <class T> struct empty_t {};
template <bool b, class T> using maybe = std::conditional_t<b, T, empty_t<T>>;

} // namespace internal
} // namespace slimenano