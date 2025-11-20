#ifndef EQUAL_H
#define EQUAL_H

// second가 더 짧은 경우: UB로 간주. 보호되지 않는다.

namespace ft
{
    template<typename InputIt1, typename InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
        for(; first1 != last1; ++first1, ++first2) {
            if(*first1 != *first2) {
                return false;
            }
        }
        return true;
    }

    // 입력받은 predicate로 판단해주세요.
    template<typename InputIt1, typename InputIt2, typename BinaryPred>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPred p) {
        for(; first1 != last1; ++first1, ++first2) {
            if(!p(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }
} // namespace ft

#endif
