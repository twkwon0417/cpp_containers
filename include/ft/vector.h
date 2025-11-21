#ifndef FT_VECTOR_H
#define FT_VECTOR_H

// Dummy implementation for linking only.
// Provides minimal definitions to avoid linker errors.

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include "equal.h"
#include "iterator_traits.h"
#include "lexicographical_compare.h"
#include "reverse_iterator.h"
#include "enable_if.h"
#include "is_integral.h"

namespace ft
{
    template<class T, class Allocator = std::allocator<T> >
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef typename Allocator::pointer pointer;
        // TODO: 더 알아보기 LegacyRandomAccessIterator, LegacyContiguousIterator
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef const ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // function 구현
        explicit vector(const Allocator &alloc = Allocator())
                : _alloc(alloc),
                  _start(nullptr),
                  _finish(nullptr),
                  _end_of_storage(nullptr) {

        }

        explicit vector(size_type count, const T &value = T(),
                        const Allocator &alloc = Allocator())
                : _alloc(alloc),
                  _start(nullptr),
                  _finish(nullptr),
                  _end_of_storage(nullptr) {
            // https://en.cppreference.com/w/cpp/memory/allocator.html

            if (count > 0) {
                _start = _alloc.allocate(count);  // allocates uninitialized storage
                _finish = _start;
                _end_of_storage = _start + count;

                pointer current = _start;
                // TODO: 할당되지 못하는 경우 예외처리 해 줘야 할듯 일단 skip
                for (size_type i = 0; i < count; i++) {
                    _alloc.construct(current, value);  // constructs an object in allocated storage
                    ++current;
                }
                _finish = current;
            }
        }

        template<class InputIt>
        vector(InputIt first, InputIt last,
               const Allocator &alloc = Allocator(),
               typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type * = 0)
                : _alloc(alloc) {
            _construct_from_range(first, last,
                                  typename ft::iterator_traits<InputIt>::iterator_category());
        }

        vector(const vector &other)
                : _alloc(other._alloc),
                  _start(nullptr),
                  _finish(nullptr),
                  _end_of_storage(nullptr) {
            size_type count = other.size();

            if (count > 0) {
                _start = _alloc.allocate(count);
                _end_of_storage = _start + count;
                _finish = _start; // 예외 안전성을 위해 size=0으로 설정

                pointer current = _start;
                const_iterator other_it = other.begin();

                // TODO: 예외처리
                for (; other_it != other.end(); ++other_it, ++current) {
                    _alloc.construct(current, *other_it);
                }
                _finish = current;

            }
        }

        ~vector() {
            if (_start != NULL) {
                pointer current = _start;
                while (current != _finish) {  // TODO: 난 for문이 더 좋은데 무슨 형이지
                    _alloc.destroy(current);
                    ++current;
                }

                _alloc.deallocate(_start, capacity());
            }
        }

        // exception safety 보장 by Copy and Swap idiom
        vector &operator=(const vector &other) {
            // 최적화 b/c deep copy costs too much
            if (this == &other) {
                return *this;
            }

            vector temp(other);  // temp라는 이름으로 vector 객체 생성해줘 (성능상의 이점 x, 컴파일시 같은 기계어로)
            this->swap(temp);  // 여기선 error 안남
            return *this;
        }

        void assign(size_type count, const T &value) {
            vector temp(count, value, this->_alloc);
            this->swap(temp);
        }

        template<class InputIt>
        void assign(InputIt first, InputIt last) {
            vector temp(first, last, this->_alloc);
            this->swap(temp);
        }

        allocator_type get_allocator() const {
            return _alloc;
        }

        // exception을 던저야 해
        // 예외가 어디서 발생했는지 식별
        reference at(size_type pos) {
            if (pos >= this->size()) {
                throw std::out_of_range("vector");
            }
            return *(_start + pos);
        }

        const_reference at(size_type pos) const {
            // TODO: const member func size()가 호출된다 한다. 어케 알아서 해주는 거지: const안에서만 해준다 -> 그리고 이 클래스 외부 클래스에서는 const 안해도 알빠노
            if (pos >= this->size()) {
                throw std::out_of_range("vector::at");
            }
            return *(_start + pos);
        }

        reference operator[](size_type pos) {
            return *(_start + pos);
        }

        const_reference operator[](size_type pos) const {
            return *(_start + pos);
        };

        reference front() {
            return *_start;
        }

        const_reference front() const {
            return *_start;
        }

        const_reference back() {
            return *(_finish - 1);
        }

        const_reference back() const {
            return *(_finish - 1);
        }

        // cpp98에서는 except 터뜨려야 함
        bool empty() const {
            return _start == _finish;
        }

        // cpp98에서는 except 터뜨려야 함
        size_type size() const {

            return end() - begin();
            // return std::distance(begin(), end()); 위험해?
        }

