#include "test_config.h"
#include "test_print.h"
#include <cassert>
#include <iostream>
#include <string>

// ============================================================
// Standalone Node and find function for testing
// (Adapted from map.h for isolated testing)
// ============================================================

namespace test_find {

struct Node {
  int key;
  std::string value;
  Node *parent;
  Node *left;
  Node *right;
  bool red;

  Node(int k, const std::string &v = "")
      : key(k), value(v), parent(nullptr), left(nullptr), right(nullptr),
        red(true) {}
};

// find function: 키를 찾거나, 삽입될 위치의 부모 노드를 반환
// - 키를 찾으면: 해당 노드 반환
// - 키가 없으면: 삽입될 위치의 부모 노드 반환 (삽입 용도)
//
// 원본 map.h의 find는 "찾거나 삽입 위치"를 반환하도록 설계됨
// 수정된 버전에서는 찾으면 node, 못찾으면 nullptr 반환

// Version 1: 삽입 위치 찾기용 (부모 노드 반환)
Node *find_insert_position(int key_to_find, Node *node) {
  if (node == nullptr) {
    return nullptr;
  }

  int key = node->key;

  if (key > key_to_find) {
    if (node->left != nullptr) {
      return find_insert_position(key_to_find, node->left);
    }
    // left가 nullptr이면 여기가 삽입 위치, 현재 node가 부모가 됨
    return node;
  } else if (key < key_to_find) {
    if (node->right != nullptr) {
      return find_insert_position(key_to_find, node->right);
    }
    // right가 nullptr이면 여기가 삽입 위치, 현재 node가 부모가 됨
    return node;
  }
  // key == key_to_find: 이미 존재하는 키
  return node;
}

// Version 2: 순수 검색용 (찾으면 노드, 없으면 nullptr)
Node *find_exact(int key_to_find, Node *node) {
  if (node == nullptr) {
    return nullptr;
  }

  int key = node->key;

  if (key > key_to_find) {
    if (node->left != nullptr) {
      return find_exact(key_to_find, node->left);
    }
    return nullptr; // 없음
  } else if (key < key_to_find) {
    if (node->right != nullptr) {
      return find_exact(key_to_find, node->right);
    }
    return nullptr; // 없음
  }
  // key == key_to_find
  return node;
}

// ============================================================
// Test Utilities
// ============================================================

void print_tree_inorder(Node *n) {
  if (n == nullptr)
    return;
  print_tree_inorder(n->left);
  std::cout << n->key << " ";
  print_tree_inorder(n->right);
}

// Helper to build a sample BST:
//           20
//          /  \
//        10    30
//       /  \     \
//      5   15    40
Node *build_sample_tree() {
  Node *n20 = new Node(20, "twenty");
  Node *n10 = new Node(10, "ten");
  Node *n30 = new Node(30, "thirty");
  Node *n5 = new Node(5, "five");
  Node *n15 = new Node(15, "fifteen");
  Node *n40 = new Node(40, "forty");

  n20->left = n10;
  n20->right = n30;
  n10->parent = n20;
  n30->parent = n20;

  n10->left = n5;
  n10->right = n15;
  n5->parent = n10;
  n15->parent = n10;

  n30->right = n40;
  n40->parent = n30;

  return n20; // root
}

void cleanup_tree(Node *n) {
  if (n == nullptr)
    return;
  cleanup_tree(n->left);
  cleanup_tree(n->right);
  delete n;
}

} // namespace test_find

// ============================================================
// Test Cases for find_insert_position
// ============================================================

void test_find_insert_existing_key() {
  using namespace test_find;
  print_section("find_insert_position: Find existing key");

  Node *root = build_sample_tree();
  std::cout << "Tree inorder: ";
  print_tree_inorder(root);
  std::cout << "\n\n";

  // Find existing keys
  Node *result;

  result = find_insert_position(20, root);
  std::cout << "find(20) = " << (result ? std::to_string(result->key) : "null")
            << "\n";
  assert(result && result->key == 20);

  result = find_insert_position(10, root);
  std::cout << "find(10) = " << (result ? std::to_string(result->key) : "null")
            << "\n";
  assert(result && result->key == 10);

  result = find_insert_position(30, root);
  std::cout << "find(30) = " << (result ? std::to_string(result->key) : "null")
            << "\n";
  assert(result && result->key == 30);

  result = find_insert_position(5, root);
  std::cout << "find(5) = " << (result ? std::to_string(result->key) : "null")
            << "\n";
  assert(result && result->key == 5);

  result = find_insert_position(40, root);
  std::cout << "find(40) = " << (result ? std::to_string(result->key) : "null")
            << "\n";
  assert(result && result->key == 40);

  std::cout << "\n✅ find existing keys PASSED\n";
  cleanup_tree(root);
}

