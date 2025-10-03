//
// Created by 권태완 on 2025. 10. 3..
//

#ifndef PAIR_H
#define PAIR_H

#include <cstdlib>

template<typename T1, typename T2>
class pair {
    private:

    public:
        T1 first;
        T2 second;
        pair();
        pair(const T1 &x, const T2 &y);
        pair(const pair<T1, T2>& p);
        ~pair();
};

template<typename T1, typename T2>  // TODO: Default 값 어케 할지 고민 -> 그냥 비어 놓으면 알아서 기본값으로 넣어줌
pair<T1, T2>::pair():first(), second() {}

template<typename T1, typename T2>
pair<T1, T2>::pair(const T1 &x, const T2 &y):first(x), second(y) {}

template<typename T1, typename T2>
pair<T1, T2>::pair(const pair<T1, T2>& p):first(p.first), second(p.second) {}

template<typename T1, typename T2>
pair<T1, T2>::~pair() {
}

#endif