        // cpp98에서는 except 터뜨려야 함
        // 1. Allocator가 반환하는 최대 크기
        // 2. 두 포인터(반복자) 간의 거리를 표현하는 'difference_type'의 최대값.
        size_type max_size() const {
            size_type alloc_limit = _alloc.max_size();

            size_type diff_limit = (size_type)std::numeric_limits<difference_type>::max();

            if (alloc_limit > diff_limit) {
                return diff_limit;
            }
            return alloc_limit;
        }

        size_type capacity() const {
            return _end_of_storage - _start;
        }

        iterator begin() {
            return _start;
        }

        const_iterator begin() const {
            return _start;
        }

        iterator end() {
            return _finish;
        }

        const_iterator end() const {
            return _finish;
        }

        void push_back(const T &value) {
            // 공간 있을때
            if (_finish != _end_of_storage) {
                _alloc.construct(_finish, value);
                ++_finish;
            }
            else {
                const size_type old_capacity = this->capacity();
                const size_type new_capacity = (old_capacity == 0) ? 1 : old_capacity * 2;

                pointer new_start = _alloc.allocate(new_capacity);
                pointer new_finish = new_start;
                pointer old_it = _start;

                for (; old_it != _finish; ++old_it, ++new_finish) {
                    _alloc.construct(new_finish, *old_it);
                }

                _alloc.construct(new_finish, value);
                ++new_finish; // size 1 증가

                // 정리 (기존 메모리 해제)
                for (pointer p = _start; p != _finish; ++p) {
                    _alloc.destroy(p);
                }
                if (_start != NULL) {
                    _alloc.deallocate(_start, old_capacity);
                }

                // Update
                _start = new_start;
                _finish = new_finish;
                _end_of_storage = new_start + new_capacity;
            }
        }

        void pop_back() {
            if (empty()) {
                return;
            }
            --_finish;
            _alloc.destroy(_finish);
        }

        iterator insert(const_iterator pos, const T &value) {
            size_type insert_index = pos - this->begin();
            this->insert(pos, 1, value);

            return _start + insert_index;
        }

        iterator insert(const_iterator pos,
                        size_type count, const T &value) {
            if (count == 0) {
                return (iterator) pos;
            }

            // insert index 저장
            size_type insert_index = pos - this->begin();
            size_type required_size = this->size() + count;


            // Relocation needed
            if (required_size > this->capacity()) {

                size_type old_capacity = this->capacity();

                size_type new_capacity = (old_capacity == 0) ? required_size : old_capacity * 2;
                if (new_capacity < required_size) {
                    new_capacity = required_size;
                }

                pointer new_start = _alloc.allocate(new_capacity);
                pointer new_finish = new_start;

                new_finish = std::uninitialized_copy(_start, _start + insert_index, new_start);

                std::uninitialized_fill_n(new_finish, count, value);
                new_finish += count;

                new_finish = std::uninitialized_copy(_start + insert_index, _finish, new_finish);

                if (_start != NULL) {
                    for (pointer p = _start; p < _finish; ++p)
                        _alloc.destroy(p);
                    _alloc.deallocate(_start, old_capacity);
                }

                _start = new_start;
                _finish = new_finish;
                _end_of_storage = new_start + new_capacity;
            }
            else {
                iterator insert_pos = _start + insert_index;
                size_type elements_after = _finish - insert_pos;

                if (elements_after > 0) {
                    if (elements_after >= count) {
                        std::uninitialized_copy(_finish - count, _finish, _finish);
                        _finish += count;
                        std::copy_backward(insert_pos, _finish - (count * 2), _finish - count);
                        std::fill_n(insert_pos, count, value);
                    } else {
                        std::uninitialized_fill_n(_finish, count - elements_after, value);
                        _finish += (count - elements_after);
                        std::uninitialized_copy(insert_pos, _finish - (count - elements_after), _finish);
                        _finish += elements_after;
                        std::fill_n(insert_pos, elements_after, value);
                    }
                } else {
                    std::uninitialized_fill_n(_finish, count, value);
                    _finish += count;
                }
            }
            return _start + insert_index;
        }