void test_find_insert_position_for_new_key() {
  using namespace test_find;
  print_section("find_insert_position: Find position for new key");

  Node *root = build_sample_tree();
  //           20
  //          /  \
    //        10    30
  //       /  \     \
    //      5   15    40

  std::cout << "Tree: 5 10 15 20 30 40\n\n";

  Node *result;

  // 1을 삽입하려면 -> 5의 left로 가야 함 -> parent는 5
  result = find_insert_position(1, root);
  std::cout << "find(1) for insert -> parent should be 5: " << result->key
            << "\n";
  assert(result->key == 5);

  // 7을 삽입하려면 -> 5의 right로 가야 함 -> parent는 5
  result = find_insert_position(7, root);
  std::cout << "find(7) for insert -> parent should be 5: " << result->key
            << "\n";
  assert(result->key == 5);

  // 12를 삽입하려면 -> 15의 left로 가야 함 -> parent는 15
  result = find_insert_position(12, root);
  std::cout << "find(12) for insert -> parent should be 15: " << result->key
            << "\n";
  assert(result->key == 15);

  // 25를 삽입하려면 -> 30의 left로 가야 함 -> parent는 30
  result = find_insert_position(25, root);
  std::cout << "find(25) for insert -> parent should be 30: " << result->key
            << "\n";
  assert(result->key == 30);

  // 50을 삽입하려면 -> 40의 right로 가야 함 -> parent는 40
  result = find_insert_position(50, root);
  std::cout << "find(50) for insert -> parent should be 40: " << result->key
            << "\n";
  assert(result->key == 40);

  std::cout << "\n✅ find position for new keys PASSED\n";
  cleanup_tree(root);
}

void test_find_insert_empty_tree() {
  using namespace test_find;
  print_section("find_insert_position: Empty tree (nullptr root)");

  Node *result = find_insert_position(10, nullptr);
  std::cout << "find(10) on nullptr = "
            << (result ? std::to_string(result->key) : "nullptr") << "\n";
  assert(result == nullptr);

  std::cout << "\n✅ find on empty tree PASSED\n";
}

void test_find_insert_single_node() {
  using namespace test_find;
  print_section("find_insert_position: Single node tree");

  Node *root = new Node(50, "fifty");

  Node *result;

  // 자기 자신 찾기
  result = find_insert_position(50, root);
  std::cout << "find(50) = " << result->key << " (self)\n";
  assert(result->key == 50);

  // 작은 값 삽입 위치 (left)
  result = find_insert_position(25, root);
  std::cout << "find(25) -> parent = " << result->key << " (should be 50)\n";
  assert(result->key == 50);

  // 큰 값 삽입 위치 (right)
  result = find_insert_position(75, root);
  std::cout << "find(75) -> parent = " << result->key << " (should be 50)\n";
  assert(result->key == 50);

  std::cout << "\n✅ find on single node tree PASSED\n";
  delete root;
}

// ============================================================
// Test Cases for find_exact (순수 검색)
// ============================================================

void test_find_exact_existing() {
  using namespace test_find;
  print_section("find_exact: Find existing keys");

  Node *root = build_sample_tree();

  Node *result;

  result = find_exact(20, root);
  assert(result && result->key == 20);
  std::cout << "find_exact(20) = " << result->key << " ✓\n";

  result = find_exact(5, root);
  assert(result && result->key == 5);
  std::cout << "find_exact(5) = " << result->key << " ✓\n";

  result = find_exact(40, root);
  assert(result && result->key == 40);
  std::cout << "find_exact(40) = " << result->key << " ✓\n";

  std::cout << "\n✅ find_exact existing PASSED\n";
  cleanup_tree(root);
}

