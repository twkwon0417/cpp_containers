#include <iostream>
#include <string>
#include <utility> // std::pair를 위해 필요
#include "utility.h"  // 직접 구현한 pair.h
#include "iterator.h"
#include "type_traits.h"

void test_reverse_iterator_behavior();
void test_reverse_iterator();


// 결과 출력용 함수
void print_result(const std::string& test_name, bool ft_res, bool std_res) {
    std::cout << "[ " << test_name << " ]\n";
    std::cout << "ft  : " << (ft_res ? "true" : "false") << "\n";
    std::cout << "std : " << (std_res ? "true" : "false") << "\n";
    std::cout << "=> " << (ft_res == std_res ? "\033[32mOK\033[0m" : "\033[31mFAIL\033[0m") << "\n\n";
}

struct TestStruct { };

template <typename T>
typename ft::enable_if<ft::is_integral<T>::value, std::string>::type
enable_if_test(T)
{
    return "Integral Type";
}

template <typename T>
typename ft::enable_if<!ft::is_integral<T>::value, std::string>::type
enable_if_test(T)
{
    return "NOT Integral Type";
}



int main() {
//    std::cout << "--- ft::pair와 std::pair 동작 비교 테스트 ---\n\n";
//
//    // 1. 생성자 테스트
//    std::cout << "========== 1. 생성자 테스트 ==========\n";
//    // 기본 생성자
//    ft::pair<int, std::string> ft_p1;
//    std::pair<int, std::string> std_p1;
//    print_result("기본 생성자 (int)", ft_p1.first == 0, std_p1.first == 0);
//    print_result("기본 생성자 (string)", ft_p1.second == "", std_p1.second == "");
//
//    // 값 초기화 생성자
//    ft::pair<int, std::string> ft_p2(42, "hello");
//    std::pair<int, std::string> std_p2(42, "hello");
//    print_result("값 초기화 생성자 (first)", ft_p2.first == 42, std_p2.first == 42);
//    print_result("값 초기화 생성자 (second)", ft_p2.second == "hello", std_p2.second == "hello");
//
//    // 복사 생성자
//    ft::pair<int, std::string> ft_p3(ft_p2);
//    std::pair<int, std::string> std_p3(std_p2);
//    print_result("복사 생성자 (first)", ft_p3.first == ft_p2.first, std_p3.first == std_p2.first);
//    print_result("복사 생성자 (second)", ft_p3.second == ft_p2.second, std_p3.second == std_p2.second);
//
//
//    // 2. 대입 연산자 테스트
//    std::cout << "========== 2. 대입 연산자 테스트 ==========\n";
//    ft::pair<int, std::string> ft_p4;
//    std::pair<int, std::string> std_p4;
//    ft_p4 = ft_p2;
//    std_p4 = std_p2;
//    print_result("대입 연산자", (ft_p4.first == ft_p2.first && ft_p4.second == ft_p2.second),
//                 (std_p4.first == std_p2.first && std_p4.second == std_p2.second));
//
//
//    // 3. 비교 연산자 테스트
//    std::cout << "========== 3. 비교 연산자 테스트 ==========\n";
//    ft::pair<int, int> ft_comp1(10, 20);
//    ft::pair<int, int> ft_comp2(10, 20); // same
//    ft::pair<int, int> ft_comp3(20, 10); // first is greater
//    ft::pair<int, int> ft_comp4(10, 30); // second is greater
//
//    std::pair<int, int> std_comp1(10, 20);
//    std::pair<int, int> std_comp2(10, 20); // same
//    std::pair<int, int> std_comp3(20, 10); // first is greater
//    std::pair<int, int> std_comp4(10, 30); // second is greater
//
//    // ==
//    print_result("p1 == p2", ft_comp1 == ft_comp2, std_comp1 == std_comp2);
//    print_result("p1 == p3", ft_comp1 == ft_comp3, std_comp1 == std_comp3);
//
//    // !=
//    print_result("p1 != p2", ft_comp1 != ft_comp2, std_comp1 != std_comp2);
//    print_result("p1 != p3", ft_comp1 != ft_comp3, std_comp1 != std_comp3);
//
//    // <
//    print_result("p1 < p3 (first big)", ft_comp1 < ft_comp3, std_comp1 < std_comp3);
//    print_result("p1 < p4 (second big)", ft_comp1 < ft_comp4, std_comp1 < std_comp4);
//    print_result("p1 < p2 (same)", ft_comp1 < ft_comp2, std_comp1 < std_comp2);
//
//    // <=
//    print_result("p1 <= p2 (same)", ft_comp1 <= ft_comp2, std_comp1 <= std_comp2);
//    print_result("p1 <= p4 (second big)", ft_comp1 <= ft_comp4, std_comp1 <= std_comp4);
//    print_result("p3 <= p1 (first small)", ft_comp3 <= ft_comp1, std_comp3 <= std_comp1);
//
//    // >
//    print_result("p3 > p1 (first big)", ft_comp3 > ft_comp1, std_comp3 > std_comp1);
//    print_result("p4 > p1 (second big)", ft_comp4 > ft_comp1, std_comp4 > std_comp1);
//    print_result("p1 > p2 (same)", ft_comp1 > ft_comp2, std_comp1 > std_comp2);
//
//    // >=
//    print_result("p1 >= p2 (same)", ft_comp1 >= ft_comp2, std_comp1 >= std_comp2);
//    print_result("p3 >= p1 (first big)", ft_comp3 >= ft_comp1, std_comp3 >= std_comp1);
//    print_result("p1 >= p4 (second small)", ft_comp1 >= ft_comp4, std_comp1 >= std_comp4);
//
//    // =============================
//    std::cout << std::boolalpha;
//
//    // ==================================================================
//    // Test 1: ft::is_integral
//    // ==================================================================
//    std::cout << "--- Testing ft::is_integral ---" << std::endl;
//
//    // Test 1.1: Integral types (should be true)
//    std::cout << "ft::is_integral<bool>::value:          "
//              << ft::is_integral<bool>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<char>::value:          "
//              << ft::is_integral<char>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<unsigned char>::value: "
//              << ft::is_integral<unsigned char>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<short>::value:         "
//              << ft::is_integral<short>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<int>::value:           "
//              << ft::is_integral<int>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<long>::value:          "
//              << ft::is_integral<long>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<const int>::value:   "
//              << ft::is_integral<const int>::value << " (Expected: true)" << std::endl;
//    std::cout << "ft::is_integral<volatile long>::value: "
//              << ft::is_integral<volatile long>::value << " (Expected: true)" << std::endl;
//
//    // Test 1.2: Non-integral types (should be false)
//    std::cout << "ft::is_integral<float>::value:         "
//              << ft::is_integral<float>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<double>::value:        "
//              << ft::is_integral<double>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<void*>::value:         "
//              << ft::is_integral<void*>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<int*>::value:          "
//              << ft::is_integral<int*>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<std::string>::value:   "
//              << ft::is_integral<std::string>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<TestStruct>::value:    "
//              << ft::is_integral<TestStruct>::value << " (Expected: false)" << std::endl;
//    std::cout << "ft::is_integral<TestStruct&>::value:   "
//              << ft::is_integral<TestStruct&>::value << " (Expected: false)" << std::endl;
//
//    // ==================================================================
//    // Test 2: ft::enable_if (using ft::is_integral)
//    // ==================================================================
//    std::cout << "\n--- Testing ft::enable_if ---" << std::endl;
//    std::cout << "Calling enable_if_test(100):      "
//              << enable_if_test(100) << " (Expected: Integral Type)" << std::endl;
//    std::cout << "Calling enable_if_test('c'):      "
//              << enable_if_test('c') << " (Expected: Integral Type)" << std::endl;
//    std::cout << "Calling enable_if_test(10.5f):    "
//              << enable_if_test(10.5f) << " (Expected: NOT Integral Type)" << std::endl;
//    std::cout << "Calling enable_if_test(TestStruct()): "
//              << enable_if_test(TestStruct()) << " (Expected: NOT Integral Type)" << std::endl;
//
//    // ==================================================================
//    // Test 3: ft::iterator_traits
//    // ==================================================================
//    std::cout << "\n--- Testing ft::iterator_traits ---" << std::endl;
//    std::cout << "(This is tested implicitly by ft::reverse_iterator)" << std::endl;
//    std::cout << "If reverse_iterator compiles and runs, your iterator_traits" << std::endl;
//    std::cout << "specializations for pointers and iterators are likely working." << std::endl;
//
//    // ==================================================================
//    // Test 4: ft::reverse_iterator
//    // ==================================================================
//    std::cout << "\n--- Testing ft::reverse_iterator ---" << std::endl;
//
//    std::vector<int> v;
//    v.push_back(10);
//    v.push_back(20);
//    v.push_back(30);
//    v.push_back(40);
//    v.push_back(50);
//
//    // Note: C++98 requires typedefs inside the class or at function scope.
//    typedef ft::reverse_iterator<std::vector<int>::iterator> RevIter;
//    typedef ft::reverse_iterator<std::vector<int>::const_iterator> ConstRevIter;
//
//    std::cout << "Original vector: 10 20 30 40 50" << std::endl;
//
//    // Test 4.1: Construction and basic iteration
//    RevIter rbegin(v.end());
//    RevIter rend(v.begin());
//
//    std::cout << "Reverse iteration (operator*, operator!=, operator++):" << std::endl;
//    std::cout << "  ";
//    for (RevIter it = rbegin; it != rend; ++it)
//    {
//        std::cout << *it << " "; // Should print: 50 40 30 20 10
//    }
//    std::cout << "(Expected: 50 40 30 20 10)" << std::endl;
//
//    // Test 4.2: Dereferencing
//    std::cout << "*rbegin: " << *rbegin << " (Expected: 50)" << std::endl;
//
//    // Test 4.3: base()
//    // rbegin.base() points one *past* the element it refers to
//    std::cout << "*(rbegin.base() - 1): " << *(rbegin.base() - 1) << " (Expected: 50)" << std::endl;
//    std::cout << "(rend.base() == v.begin()): " << (rend.base() == v.begin()) << " (Expected: true)" << std::endl;
//
//    // Test 4.4: Relational operators
//    std::cout << "(rbegin == rbegin): " << (rbegin == rbegin) << " (Expected: true)" << std::endl;
//    std::cout << "(rbegin != rend): " << (rbegin != rend) << " (Expected: true)" << std::endl;
//    std::cout << "(rbegin < rend): " << (rbegin < rend) << " (Expected: true)" << std::endl;
//    std::cout << "(rbegin > rend): " << (rbegin > rend) << " (Expected: false)" << std::endl;
//    std::cout << "(rbegin <= rend): " << (rbegin <= rend) << " (Expected: true)" << std::endl;
//    std::cout << "(rbegin >= rend): " << (rbegin >= rend) << " (Expected: false)" << std::endl;
//
//    // Test 4.5: Arithmetic operators (+, -, +=, -=)
//    std::cout << "*(rbegin + 2): " << *(rbegin + 2) << " (Expected: 30)" << std::endl;
//
//    RevIter it_arith = rbegin;
//    it_arith++;
//    std::cout << "*it_arith (after ++): " << *it_arith << " (Expected: 40)" << std::endl;
//    it_arith--;
//    std::cout << "*it_arith (after --): " << *it_arith << " (Expected: 50)" << std::endl;
//
//    it_arith += 4;
//    std::cout << "*it_arith (after += 4): " << *it_arith << " (Expected: 10)" << std::endl;
//    it_arith -= 2;
//    std::cout << "*it_arith (after -= 2): " << *it_arith << " (Expected: 30)" << std::endl;
//
//    // Test 4.6: Offset dereference (operator[])
//    std::cout << "rbegin[3]: " << rbegin[3] << " (Expected: 20)" << std::endl;
//
//    // Test 4.7: Difference
//    std::cout << "(rend - rbegin): " << (rend - rbegin) << " (Expected: 5)" << std::endl;
//
//    // Test 4.8: Const iterator compatibility
//    ConstRevIter crbegin(v.end());
//    ConstRevIter crend(v.begin());
//    std::cout << "*crbegin: " << *crbegin << " (Expected: 50)" << std::endl;
//    std::cout << "(crbegin == rbegin): " << (crbegin == rbegin) << " (Expected: true)" << std::endl;
//    std::cout << "(crbegin < rend): " << (crbegin < rend) << " (Expected: true)" << std::endl;
//
//    // Test 4.9: Arrow operator (operator->)
//    struct Point { int x; Point(int v = 0) : x(v) {} };
//    std::vector<Point> points;
//    points.push_back(Point(100));
//    points.push_back(Point(200));
//    points.push_back(Point(300));
//
//    typedef ft::reverse_iterator<std::vector<Point>::iterator> PointRevIter;
//    PointRevIter pr_it(points.end());
//
//    std::cout << "pr_it->x: " << pr_it->x << " (Expected: 300)" << std::endl;
//    (++pr_it);
//    std::cout << "(++pr_it)->x: " << pr_it->x << " (Expected: 200)" << std::endl;
//
//    std::cout << "\n--- Testing Complete ---" << std::endl;
//
//    return 0;

    test_reverse_iterator_behavior();
    test_reverse_iterator();
}