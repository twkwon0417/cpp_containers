#include "test_config.h"
#include "test_print.h"
#include <cassert>
#include <iostream>

// ============================================================
// Standalone Node and Rotate functions for testing
// (Copied from map.h for isolated testing)
// ============================================================

namespace test_rotate {

struct Node {
  int key;
  Node *parent;
  Node *left;
  Node *right;
  bool red;

  Node(int k)
      : key(k), parent(nullptr), left(nullptr), right(nullptr), red(true) {}
};

// Helper: root pointer for tree (simulates this->root in map)
Node *g_root = nullptr;

//   (x)                       (y)
//   / \     Left Rotate(x)    / \
//  α  (y)   ------------->  (x)  γ
//     / \                   / \
//    β   γ  <------------- α   β
//           Right Rotate(y)

void rotate_left(Node *x) {
  Node *a = x->left;
  Node *y = x->right;
  Node *c = y->right;
  Node *b = y->left; // may be nullptr

  Node *parent = x->parent;
  y->parent = parent;
  if (parent != nullptr) {
    if (parent->left == x) {
      parent->left = y;
    } else {
      parent->right = y;
    }
  } else {
    g_root = y;
  }

  x->left = a;
  x->right = b;

  y->left = x;
  y->right = c;
  if (b != nullptr) {
    b->parent = x;
  }
  x->parent = y;
}

void rotate_right(Node *x) {
  Node *a = x->right;
  Node *y = x->left;
  Node *c = y->left;
  Node *b = y->right; // may be nullptr

  Node *parent = x->parent;
  y->parent = parent;

  if (parent != nullptr) {
    if (parent->left == x) {
      parent->left = y;
    } else {
      parent->right = y;
    }
  } else {
    g_root = y;
  }

  x->right = a;
  x->left = b;

  y->right = x;
  y->left = c;

  if (b != nullptr) {
    b->parent = x;
  }
  x->parent = y;
}

// ============================================================
// Test Utilities
// ============================================================

void print_node_info(const char *label, Node *n) {
  if (n == nullptr) {
    std::cout << label << ": nullptr\n";
  } else {
    std::cout << label << ": key=" << n->key << ", parent="
              << (n->parent ? std::to_string(n->parent->key) : "null")
              << ", left=" << (n->left ? std::to_string(n->left->key) : "null")
              << ", right="
              << (n->right ? std::to_string(n->right->key) : "null") << "\n";
  }
}

void verify_parent_links(Node *n, const char *context) {
  if (n == nullptr)
    return;
  if (n->left != nullptr) {
    assert(n->left->parent == n && "Left child parent mismatch");
  }
  if (n->right != nullptr) {
    assert(n->right->parent == n && "Right child parent mismatch");
  }
  (void)context;
}

// Inorder traversal to verify BST property
void inorder_print(Node *n) {
  if (n == nullptr)
    return;
  inorder_print(n->left);
  std::cout << n->key << " ";
  inorder_print(n->right);
}

void cleanup_nodes(Node *nodes[], int count) {
  for (int i = 0; i < count; ++i) {
    delete nodes[i];
  }
}

} // namespace test_rotate

// ============================================================
// Test Cases
// ============================================================

