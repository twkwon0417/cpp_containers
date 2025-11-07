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

    template<typename Iter>
    class reverse_iterator {
        typedef Iter iterator_type;
        typedef typename ft::iterator_traits<Iter>::iterator_category iterator_category;
        typedef typename ft::iterator_traits<Iter>::value_type value_type;
        typedef typename ft::iterator_traits<Iter>::difference_type difference_type;
        typedef typename ft::iterator_traits<Iter>::pointer pointer;
        typedef typename ft::iterator_traits<Iter>::reference reference;

    protected:
        Iter current;

    public:
        reverse_iterator() : current() {}

        explicit reverse_iterator(iterator_type x) : current(x) {}

        template <class U>
        reverse_iterator(const reverse_iterator<U> &other) : current(other.base()) {}

        // ================================================

        template<typename U>
        reverse_iterator& operator=(const reverse_iterator<U>& other) {
            this->current = other.base();
            return *this;
        }

        iterator_type base() const {
            return this;
        }

        reference operator*() const {
            Iter tmp = current;
            return *--tmp;
        }

        pointer operator->() const {
            return &(operator*());
        }

        // https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator_at.html 어쩌라고
        reference operator[] ( difference_type n ) const {
            return current[-n - 1];
        }

        reverse_iterator& operator++() {
            --current; return *this;
        }

        reverse_iterator& operator--() {
            ++current; return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp = *this; --current; return tmp;
        }

        reverse_iterator operator--(int) {
            reverse_iterator tmp = *this; ++current; return tmp;
        }

        reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(current - n);
        }

        reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(current + n);
        }

        reverse_iterator& operator+=( difference_type n ) {
            current -= n; return *this;
        }

        reverse_iterator& operator-=( difference_type n ) {
            current += n; return *this;
        }
    };

    template<typename Iter1, typename Iter2 >
    bool operator==( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() == rhs.base();
    }

    template<typename Iter1, typename Iter2 >
    bool operator!=( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() != rhs.base();
    }

    template<typename Iter1, typename Iter2 >
    bool operator<( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() > rhs.base();
    }

    template<typename Iter1, typename Iter2 >
    bool operator<=( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() >= rhs.base();
    }

    template<typename Iter1, typename Iter2 >
    bool operator>( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() < rhs.base();
    }

    template<typename Iter1, typename Iter2 >
    bool operator>=( const std::reverse_iterator<Iter1>& lhs,
                     const std::reverse_iterator<Iter2>& rhs ) {
        return lhs.base() <= rhs.base();
    }

    template<typename Iter>
    reverse_iterator<Iter> operator+
            ( typename reverse_iterator<Iter>::difference_type n,
              const reverse_iterator<Iter>& it ) {
        return reverse_iterator<Iter>(it.base() - n);
    }

    template<typename Iter1, typename Iter2 >
    typename reverse_iterator<Iter1>::difference_type
    operator-( const reverse_iterator<Iter1>& lhs,
               const reverse_iterator<Iter2>& rhs ) {
        return rhs.base() - lhs.base();
    }
}
#endif //CPP_CONTAINERS_ITERATOR_H