void test_find_exact_not_found() {
  using namespace test_find;
  print_section("find_exact: Key not found returns nullptr");

  Node *root = build_sample_tree();

  Node *result;

  result = find_exact(1, root);
  std::cout << "find_exact(1) = "
            << (result ? std::to_string(result->key) : "nullptr") << "\n";
  assert(result == nullptr);

  result = find_exact(25, root);
  std::cout << "find_exact(25) = "
            << (result ? std::to_string(result->key) : "nullptr") << "\n";
  assert(result == nullptr);

  result = find_exact(100, root);
  std::cout << "find_exact(100) = "
            << (result ? std::to_string(result->key) : "nullptr") << "\n";
  assert(result == nullptr);

  std::cout << "\n✅ find_exact not found PASSED\n";
  cleanup_tree(root);
}

// ============================================================
// Test: BST traversal path verification
// ============================================================

void test_find_traversal_path() {
  using namespace test_find;
  print_section("find: Verify correct BST traversal path");

  Node *root = build_sample_tree();
  //           20
  //          /  \
    //        10    30
  //       /  \     \
    //      5   15    40

  std::cout << "Finding 15 should go: 20 -> 10 -> 15\n";
  std::cout << "Finding 40 should go: 20 -> 30 -> 40\n";
  std::cout << "Finding 5 should go: 20 -> 10 -> 5\n\n";

  // 각 노드 찾기가 올바른 결과를 반환하는지 확인
  assert(find_exact(15, root)->key == 15);
  assert(find_exact(40, root)->key == 40);
  assert(find_exact(5, root)->key == 5);

  // 잘못된 경로로 가면 안됨
  // 예: 40을 왼쪽 서브트리에서 찾으면 안됨
  assert(find_exact(40, root->left) == nullptr);

  // 5는 오른쪽 서브트리에 없음
  assert(find_exact(5, root->right) == nullptr);

  std::cout << "✅ BST traversal path verification PASSED\n";
  cleanup_tree(root);
}

// ============================================================
// Test: Deep tree
// ============================================================

void test_find_deep_tree() {
  using namespace test_find;
  print_section("find: Deep unbalanced tree");

  // 1 -> 2 -> 3 -> 4 -> 5 (right-skewed)
  Node *n1 = new Node(1);
  Node *n2 = new Node(2);
  Node *n3 = new Node(3);
  Node *n4 = new Node(4);
  Node *n5 = new Node(5);

  n1->right = n2;
  n2->parent = n1;
  n2->right = n3;
  n3->parent = n2;
  n3->right = n4;
  n4->parent = n3;
  n4->right = n5;
  n5->parent = n4;

  std::cout << "Right-skewed tree: 1 -> 2 -> 3 -> 4 -> 5\n\n";

  assert(find_exact(1, n1)->key == 1);
  std::cout << "find(1) ✓\n";
  assert(find_exact(3, n1)->key == 3);
  std::cout << "find(3) ✓\n";
  assert(find_exact(5, n1)->key == 5);
  std::cout << "find(5) ✓\n";
  assert(find_exact(6, n1) == nullptr);
  std::cout << "find(6) = nullptr ✓\n";
  assert(find_exact(0, n1) == nullptr);
  std::cout << "find(0) = nullptr ✓\n";

  std::cout << "\n✅ deep tree find PASSED\n";

  // cleanup
  delete n5;
  delete n4;
  delete n3;
  delete n2;
  delete n1;
}

// ============================================================
// Main Test Function
// ============================================================

void test_map_find() {
  FILE_BANNER();

  // find_insert_position tests
  test_find_insert_existing_key();
  test_find_insert_position_for_new_key();
  test_find_insert_empty_tree();
  test_find_insert_single_node();

  // find_exact tests
  test_find_exact_existing();
  test_find_exact_not_found();

  // Traversal and deep tree tests
  test_find_traversal_path();
  test_find_deep_tree();

  print_section("ALL FIND TESTS COMPLETED");
  std::cout << "✅ All find function tests passed!\n\n";
}
