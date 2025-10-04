//
// Created by 권태완 on 2025. 10. 3..
// .은 객체 내부 접근, ::은 class 접근
// &은 참조를 의미 주소외에도 다양한 것들을 의미

#ifndef PAIR_H
#define PAIR_H

namespace ft {

    template<typename T1, typename T2>
    class pair {
    private:

    public:
        typedef T1 first_type;
        typedef T2 second_type;
        T1 first;
        T2 second;

        pair();

        pair(const T1 &x, const T2 &y);

        pair(const pair<T1, T2> &p);

        ~pair();

        pair &operator=(const pair &other);

        template<typename U1, typename U2>
        pair &
        operator=(const pair<U1, U2> &other);  // 자기 자신의 상태를 직접 바꾸는 연산은 객체지향원칙에 따라 그 객체의 기능, 즉 멤버 함수로 구현하는 것이 가장 자연스럽다.
    };

    template<typename T1, typename T2>
    // Default 값 어케 할지 고민 -> 그냥 비어 놓으면 알아서 기본값으로 넣어줌
    pair<T1, T2>::pair():first(), second() {}

    template<typename T1, typename T2>
    pair<T1, T2>::pair(const T1 &x, const T2 &y):first(x), second(y) {}

    template<typename T1, typename T2>
    pair<T1, T2>::pair(const pair<T1, T2> &p):first(p.first), second(p.second) {}

    template<typename T1, typename T2>
    pair<T1, T2>::~pair() {
    }

    template<typename T1, typename T2>
    pair<T1, T2> &pair<T1, T2>::operator=(const pair<T1, T2> &other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    template<typename T1, typename T2>
    template<typename U1, typename U2>
    pair<T1, T2> &pair<T1, T2>::operator=(const pair<U1, U2> &other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

// Non-member function
// 대칭성: a == b가 참이면 b == a도 참이어야 합니다. 비교는 대칭적 관계입니다. a가 주체라기보다는 a와 b를 동등하게 놓고 비교하는 것에 가깝
// 유연성 (가장 중요한 이유): 비교 연산자를 일반 함수로 만들면 암시적 타입 변환을 양쪽 피연산자에 모두 자유롭게 적용할 수 있다.
    template<typename T1, typename T2>
    bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }

    template<typename T1, typename T2>
    bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        return !(lhs.first == rhs.first && lhs.second == rhs.second);
    }

    template<typename T1, typename T2>
    // TODO: 여기부터 합시다.
    bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        if (lhs.first < rhs.first) return true;
        if (lhs.first == rhs.first && lhs.second < rhs.second) return true;
        return false;
    }

    template<typename T1, typename T2>
    bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        if (lhs.first < rhs.first) return true;
        if (lhs.first == rhs.first && lhs.second <= rhs.second) return true;
        return false;
    }

    template<typename T1, typename T2>
    bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        if (lhs.first > rhs.first) return true;
        if (lhs.first == rhs.first && lhs.second > rhs.second) return true;
        return false;
    }

    template<typename T1, typename T2>
    bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        if (lhs.first > rhs.first) return true;
        if (lhs.first == rhs.first && lhs.second >= rhs.second) return true;
        return false;
    }

#endif
}