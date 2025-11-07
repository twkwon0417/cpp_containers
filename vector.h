#ifndef FT_VECTOR_H
#define FT_VECTOR_H

// Dummy implementation for linking only.
// Provides minimal definitions to avoid linker errors.

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "equal.h"
#include "iterator_traits.h"
#include "lexicographical_compare.h"
#include "reverse_iterator.h"

namespace ft
{
    template<class T, class Allocator = std::allocator<T> >
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef typename Allocator::pointer pointer;
        // TODO: 더 알아보기 LegacyRandomAccessIterator, LegacyContiguousIterator
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef const ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // function 구현
        explicit vector(const Allocator &alloc = Allocator())
                : _alloc(alloc),
                  _start(nullptr),
                  _finish(nullptr),
                  _end_of_storage(nullptr) {

        }

        explicit vector(size_type count, const T& value = T(),
                         const Allocator& alloc = Allocator())
                         : _alloc(alloc),
                           _start(nullptr),
                           _finish(nullptr),
                           _end_of_storage(nullptr) {
            // https://en.cppreference.com/w/cpp/memory/allocator.html

            if(count > 0) {
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
                const Allocator& alloc = Allocator())
                : _alloc(alloc)
                {
            // TODO: 구현 해야해
            // Case 1: Dont know distance between first and last (InputIterator)
            // Case 2: Knows the distance between first and last (ForwardIterator)
        }

        vector(const vector& other)
            : _alloc(other._alloc),
            _start(nullptr),
            _finish(nullptr),
            _end_of_storage(nullptr) {
            size_type count = other.size();

            if(count > 0) {
                _start = _alloc.allocate(count);
                _end_of_storage = _start + count;
                _finish = _start;

                pointer current = _start;
                const_iterator other_it = other.begin();

                // TODO: 예외 처리 해야함
                for (pointer p = _start; p < current; ++p) {
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
        vector& operator=( const vector& other ) {
            // 최적화 b/c deep copy costs too much
            if (this == &other) {
                return *this;
            }

            vector temp(other);  // temp라는 이름으로 vector 객체 생성해줘 (성능상의 이점 x, 컴파일시 같은 기계어로)

            this->swap(other);  // 여기선 error 안남

            return *this;
        }

        void assign( size_type count, const T& value ) {
            // TODO: 대입 연산자와 유사
        }

        template<class InputIt>
        void assign(InputIt first, InputIt last) {
            // TODO: 대입 연산자와 유사
        }

        allocator_type get_allocator() const {
            return _alloc;
        }

        // exception을 던저야 해
        // 예외가 어디서 발생했는지 식별
        reference at(size_type pos) {
            if(pos >= this->size()) {
                throw std::out_of_range("vector::at");
            }
            return *(_start + pos);
        }

        const_reference at(size_type pos) const {
            // TODO: const member func size()가 호출된다 한다. 어케 알아서 해주는 거지
            if(pos >= this->size()) {
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
            return *_finish;
        }

        const_reference back() const {
            return *_finish;
        }

        // cpp98에서는 except 터뜨려야 함
        bool empty() const {
            return _start == _finish;
        }

        // cpp98에서는 except 터뜨려야 함
        size_type size() const {
            return std::distance(begin(), end());
        }

        // cpp98에서는 except 터뜨려야 함
        // 1. Allocator가 반환하는 최대 크기
        // 2. 두 포인터(반복자) 간의 거리를 표현하는 'difference_type'의 최대값.
        size_type max_size() const {
            return NULL;
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

    private:
        allocator_type _alloc;
        pointer _start;
        pointer _finish;
        pointer _end_of_storage;

        void swap(vector& other) {
            // TODO: 한창 직접 구현하기 싫을 나이
            std::swap(_alloc, other._alloc);
            std::swap(_start, other._start);
            std::swap(_finish, other._finish);
            std::swap(_end_of_storage, other._end_of_storage);
        }
    };
} // namespace ft

#endif // FT_VECTOR_H
