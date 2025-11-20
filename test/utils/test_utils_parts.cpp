// test_utils_parts.cpp
#include "test_print.h"

#ifndef TEST_UTILS_PART_1
    #define TEST_UTILS_PART_1 0
#endif
#ifndef TEST_UTILS_PART_2
    #define TEST_UTILS_PART_2 0
#endif

#if TEST_UTILS_PART_1
void test_utils_pair();
void test_utils_equal();
void test_utils_lexicographical_compare();
#endif

#if TEST_UTILS_PART_2
void test_utils_enable_if();
void test_utils_is_integral();
void test_utils_iterator_traits();
void test_utils_reverse_iterator();
#endif

void test_utils_part_1()
{
#if TEST_UTILS_PART_1
    print_section("Utils Part 1 — pair / equal / lexicographical_compare");
    test_utils_pair();
    test_utils_equal();
    test_utils_lexicographical_compare();
#else
    (void)0; // no-op
#endif
}

void test_utils_part_2()
{
#if TEST_UTILS_PART_2
    print_section("Utils Part 2 — enable_if / is_integral / iterator_traits / reverse_iterator");
    test_utils_enable_if();
    test_utils_is_integral();
    test_utils_iterator_traits();
    test_utils_reverse_iterator();
#else
    (void)0; // no-op
#endif
}
