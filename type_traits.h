//
// Created by 권태완 on 2025. 10. 30..
//

#ifndef CPP_CONTAINERS_TYPE_TRAITS_H
#define CPP_CONTAINERS_TYPE_TRAITS_H

#include <type_traits>

namespace ft{
    template<bool B, typename T = void>  // 기본 struct
    struct enable_if {
    };

    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    // is_integral
    template<typename T>
    struct is_integral {
        static const bool value = false;
    };

    template <>
    struct is_integral<bool> {
        static const bool value = true;
    };

    template <>
    struct is_integral<char> {
        static const bool value = true;
    };

    template <>
    struct is_integral<signed char> {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned char> {
        static const bool value = true;
    };

    template <>
    struct is_integral<wchar_t> {
        static const bool value = true;
    };

    template <>
    struct is_integral<short> {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned short> {
        static const bool value = true;
    };

    template <>
    struct is_integral<int> {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned int> {
        static const bool value = true;
    };

    template <>
    struct is_integral<long> {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned long> {
        static const bool value = true;
    };
}
#endif //CPP_CONTAINERS_TYPE_TRAITS_H