        template<class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last,
                        typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type * = 0) {
            size_type insert_index = pos - this->begin();

            for (; first != last; ++first) {
                iterator current_pos = _start + insert_index;
                this->insert(current_pos, *first); // 단일 삽입 호출
                ++insert_index;
            }
            return _start + (pos - this->begin()); // 원본 삽입 위치 반환
        }

        iterator erase(iterator pos) {
            if (pos != _finish) {
                std::copy(pos + 1, _finish, pos);
                --_finish;
                _alloc.destroy(_finish);
            }
            return pos;
        }

        iterator erase(iterator first, iterator last) {
            if (first == last) {
                return first;
            }
            iterator new_finish = std::copy(last, _finish, first);

            iterator p = new_finish;
            while (p != _finish) {
                _alloc.destroy(p);
                ++p;
            }
            _finish = new_finish;

            return first;
        }

        void clear() {
            if (empty()) {
                return;
            }
            pointer current = _start;
            while (current != _finish) {
                _alloc.destroy(current);
                ++current;
            }
            _finish = _start;
        }

        void resize(size_type count) {
            this->resize(count, value_type());
        }

        void resize(size_type count, const value_type &value) {
            size_type old_size = this->size();

            // size가 주는 경우
            if (count < old_size) {
                iterator new_finish = _start + count;
                for (iterator p = new_finish; p != _finish; ++p) {
                    _alloc.destroy(p);
                }

                _finish = new_finish;
            }
                // size가 늘어나는 경우
            else if (count > old_size) {
                size_type elements_to_add = count - old_size;
                // Capacity가 충분한 경우
                if (count <= this->capacity()) {
                    pointer current = _finish;
                    for (size_type i = 0; i < elements_to_add; ++i, ++current) {
                        _alloc.construct(current, value);
                    }
                    _finish = current;
                }
                    // Reallocation needed
                else {
                    size_type old_capacity = this->capacity();

                    size_type new_capacity = old_capacity * 2;
                    if (new_capacity < count) {
                        new_capacity = count;
                    }

                    pointer new_start = _alloc.allocate(new_capacity);
                    pointer new_finish = new_start; // 새 메모리의 끝 포인터

                    pointer p_copy = new_start;
                    for (iterator old_it = _start; old_it != _finish; ++old_it, ++p_copy) {
                        _alloc.construct(p_copy, *old_it);
                    }
                    new_finish = p_copy;

                    for (size_type i = 0; i < elements_to_add; ++i, ++new_finish) {
                        _alloc.construct(new_finish, value);
                    }

                    if (_start != NULL) {
                        for (iterator p = _start; p != _finish; ++p) {
                            _alloc.destroy(p);
                        }
                        _alloc.deallocate(_start, old_capacity);
                    }

                    _start = new_start;
                    _finish = new_finish;
                    _end_of_storage = new_start + new_capacity;
                }
            }
        }

        void reserve(size_type new_cap) {
            if (new_cap > this->max_size()) {
                throw std::length_error("vector::reserve");
            }

            if (new_cap <= this->capacity()) {
                return;
            }

            // reallocation
            size_type old_capacity = this->capacity();

            pointer new_start = _alloc.allocate(new_cap);
            pointer new_finish = new_start; // 예외 안전성을 위해

            new_finish = std::uninitialized_copy(_start, _finish, new_start);

            if (_start != NULL) {
                for (iterator p = _start; p != _finish; ++p) {
                    _alloc.destroy(p);
                }
                _alloc.deallocate(_start, old_capacity);
            }

            _start = new_start;
            _finish = new_finish;
            _end_of_storage = new_start + new_cap;
        }

        reverse_iterator rbegin() {
            return reverse_iterator(this->end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(this->end());
        }

        reverse_iterator rend() {
            return reverse_iterator(this->begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(this->begin());
        }

    private:
        allocator_type _alloc;
        pointer _start;
        pointer _finish;
        pointer _end_of_storage;

        void swap(vector &other) {
            // TODO: 한창 직접 구현하기 싫을 나이
            std::swap(_alloc, other._alloc);
            std::swap(_start, other._start);
            std::swap(_finish, other._finish);
            std::swap(_end_of_storage, other._end_of_storage);
        }

        template<class InputIt>
        void _construct_from_range(InputIt first, InputIt last, std::input_iterator_tag) {
            std::cout << "\nINPUT ITERATOR CALLED\n";

            _start = nullptr;
            _finish = nullptr;
            _end_of_storage = nullptr;

            for (; first != last; ++first) {
                push_back(*first);
            }
        }

        template<class ForwardIt>
        void _construct_from_range(ForwardIt first, ForwardIt last, std::forward_iterator_tag) {
            size_type n = std::distance(first, last);

            if (n > 0) {
                _start = _alloc.allocate(n);
                _finish = _start;
                _end_of_storage = _start + n;

                pointer current = _start;
                for (; first != last; ++first, ++current) {
                    _alloc.construct(current, *first);
                }
                _finish = current;

            } else {
                _start = nullptr;
                _finish = nullptr;
                _end_of_storage = nullptr;
            }
        }
    };

    template <class T, class Alloc>
    bool operator==(const ft::vector<T, Alloc>& lhs,
                    const ft::vector<T, Alloc>& rhs)
    {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }


    template <class T, class Alloc>
    bool operator<(const ft::vector<T, Alloc>& lhs,
                   const ft::vector<T, Alloc>& rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                            rhs.begin(), rhs.end());
    }

    template <class T, class Alloc>
    bool operator!=(const ft::vector<T, Alloc>& lhs,
                    const ft::vector<T, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Alloc>
    bool operator>(const ft::vector<T, Alloc>& lhs,
                   const ft::vector<T, Alloc>& rhs)
    {
        return rhs < lhs;
    }

    template <class T, class Alloc>
    bool operator<=(const ft::vector<T, Alloc>& lhs,
                    const ft::vector<T, Alloc>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>=(const ft::vector<T, Alloc>& lhs,
                    const ft::vector<T, Alloc>& rhs)
    {
        return !(lhs < rhs);
    }
} // namespace ft

#endif // FT_VECTOR_H