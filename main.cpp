#include <iostream>
#include <string>
#include <utility> // std::pair를 위해 필요
#include "utility.h"  // 직접 구현한 pair.h

// 결과 출력용 함수
void print_result(const std::string& test_name, bool ft_res, bool std_res) {
    std::cout << "[ " << test_name << " ]\n";
    std::cout << "ft  : " << (ft_res ? "true" : "false") << "\n";
    std::cout << "std : " << (std_res ? "true" : "false") << "\n";
    std::cout << "=> " << (ft_res == std_res ? "\033[32mOK\033[0m" : "\033[31mFAIL\033[0m") << "\n\n";
}

int main() {
    std::cout << "--- ft::pair와 std::pair 동작 비교 테스트 ---\n\n";

    // 1. 생성자 테스트
    std::cout << "========== 1. 생성자 테스트 ==========\n";
    // 기본 생성자
    ft::pair<int, std::string> ft_p1;
    std::pair<int, std::string> std_p1;
    print_result("기본 생성자 (int)", ft_p1.first == 0, std_p1.first == 0);
    print_result("기본 생성자 (string)", ft_p1.second == "", std_p1.second == "");

    // 값 초기화 생성자
    ft::pair<int, std::string> ft_p2(42, "hello");
    std::pair<int, std::string> std_p2(42, "hello");
    print_result("값 초기화 생성자 (first)", ft_p2.first == 42, std_p2.first == 42);
    print_result("값 초기화 생성자 (second)", ft_p2.second == "hello", std_p2.second == "hello");

    // 복사 생성자
    ft::pair<int, std::string> ft_p3(ft_p2);
    std::pair<int, std::string> std_p3(std_p2);
    print_result("복사 생성자 (first)", ft_p3.first == ft_p2.first, std_p3.first == std_p2.first);
    print_result("복사 생성자 (second)", ft_p3.second == ft_p2.second, std_p3.second == std_p2.second);


    // 2. 대입 연산자 테스트
    std::cout << "========== 2. 대입 연산자 테스트 ==========\n";
    ft::pair<int, std::string> ft_p4;
    std::pair<int, std::string> std_p4;
    ft_p4 = ft_p2;
    std_p4 = std_p2;
    print_result("대입 연산자", (ft_p4.first == ft_p2.first && ft_p4.second == ft_p2.second),
                 (std_p4.first == std_p2.first && std_p4.second == std_p2.second));


    // 3. 비교 연산자 테스트
    std::cout << "========== 3. 비교 연산자 테스트 ==========\n";
    ft::pair<int, int> ft_comp1(10, 20);
    ft::pair<int, int> ft_comp2(10, 20); // same
    ft::pair<int, int> ft_comp3(20, 10); // first is greater
    ft::pair<int, int> ft_comp4(10, 30); // second is greater

    std::pair<int, int> std_comp1(10, 20);
    std::pair<int, int> std_comp2(10, 20); // same
    std::pair<int, int> std_comp3(20, 10); // first is greater
    std::pair<int, int> std_comp4(10, 30); // second is greater

    // ==
    print_result("p1 == p2", ft_comp1 == ft_comp2, std_comp1 == std_comp2);
    print_result("p1 == p3", ft_comp1 == ft_comp3, std_comp1 == std_comp3);

    // !=
    print_result("p1 != p2", ft_comp1 != ft_comp2, std_comp1 != std_comp2);
    print_result("p1 != p3", ft_comp1 != ft_comp3, std_comp1 != std_comp3);

    // <
    print_result("p1 < p3 (first big)", ft_comp1 < ft_comp3, std_comp1 < std_comp3);
    print_result("p1 < p4 (second big)", ft_comp1 < ft_comp4, std_comp1 < std_comp4);
    print_result("p1 < p2 (same)", ft_comp1 < ft_comp2, std_comp1 < std_comp2);

    // <=
    print_result("p1 <= p2 (same)", ft_comp1 <= ft_comp2, std_comp1 <= std_comp2);
    print_result("p1 <= p4 (second big)", ft_comp1 <= ft_comp4, std_comp1 <= std_comp4);
    print_result("p3 <= p1 (first small)", ft_comp3 <= ft_comp1, std_comp3 <= std_comp1);

    // >
    print_result("p3 > p1 (first big)", ft_comp3 > ft_comp1, std_comp3 > std_comp1);
    print_result("p4 > p1 (second big)", ft_comp4 > ft_comp1, std_comp4 > std_comp1);
    print_result("p1 > p2 (same)", ft_comp1 > ft_comp2, std_comp1 > std_comp2);

    // >=
    print_result("p1 >= p2 (same)", ft_comp1 >= ft_comp2, std_comp1 >= std_comp2);
    print_result("p3 >= p1 (first big)", ft_comp3 >= ft_comp1, std_comp3 >= std_comp1);
    print_result("p1 >= p4 (second small)", ft_comp1 >= ft_comp4, std_comp1 >= std_comp4);


    return 0;
}