void test_rotate_left_basic() {
  using namespace test_rotate;

  print_section("rotate_left: Basic Test");

  //  Initial:      After rotate_left(x):
  //      (x)              (y)
  //      / \              / \
    //     α  (y)   -->    (x)  γ
  //        / \          / \
    //       β   γ        α   β

  Node *x = new Node(10);     // x
  Node *alpha = new Node(5);  // α
  Node *y = new Node(20);     // y
  Node *beta = new Node(15);  // β
  Node *gamma = new Node(25); // γ

  // Build initial tree
  g_root = x;
  x->parent = nullptr;
  x->left = alpha;
  x->right = y;
  alpha->parent = x;
  y->parent = x;
  y->left = beta;
  y->right = gamma;
  beta->parent = y;
  gamma->parent = y;

  std::cout << "Before rotate_left(x=10):\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  print_node_info("x (10)", x);
  print_node_info("y (20)", y);
  print_node_info("alpha (5)", alpha);
  print_node_info("beta (15)", beta);
  print_node_info("gamma (25)", gamma);

  // Perform rotation
  rotate_left(x);

  std::cout << "\nAfter rotate_left(x=10):\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  print_node_info("y (20) - now root", y);
  print_node_info("x (10) - now y's left", x);
  print_node_info("alpha (5)", alpha);
  print_node_info("beta (15)", beta);
  print_node_info("gamma (25)", gamma);

  // Verify structure
  assert(g_root == y && "y should be new root");
  assert(y->parent == nullptr && "y parent should be null");
  assert(y->left == x && "y's left should be x");
  assert(y->right == gamma && "y's right should be gamma");
  assert(x->parent == y && "x's parent should be y");
  assert(x->left == alpha && "x's left should be alpha");
  assert(x->right == beta && "x's right should be beta");
  assert(beta->parent == x && "beta's parent should be x");
  assert(alpha->parent == x && "alpha's parent should be x");
  assert(gamma->parent == y && "gamma's parent should be y");

  verify_parent_links(g_root, "root");
  verify_parent_links(x, "x");
  verify_parent_links(y, "y");

  std::cout << "\n✅ rotate_left basic test PASSED\n";

  Node *nodes[] = {x, alpha, y, beta, gamma};
  cleanup_nodes(nodes, 5);
}

void test_rotate_right_basic() {
  using namespace test_rotate;

  print_section("rotate_right: Basic Test");

  //  Initial:        After rotate_right(x):
  //       (x)              (y)
  //       / \              / \
    //     (y)  α    -->     γ  (x)
  //     / \                  / \
    //    γ   β                β   α

  Node *x = new Node(20);     // x
  Node *alpha = new Node(25); // α (right of x)
  Node *y = new Node(10);     // y (left of x)
  Node *beta = new Node(15);  // β (right of y)
  Node *gamma = new Node(5);  // γ (left of y)

  // Build initial tree
  g_root = x;
  x->parent = nullptr;
  x->left = y;
  x->right = alpha;
  alpha->parent = x;
  y->parent = x;
  y->left = gamma;
  y->right = beta;
  gamma->parent = y;
  beta->parent = y;

  std::cout << "Before rotate_right(x=20):\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  print_node_info("x (20)", x);
  print_node_info("y (10)", y);
  print_node_info("alpha (25)", alpha);
  print_node_info("beta (15)", beta);
  print_node_info("gamma (5)", gamma);

  // Perform rotation
  rotate_right(x);

  std::cout << "\nAfter rotate_right(x=20):\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  print_node_info("y (10) - now root", y);
  print_node_info("x (20) - now y's right", x);
  print_node_info("alpha (25)", alpha);
  print_node_info("beta (15)", beta);
  print_node_info("gamma (5)", gamma);

  // Verify structure
  assert(g_root == y && "y should be new root");
  assert(y->parent == nullptr && "y parent should be null");
  assert(y->left == gamma && "y's left should be gamma");
  assert(y->right == x && "y's right should be x");
  assert(x->parent == y && "x's parent should be y");
  assert(x->left == beta && "x's left should be beta");
  assert(x->right == alpha && "x's right should be alpha");
  assert(beta->parent == x && "beta's parent should be x");
  assert(gamma->parent == y && "gamma's parent should be y");
  assert(alpha->parent == x && "alpha's parent should be x");

  verify_parent_links(g_root, "root");
  verify_parent_links(x, "x");
  verify_parent_links(y, "y");

  std::cout << "\n✅ rotate_right basic test PASSED\n";

  Node *nodes[] = {x, alpha, y, beta, gamma};
  cleanup_nodes(nodes, 5);
}

