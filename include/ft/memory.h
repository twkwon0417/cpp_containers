//
// Created by 권태완 on 2025. 11. 7..
//

#ifndef CPP_CONTAINERS_MEMORY_H
#define CPP_CONTAINERS_MEMORY_H

#include <cstddef>

namespace ft {

    template<class T>
    struct allocator {
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        // TODO: rebind
        typedef std::true_type is_always_equal;
    };

    template<class U>
    struct rebind {
        typedef allocator<U> other;
    };
}
#endif //CPP_CONTAINERS_MEMORY_H
