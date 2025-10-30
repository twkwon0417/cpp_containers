//
// Created by 권태완 on 2025. 10. 4..
//

#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace ft{
    // 이러면 first++가 범위를 벗어낫을때 처리해줌?
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

    // 두 배열이 (시작부터) 일부 같은 경우 짧은게 작다고 판단
    // 두 배열이 같으면 returns false
    template<typename InputIt1, typename InputIt2>
    bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2 ) {
        for(; first1 != last1; ++first1, ++first2) {
            if(first2 == last2) {  // 비교해야 하는 범위 넘어 처음께 더 길이가 길다는 뜻
                return false;
            }
            if(*first1 < *first2) {
                return true;
            }
            else if (*first1 > *first2) {
                return false;
            }
        }
        return first2 != last2;
    }

    template<typename InputIt1, typename InputIt2, typename Compare>
    bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp ) {
        for(; first1 != last1; ++first1, ++first2) {
            if(first2 == last2) {  // 비교해야 하는 범위 넘어 처음께 더 길이가 길다는 뜻
                return false;
            }
            if(comp(*first1, *first2)) {
                return true;
            }
            else if (comp(*first2, *first1)) {
                return false;
            }
        }
        return first2 != last2;
    }

#endif //ALGORITHM_H
}