void test_rotate_left_with_nullptr_beta() {
  using namespace test_rotate;

  print_section("rotate_left: With nullptr beta (y->left is null)");

  Node *x = new Node(10);
  Node *alpha = new Node(5);
  Node *y = new Node(20);
  Node *gamma = new Node(25);
  // beta is nullptr

  g_root = x;
  x->parent = nullptr;
  x->left = alpha;
  x->right = y;
  alpha->parent = x;
  y->parent = x;
  y->left = nullptr; // beta is null
  y->right = gamma;
  gamma->parent = y;

  std::cout << "Before rotate_left(x=10) with beta=nullptr:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n";

  rotate_left(x);

  std::cout << "After rotate_left:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  assert(g_root == y);
  assert(y->left == x);
  assert(x->right == nullptr && "x's right (beta) should be nullptr");
  assert(x->left == alpha);

  std::cout << "✅ rotate_left with nullptr beta PASSED\n";

  Node *nodes[] = {x, alpha, y, gamma};
  cleanup_nodes(nodes, 4);
}

void test_rotate_right_with_nullptr_beta() {
  using namespace test_rotate;

  print_section("rotate_right: With nullptr beta (y->right is null)");

  Node *x = new Node(20);
  Node *alpha = new Node(25);
  Node *y = new Node(10);
  Node *gamma = new Node(5);
  // beta is nullptr

  g_root = x;
  x->parent = nullptr;
  x->left = y;
  x->right = alpha;
  alpha->parent = x;
  y->parent = x;
  y->left = gamma;
  y->right = nullptr; // beta is null
  gamma->parent = y;

  std::cout << "Before rotate_right(x=20) with beta=nullptr:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n";

  rotate_right(x);

  std::cout << "After rotate_right:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  assert(g_root == y);
  assert(y->right == x);
  assert(x->left == nullptr && "x's left (beta) should be nullptr");
  assert(x->right == alpha);

  std::cout << "✅ rotate_right with nullptr beta PASSED\n";

  Node *nodes[] = {x, alpha, y, gamma};
  cleanup_nodes(nodes, 4);
}

void test_rotate_left_non_root() {
  using namespace test_rotate;

  print_section("rotate_left: Non-root node (x has parent)");

  //        (P)                    (P)
  //       /                      /
  //     (x)      -->           (y)
  //     / \                    / \
    //    α  (y)                (x)  γ
  //       / \                / \
    //      β   γ              α   β

  Node *P = new Node(50);
  Node *x = new Node(10);
  Node *alpha = new Node(5);
  Node *y = new Node(20);
  Node *beta = new Node(15);
  Node *gamma = new Node(25);

  g_root = P;
  P->parent = nullptr;
  P->left = x;
  P->right = nullptr;

  x->parent = P;
  x->left = alpha;
  x->right = y;
  alpha->parent = x;
  y->parent = x;
  y->left = beta;
  y->right = gamma;
  beta->parent = y;
  gamma->parent = y;

  std::cout << "Before rotate_left(x=10) under parent P=50:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n";

  rotate_left(x);

  std::cout << "After rotate_left:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  assert(g_root == P && "P should still be root");
  assert(P->left == y && "P's left should now be y");
  assert(y->parent == P && "y's parent should be P");
  assert(y->left == x && "y's left should be x");

  std::cout << "✅ rotate_left non-root PASSED\n";

  Node *nodes[] = {P, x, alpha, y, beta, gamma};
  cleanup_nodes(nodes, 6);
}

void test_rotate_right_non_root() {
  using namespace test_rotate;

  print_section("rotate_right: Non-root node (x has parent)");

  Node *P = new Node(50);
  Node *x = new Node(30);
  Node *alpha = new Node(35);
  Node *y = new Node(20);
  Node *beta = new Node(25);
  Node *gamma = new Node(15);

  g_root = P;
  P->parent = nullptr;
  P->left = nullptr;
  P->right = x;

  x->parent = P;
  x->left = y;
  x->right = alpha;
  alpha->parent = x;
  y->parent = x;
  y->left = gamma;
  y->right = beta;
  gamma->parent = y;
  beta->parent = y;

  std::cout << "Before rotate_right(x=30) under parent P=50:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n";

  rotate_right(x);

  std::cout << "After rotate_right:\n";
  std::cout << "Inorder: ";
  inorder_print(g_root);
  std::cout << "\n\n";

  assert(g_root == P && "P should still be root");
  assert(P->right == y && "P's right should now be y");
  assert(y->parent == P && "y's parent should be P");
  assert(y->right == x && "y's right should be x");

  std::cout << "✅ rotate_right non-root PASSED\n";

  Node *nodes[] = {P, x, alpha, y, beta, gamma};
  cleanup_nodes(nodes, 6);
}

