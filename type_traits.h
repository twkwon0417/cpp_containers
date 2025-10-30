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
}
#endif //CPP_CONTAINERS_TYPE_TRAITS_H
