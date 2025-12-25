#include "ft/map.h"
#include "test_config.h"
#include "test_print.h"
#include <cassert>
#include <iostream>
#include <string>

// ============================================================
// Test Cases for erase
// ============================================================

void test_erase_single_element() {
  print_section("erase: Single element tree");

  ft::map<int, std::string> m;
  m.insert(ft::make_pair(10, std::string("ten")));
  assert(m.size() == 1);

  m.erase(10);
  assert(m.size() == 0);
  assert(m.empty());

  std::cout << "Erased single element, size now: " << m.size() << "\n";
  std::cout << "✅ erase single element PASSED\n";
}

void test_erase_leaf_node() {
  print_section("erase: Leaf node");

  ft::map<int, int> m;
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));
  m.insert(ft::make_pair(75, 75));
  m.insert(ft::make_pair(10, 10));

  assert(m.size() == 4);

  // Erase leaf node (10)
  m.erase(10);
  assert(m.size() == 3);

  std::cout << "Erased leaf node 10, size: " << m.size() << "\n";
  std::cout << "✅ erase leaf node PASSED\n";
}

void test_erase_node_with_one_child() {
  print_section("erase: Node with one child");

  ft::map<int, int> m;
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));
  m.insert(ft::make_pair(75, 75));
  m.insert(ft::make_pair(10, 10));
  // Tree:     50
  //          /  \
    //        25    75
  //       /
  //      10

  assert(m.size() == 4);

  // Erase node with one child (25 has left child 10)
  m.erase(25);
  assert(m.size() == 3);

  std::cout << "Erased node 25 (had left child 10), size: " << m.size() << "\n";
  std::cout << "✅ erase node with one child PASSED\n";
}

void test_erase_node_with_two_children() {
  print_section("erase: Node with two children");

  ft::map<int, int> m;
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));
  m.insert(ft::make_pair(75, 75));
  m.insert(ft::make_pair(10, 10));
  m.insert(ft::make_pair(30, 30));
  // Tree:     50
  //          /  \
    //        25    75
  //       /  \
    //      10  30

  assert(m.size() == 5);

  // Erase node with two children (25 has both 10 and 30)
  m.erase(25);
  assert(m.size() == 4);

  std::cout << "Erased node 25 (had two children), size: " << m.size() << "\n";
  std::cout << "✅ erase node with two children PASSED\n";
}

void test_erase_root() {
  print_section("erase: Root node");

  ft::map<int, int> m;
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));
  m.insert(ft::make_pair(75, 75));

  assert(m.size() == 3);

  // Erase root
  m.erase(50);
  assert(m.size() == 2);

  std::cout << "Erased root node 50, size: " << m.size() << "\n";
  std::cout << "✅ erase root node PASSED\n";
}

void test_erase_nonexistent_key() {
  print_section("erase: Non-existent key (should be no-op)");

  ft::map<int, int> m;
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));

  assert(m.size() == 2);

  // Try to erase non-existent key
  m.erase(999);
  assert(m.size() == 2); // Size unchanged

  std::cout << "Tried to erase 999 (doesn't exist), size unchanged: "
            << m.size() << "\n";
  std::cout << "✅ erase non-existent key PASSED\n";
}

void test_erase_all_elements() {
  print_section("erase: All elements one by one");

  ft::map<int, int> m;
  int keys[] = {50, 25, 75, 10, 30, 60, 90};
  for (int i = 0; i < 7; ++i) {
    m.insert(ft::make_pair(keys[i], keys[i]));
  }
  assert(m.size() == 7);

  // Erase all elements
  for (int i = 0; i < 7; ++i) {
    m.erase(keys[i]);
    std::cout << "  After erase(" << keys[i] << "): size=" << m.size() << "\n";
  }

  assert(m.size() == 0);
  assert(m.empty());

  std::cout << "✅ erase all elements PASSED\n";
}

void test_erase_ascending_order() {
  print_section("erase: In ascending order");

  ft::map<int, int> m;
  for (int i = 1; i <= 10; ++i) {
    m.insert(ft::make_pair(i, i * 10));
  }
  assert(m.size() == 10);

  // Erase in ascending order
  for (int i = 1; i <= 10; ++i) {
    m.erase(i);
  }

  assert(m.size() == 0);
  std::cout << "Erased 1-10 in ascending order\n";
  std::cout << "✅ erase ascending order PASSED\n";
}

void test_erase_descending_order() {
  print_section("erase: In descending order");

  ft::map<int, int> m;
  for (int i = 1; i <= 10; ++i) {
    m.insert(ft::make_pair(i, i * 10));
  }
  assert(m.size() == 10);

  // Erase in descending order
  for (int i = 10; i >= 1; --i) {
    m.erase(i);
  }

  assert(m.size() == 0);
  std::cout << "Erased 10-1 in descending order\n";
  std::cout << "✅ erase descending order PASSED\n";
}

void test_erase_insert_mix() {
  print_section("erase: Mixed insert and erase operations");

  ft::map<int, int> m;

  // Insert some
  m.insert(ft::make_pair(50, 50));
  m.insert(ft::make_pair(25, 25));
  m.insert(ft::make_pair(75, 75));
  assert(m.size() == 3);

  // Erase one
  m.erase(25);
  assert(m.size() == 2);

  // Insert more
  m.insert(ft::make_pair(30, 30));
  m.insert(ft::make_pair(20, 20));
  assert(m.size() == 4);

  // Erase root
  m.erase(50);
  assert(m.size() == 3);

  // Insert again at 50
  m.insert(ft::make_pair(50, 500));
  assert(m.size() == 4);

  std::cout << "Mixed operations completed, final size: " << m.size() << "\n";
  std::cout << "✅ erase insert mix PASSED\n";
}

void test_erase_large_dataset() {
  print_section("erase: Large dataset (100 elements)");

  ft::map<int, int> m;

  // Insert 100 elements
  for (int i = 0; i < 100; ++i) {
    m.insert(ft::make_pair(i, i));
  }
  assert(m.size() == 100);

  // Erase all in random-ish order
  for (int i = 0; i < 100; ++i) {
    int key = (i * 37) % 100;
    m.erase(key);
  }

  assert(m.size() == 0);
  std::cout << "Inserted and erased 100 elements\n";
  std::cout << "✅ erase large dataset PASSED\n";
}

// ============================================================
// Main Test Function
// ============================================================

void test_map_erase() {
  FILE_BANNER();

  test_erase_single_element();
  test_erase_leaf_node();
  test_erase_node_with_one_child();
  test_erase_node_with_two_children();
  test_erase_root();
  test_erase_nonexistent_key();
  test_erase_all_elements();
  test_erase_ascending_order();
  test_erase_descending_order();
  test_erase_insert_mix();
  test_erase_large_dataset();

  print_section("ALL ERASE TESTS COMPLETED");
  std::cout << "✅ All erase function tests passed!\n\n";
}
