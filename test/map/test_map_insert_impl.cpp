#include "ft/map.h"
#include "test_config.h"
#include "test_print.h"
#include <cassert>
#include <iostream>
#include <string>

// ============================================================
// Test Cases
// ============================================================

void test_insert_empty_tree() {
  print_section("insert: First element into empty tree");

  ft::map<int, std::string> m;
  assert(m.empty());
  assert(m.size() == 0);

  auto result = m.insert(ft::make_pair(10, std::string("ten")));

  assert(result.second == true); // insertion succeeded
  assert(m.size() == 1);
  assert(!m.empty());

  std::cout << "Inserted (10, \"ten\") into empty tree\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert into empty tree PASSED\n";
}

void test_insert_multiple_elements() {
  print_section("insert: Multiple elements");

  ft::map<int, std::string> m;

  // Insert several elements
  m.insert(ft::make_pair(50, std::string("fifty")));
  m.insert(ft::make_pair(25, std::string("twenty-five")));
  m.insert(ft::make_pair(75, std::string("seventy-five")));
  m.insert(ft::make_pair(10, std::string("ten")));
  m.insert(ft::make_pair(30, std::string("thirty")));
  m.insert(ft::make_pair(60, std::string("sixty")));
  m.insert(ft::make_pair(90, std::string("ninety")));

  assert(m.size() == 7);
  std::cout << "Inserted 7 elements: 50, 25, 75, 10, 30, 60, 90\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert multiple elements PASSED\n";
}

void test_insert_duplicate_key() {
  print_section("insert: Duplicate key should not be inserted");

  ft::map<int, std::string> m;

  auto r1 = m.insert(ft::make_pair(10, std::string("original")));
  assert(r1.second == true);
  assert(m.size() == 1);

  auto r2 = m.insert(ft::make_pair(10, std::string("duplicate")));
  assert(r2.second == false); // Duplicate not inserted
  assert(m.size() == 1);      // Size unchanged

  std::cout << "First insert(10): success=" << r1.second << "\n";
  std::cout << "Second insert(10): success=" << r2.second << "\n";
  std::cout << "Size after duplicate attempt: " << m.size() << "\n";
  std::cout << "✅ insert duplicate key PASSED\n";
}

void test_insert_ascending_order() {
  print_section("insert: Ascending order (triggers rotations)");

  ft::map<int, int> m;

  // Ascending order insertion triggers many left rotations
  for (int i = 1; i <= 10; ++i) {
    m.insert(ft::make_pair(i, i * 10));
  }

  assert(m.size() == 10);
  std::cout << "Inserted 1-10 in ascending order\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert ascending order PASSED\n";
}

void test_insert_descending_order() {
  print_section("insert: Descending order (triggers rotations)");

  ft::map<int, int> m;

  // Descending order insertion triggers many right rotations
  for (int i = 10; i >= 1; --i) {
    m.insert(ft::make_pair(i, i * 10));
  }

  assert(m.size() == 10);
  std::cout << "Inserted 10-1 in descending order\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert descending order PASSED\n";
}

void test_insert_zigzag_pattern() {
  print_section("insert: Zigzag pattern (triggers double rotations)");

  ft::map<int, int> m;

  // Zigzag pattern: 1, 10, 2, 9, 3, 8, 4, 7, 5, 6
  int values[] = {1, 10, 2, 9, 3, 8, 4, 7, 5, 6};
  for (int i = 0; i < 10; ++i) {
    m.insert(ft::make_pair(values[i], values[i]));
  }

  assert(m.size() == 10);
  std::cout << "Inserted zigzag pattern: 1,10,2,9,3,8,4,7,5,6\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert zigzag pattern PASSED\n";
}

void test_insert_large_dataset() {
  print_section("insert: Large dataset (100 elements)");

  ft::map<int, int> m;

  // Insert 100 elements
  for (int i = 0; i < 100; ++i) {
    // Interleaved pattern to stress test
    int key = (i * 37) % 100; // Pseudo-random-ish order
    m.insert(ft::make_pair(key, i));
  }

  assert(m.size() == 100);
  std::cout << "Inserted 100 elements in pseudo-random order\n";
  std::cout << "Size: " << m.size() << "\n";
  std::cout << "✅ insert large dataset PASSED\n";
}

// ============================================================
// Main Test Function
// ============================================================

void test_map_insert() {
  FILE_BANNER();

  test_insert_empty_tree();
  test_insert_multiple_elements();
  test_insert_duplicate_key();
  test_insert_ascending_order();
  test_insert_descending_order();
  test_insert_zigzag_pattern();
  test_insert_large_dataset();

  print_section("ALL INSERT TESTS COMPLETED");
  std::cout << "✅ All insert function tests passed!\n\n";
}
