//
// Created by 권태완 on 2025. 10. 30..
//

#ifndef CPP_CONTAINERS_ITERATOR_H
#define CPP_CONTAINERS_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace ft {
    template<typename Iter>
    struct iterator_traits {
        typedef typename Iter::value_type        value_type;
        typedef typename Iter::difference_type   difference_type;
        typedef typename Iter::pointer           pointer;
        typedef typename Iter::reference         reference;
        typedef typename Iter::iterator_category iterator_category;
    };

    // C++에서 int* 같은 일반 포인터도 배열을 순회하는 이터레이터(iterator)로 취급
    template<typename T >
    struct iterator_traits<T*> {
        typedef typename std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef const std::random_access_iterator_tag iterator_category;
    };


}
#endif //CPP_CONTAINERS_ITERATOR_H