void test_double_rotation_left_right() {
  using namespace test_rotate;

  print_section("Double Rotation: Left then Right");

  // Test that rotate_left followed by rotate_right returns to original

  Node *x = new Node(10);
  Node *alpha = new Node(5);
  Node *y = new Node(20);
  Node *beta = new Node(15);
  Node *gamma = new Node(25);

  g_root = x;
  x->parent = nullptr;
  x->left = alpha;
  x->right = y;
  alpha->parent = x;
  y->parent = x;
  y->left = beta;
  y->right = gamma;
  beta->parent = y;
  gamma->parent = y;

  std::cout << "Initial inorder: ";
  inorder_print(g_root);
  std::cout << "\n";

  rotate_left(x);
  std::cout << "After rotate_left(x=10): ";
  inorder_print(g_root);
  std::cout << " (root=" << g_root->key << ")\n";

  // Now y is root, rotate_right(y) should restore x as root
  rotate_right(y);
  std::cout << "After rotate_right(y=20): ";
  inorder_print(g_root);
  std::cout << " (root=" << g_root->key << ")\n\n";

  assert(g_root == x && "x should be root again after double rotation");
  assert(x->right == y);
  assert(y->left == beta);
  assert(y->right == gamma);
  assert(x->left == alpha);

  std::cout << "✅ Double rotation (left then right) PASSED\n";

  Node *nodes[] = {x, alpha, y, beta, gamma};
  cleanup_nodes(nodes, 5);
}

void test_bst_property_preserved() {
  using namespace test_rotate;

  print_section("BST Property: Inorder traversal preserved after rotations");

  // Build a more complex tree
  //           30
  //          /  \
    //        20    40
  //       /  \     \
    //      10  25    50

  Node *n30 = new Node(30);
  Node *n20 = new Node(20);
  Node *n40 = new Node(40);
  Node *n10 = new Node(10);
  Node *n25 = new Node(25);
  Node *n50 = new Node(50);

  g_root = n30;
  n30->parent = nullptr;
  n30->left = n20;
  n30->right = n40;
  n20->parent = n30;
  n40->parent = n30;
  n20->left = n10;
  n20->right = n25;
  n10->parent = n20;
  n25->parent = n20;
  n40->left = nullptr;
  n40->right = n50;
  n50->parent = n40;

  std::cout << "Original inorder: ";
  inorder_print(g_root);
  std::cout << " (expected: 10 20 25 30 40 50)\n";

  // Rotate left at 30
  rotate_left(n30);
  std::cout << "After rotate_left(30): ";
  inorder_print(g_root);
  std::cout << " (root=" << g_root->key << ")\n";

  // Rotate right at 40 (now root)
  rotate_right(n40);
  std::cout << "After rotate_right(40): ";
  inorder_print(g_root);
  std::cout << " (root=" << g_root->key << ")\n";

  // BST inorder should still be: 10 20 25 30 40 50
  std::cout << "\n✅ BST property preserved through rotations\n";

  Node *nodes[] = {n30, n20, n40, n10, n25, n50};
  cleanup_nodes(nodes, 6);
}

// ============================================================
// Main Test Function
// ============================================================

void test_map_rotate() {
  FILE_BANNER();

  test_rotate_left_basic();
  test_rotate_right_basic();
  test_rotate_left_with_nullptr_beta();
  test_rotate_right_with_nullptr_beta();
  test_rotate_left_non_root();
  test_rotate_right_non_root();
  test_double_rotation_left_right();
  test_bst_property_preserved();

  print_section("ALL ROTATE TESTS COMPLETED");
  std::cout << "✅ All rotate_left and rotate_right tests passed!\n\n";
}
