#include "test_config.h"
#include "test_print.h"
#include <cassert>
#include <iostream>
#include <string>

// ============================================================
// Standalone Node and RBT helper functions for testing
// (Copied from map.h for isolated testing)
// ============================================================

namespace test_rbt_helpers {

struct Node {
  int key;
  Node *parent;
  Node *left;
  Node *right;
  bool red;

  Node(int k, bool is_red = true)
      : key(k), parent(nullptr), left(nullptr), right(nullptr), red(is_red) {}
};

// ============================================================
// Helper functions from map.h
// ============================================================

// _find_uncle: 삼촌 노드 찾기
Node *find_uncle(Node *new_node) {
  Node *parent_node = new_node->parent;
  if (parent_node == nullptr) {
    return nullptr;
  }

  Node *grand_node = parent_node->parent;
  if (grand_node == nullptr) {
    return nullptr;
  }

  // 경우 나눠서 uncle node 찾아주기
  if (grand_node->left == parent_node) {
    return grand_node->right;
  }
  return grand_node->left;
}

// _check_reconstruct: 재구성이 필요한지 확인
// parent가 red이고, uncle이 black(또는 nullptr)인 경우
bool check_reconstruct(Node *new_node) {
  if (new_node->parent == nullptr) {
    return false; // new_node가 root면 false
  }

  Node *uncle_node = find_uncle(new_node);

  // uncle의 색을 확인하는 로직 (nullptr은 black 취급)
  bool uncle_red = (uncle_node != nullptr && uncle_node->red);

  if (new_node->parent->red && !uncle_red) {
    return true;
  }
  return false;
}

// _check_recoloring: 재색칠이 필요한지 확인
// parent가 red이고, uncle도 red인 경우
bool check_recoloring(Node *new_node) {
  if (new_node->parent == nullptr) {
    return false; // new_node가 root면 false
  }

  Node *uncle_node = find_uncle(new_node);

  // uncle의 색을 확인하는 로직 (nullptr은 black 취급)
  bool uncle_red = (uncle_node != nullptr && uncle_node->red);

  if (new_node->parent->red && uncle_red) {
    return true;
  }
  return false;
}

// ============================================================
// Test Utilities
// ============================================================

void print_node_info(const char *label, Node *n) {
  if (n == nullptr) {
    std::cout << label << ": nullptr\n";
  } else {
    std::cout << label << ": key=" << n->key
              << ", color=" << (n->red ? "RED" : "BLACK") << ", parent="
              << (n->parent ? std::to_string(n->parent->key) : "null")
              << ", left=" << (n->left ? std::to_string(n->left->key) : "null")
              << ", right="
              << (n->right ? std::to_string(n->right->key) : "null") << "\n";
  }
}

void print_tree_structure(Node *root, const std::string &title) {
  std::cout << "\n=== " << title << " ===\n";
  if (root == nullptr) {
    std::cout << "(empty tree)\n";
    return;
  }

  // Simple level-order print for visualization
  std::cout << "Root: " << root->key << " (" << (root->red ? "R" : "B")
            << ")\n";
  if (root->left) {
    std::cout << "  L: " << root->left->key << " ("
              << (root->left->red ? "R" : "B") << ")";
    if (root->left->left)
      std::cout << " [LL:" << root->left->left->key << "]";
    if (root->left->right)
      std::cout << " [LR:" << root->left->right->key << "]";
    std::cout << "\n";
  }
  if (root->right) {
    std::cout << "  R: " << root->right->key << " ("
              << (root->right->red ? "R" : "B") << ")";
    if (root->right->left)
      std::cout << " [RL:" << root->right->left->key << "]";
    if (root->right->right)
      std::cout << " [RR:" << root->right->right->key << "]";
    std::cout << "\n";
  }
}

void cleanup_nodes(Node *nodes[], int count) {
  for (int i = 0; i < count; ++i) {
    delete nodes[i];
  }
}

} // namespace test_rbt_helpers

// ============================================================
// Test Cases for _find_uncle
// ============================================================

void test_find_uncle_left_path() {
  using namespace test_rbt_helpers;
  print_section("find_uncle: Node on left path");

  //        G(20)
  //       /    \
    //     P(10)  U(30)  <-- uncle
  //     /
  //   N(5) <-- new_node

  Node *G = new Node(20, false); // grandparent (black)
  Node *P = new Node(10, true);  // parent (red)
  Node *U = new Node(30, true);  // uncle (red)
  Node *N = new Node(5, true);   // new_node (red)

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");

  Node *uncle = find_uncle(N);

  std::cout << "find_uncle(N=5) = ";
  if (uncle)
    std::cout << uncle->key << "\n";
  else
    std::cout << "nullptr\n";

  assert(uncle == U && "Uncle should be node 30");
  std::cout << "✅ find_uncle left path PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_find_uncle_right_path() {
  using namespace test_rbt_helpers;
  print_section("find_uncle: Node on right path");

  //        G(20)
  //       /    \
    //     U(10)  P(30)
  //              \
    //             N(35)

  Node *G = new Node(20, false);
  Node *U = new Node(10, true); // uncle
  Node *P = new Node(30, true); // parent
  Node *N = new Node(35, true); // new_node

  G->left = U;
  G->right = P;
  U->parent = G;
  P->parent = G;
  P->right = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");

  Node *uncle = find_uncle(N);

  std::cout << "find_uncle(N=35) = ";
  if (uncle)
    std::cout << uncle->key << "\n";
  else
    std::cout << "nullptr\n";

  assert(uncle == U && "Uncle should be node 10");
  std::cout << "✅ find_uncle right path PASSED\n";

  Node *nodes[] = {G, U, P, N};
  cleanup_nodes(nodes, 4);
}

void test_find_uncle_no_grandparent() {
  using namespace test_rbt_helpers;
  print_section("find_uncle: No grandparent (parent is root)");

  //     P(20) <-- root, no parent
  //     /
  //   N(10)

  Node *P = new Node(20, false); // root
  Node *N = new Node(10, true);

  P->left = N;
  N->parent = P;
  P->parent = nullptr;

  Node *uncle = find_uncle(N);

  std::cout << "find_uncle(N=10) when parent is root = ";
  if (uncle)
    std::cout << uncle->key << "\n";
  else
    std::cout << "nullptr\n";

  assert(uncle == nullptr && "Uncle should be nullptr when no grandparent");
  std::cout << "✅ find_uncle no grandparent PASSED\n";

  Node *nodes[] = {P, N};
  cleanup_nodes(nodes, 2);
}

void test_find_uncle_nullptr_uncle() {
  using namespace test_rbt_helpers;
  print_section("find_uncle: Uncle position is nullptr");

  //        G(20)
  //       /
  //     P(10)  (no right child - uncle is nullptr)
  //     /
  //   N(5)

  Node *G = new Node(20, false);
  Node *P = new Node(10, true);
  Node *N = new Node(5, true);

  G->left = P;
  G->right = nullptr; // uncle position is nullptr
  P->parent = G;
  P->left = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");

  Node *uncle = find_uncle(N);

  std::cout << "find_uncle(N=5) when uncle position is empty = ";
  if (uncle)
    std::cout << uncle->key << "\n";
  else
    std::cout << "nullptr\n";

  assert(uncle == nullptr && "Uncle should be nullptr");
  std::cout << "✅ find_uncle nullptr uncle PASSED\n";

  Node *nodes[] = {G, P, N};
  cleanup_nodes(nodes, 3);
}

// ============================================================
// Test Cases for _check_reconstruct
// ============================================================

void test_check_reconstruct_true_case() {
  using namespace test_rbt_helpers;
  print_section(
      "check_reconstruct: Should return true (parent=RED, uncle=BLACK)");

  //        G(20) BLACK
  //       /    \
    //     P(10)  U(30)
  //     RED     BLACK
  //     /
  //   N(5) RED

  Node *G = new Node(20, false); // black
  Node *P = new Node(10, true);  // red
  Node *U = new Node(30, false); // black - triggers reconstruct
  Node *N = new Node(5, true);   // red

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");
  std::cout << "Parent(10)=RED, Uncle(30)=BLACK\n";

  bool result = check_reconstruct(N);
  std::cout << "check_reconstruct(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == true && "Should need reconstruct");
  std::cout << "✅ check_reconstruct true case PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_check_reconstruct_false_uncle_red() {
  using namespace test_rbt_helpers;
  print_section("check_reconstruct: Should return false (parent=RED, uncle=RED "
                "-> recolor instead)");

  //        G(20) BLACK
  //       /    \
    //     P(10)  U(30)
  //     RED     RED
  //     /
  //   N(5) RED

  Node *G = new Node(20, false);
  Node *P = new Node(10, true); // red
  Node *U = new Node(30, true); // red - triggers recoloring, not reconstruct
  Node *N = new Node(5, true);

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");
  std::cout << "Parent(10)=RED, Uncle(30)=RED\n";

  bool result = check_reconstruct(N);
  std::cout << "check_reconstruct(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == false && "Should NOT need reconstruct (uncle is red)");
  std::cout << "✅ check_reconstruct false (uncle red) PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_check_reconstruct_false_parent_black() {
  using namespace test_rbt_helpers;
  print_section("check_reconstruct: Should return false (parent=BLACK)");

  //        G(20) BLACK
  //       /    \
    //     P(10)  U(30)
  //     BLACK   RED
  //     /
  //   N(5) RED

  Node *G = new Node(20, false);
  Node *P = new Node(10, false); // black - no violation
  Node *U = new Node(30, true);
  Node *N = new Node(5, true);

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  bool result = check_reconstruct(N);
  std::cout << "Parent(10)=BLACK, so no red-red violation\n";
  std::cout << "check_reconstruct(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == false && "No reconstruct needed when parent is black");
  std::cout << "✅ check_reconstruct false (parent black) PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_check_reconstruct_nullptr_uncle() {
  using namespace test_rbt_helpers;
  print_section("check_reconstruct: Uncle is nullptr (treated as BLACK)");

  //        G(20) BLACK
  //       /
  //     P(10)  (no uncle)
  //     RED
  //     /
  //   N(5) RED

  Node *G = new Node(20, false);
  Node *P = new Node(10, true); // red
  Node *N = new Node(5, true);

  G->left = P;
  G->right = nullptr; // nullptr = black
  P->parent = G;
  P->left = N;
  N->parent = P;

  bool result = check_reconstruct(N);
  std::cout << "Parent(10)=RED, Uncle=nullptr(treated as BLACK)\n";
  std::cout << "check_reconstruct(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == true &&
         "nullptr uncle treated as black, so reconstruct needed");
  std::cout << "✅ check_reconstruct nullptr uncle PASSED\n";

  Node *nodes[] = {G, P, N};
  cleanup_nodes(nodes, 3);
}

// ============================================================
// Test Cases for _check_recoloring
// ============================================================

void test_check_recoloring_true_case() {
  using namespace test_rbt_helpers;
  print_section("check_recoloring: Should return true (parent=RED, uncle=RED)");

  //        G(20) BLACK
  //       /    \
    //     P(10)  U(30)
  //     RED     RED
  //     /
  //   N(5) RED

  Node *G = new Node(20, false);
  Node *P = new Node(10, true); // red
  Node *U = new Node(30, true); // red - triggers recoloring
  Node *N = new Node(5, true);

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  print_tree_structure(G, "Tree structure");
  std::cout << "Parent(10)=RED, Uncle(30)=RED\n";

  bool result = check_recoloring(N);
  std::cout << "check_recoloring(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == true && "Should need recoloring");
  std::cout << "✅ check_recoloring true case PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_check_recoloring_false_uncle_black() {
  using namespace test_rbt_helpers;
  print_section(
      "check_recoloring: Should return false (parent=RED, uncle=BLACK)");

  //        G(20) BLACK
  //       /    \
    //     P(10)  U(30)
  //     RED     BLACK
  //     /
  //   N(5) RED

  Node *G = new Node(20, false);
  Node *P = new Node(10, true);
  Node *U = new Node(30, false); // black - reconstruct, not recolor
  Node *N = new Node(5, true);

  G->left = P;
  G->right = U;
  P->parent = G;
  U->parent = G;
  P->left = N;
  N->parent = P;

  bool result = check_recoloring(N);
  std::cout << "Parent(10)=RED, Uncle(30)=BLACK\n";
  std::cout << "check_recoloring(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == false && "Should NOT need recoloring (uncle is black)");
  std::cout << "✅ check_recoloring false (uncle black) PASSED\n";

  Node *nodes[] = {G, P, U, N};
  cleanup_nodes(nodes, 4);
}

void test_check_recoloring_false_nullptr_uncle() {
  using namespace test_rbt_helpers;
  print_section("check_recoloring: Uncle is nullptr (treated as BLACK)");

  Node *G = new Node(20, false);
  Node *P = new Node(10, true);
  Node *N = new Node(5, true);

  G->left = P;
  G->right = nullptr;
  P->parent = G;
  P->left = N;
  N->parent = P;

  bool result = check_recoloring(N);
  std::cout << "Parent(10)=RED, Uncle=nullptr(BLACK)\n";
  std::cout << "check_recoloring(N=5) = " << (result ? "true" : "false")
            << "\n";

  assert(result == false && "nullptr uncle is black, so no recoloring");
  std::cout << "✅ check_recoloring nullptr uncle PASSED\n";

  Node *nodes[] = {G, P, N};
  cleanup_nodes(nodes, 3);
}

// ============================================================
// Integration: Combined scenario tests
// ============================================================

void test_recolor_vs_reconstruct_decision() {
  using namespace test_rbt_helpers;
  print_section("Integration: Recolor vs Reconstruct decision matrix");

  std::cout << "\n| Parent Color | Uncle Color | Recolor | Reconstruct |\n";
  std::cout << "|--------------|-------------|---------|-------------|\n";

  // Case 1: P=RED, U=RED -> recolor=true, reconstruct=false
  {
    Node *G = new Node(20, false);
    Node *P = new Node(10, true);
    Node *U = new Node(30, true);
    Node *N = new Node(5, true);
    G->left = P;
    G->right = U;
    P->parent = G;
    U->parent = G;
    P->left = N;
    N->parent = P;

    bool recolor = check_recoloring(N);
    bool reconstruct = check_reconstruct(N);
    std::cout << "| RED          | RED         | "
              << (recolor ? "true   " : "false  ") << " | "
              << (reconstruct ? "true       " : "false      ") << " |\n";
    assert(recolor == true && reconstruct == false);

    Node *nodes[] = {G, P, U, N};
    cleanup_nodes(nodes, 4);
  }

  // Case 2: P=RED, U=BLACK -> recolor=false, reconstruct=true
  {
    Node *G = new Node(20, false);
    Node *P = new Node(10, true);
    Node *U = new Node(30, false);
    Node *N = new Node(5, true);
    G->left = P;
    G->right = U;
    P->parent = G;
    U->parent = G;
    P->left = N;
    N->parent = P;

    bool recolor = check_recoloring(N);
    bool reconstruct = check_reconstruct(N);
    std::cout << "| RED          | BLACK       | "
              << (recolor ? "true   " : "false  ") << " | "
              << (reconstruct ? "true       " : "false      ") << " |\n";
    assert(recolor == false && reconstruct == true);

    Node *nodes[] = {G, P, U, N};
    cleanup_nodes(nodes, 4);
  }

  // Case 3: P=RED, U=nullptr -> recolor=false, reconstruct=true
  {
    Node *G = new Node(20, false);
    Node *P = new Node(10, true);
    Node *N = new Node(5, true);
    G->left = P;
    G->right = nullptr;
    P->parent = G;
    P->left = N;
    N->parent = P;

    bool recolor = check_recoloring(N);
    bool reconstruct = check_reconstruct(N);
    std::cout << "| RED          | nullptr     | "
              << (recolor ? "true   " : "false  ") << " | "
              << (reconstruct ? "true       " : "false      ") << " |\n";
    assert(recolor == false && reconstruct == true);

    Node *nodes[] = {G, P, N};
    cleanup_nodes(nodes, 3);
  }

  // Case 4: P=BLACK, U=RED -> recolor=false, reconstruct=false (no violation)
  {
    Node *G = new Node(20, false);
    Node *P = new Node(10, false);
    Node *U = new Node(30, true);
    Node *N = new Node(5, true);
    G->left = P;
    G->right = U;
    P->parent = G;
    U->parent = G;
    P->left = N;
    N->parent = P;

    bool recolor = check_recoloring(N);
    bool reconstruct = check_reconstruct(N);
    std::cout << "| BLACK        | RED         | "
              << (recolor ? "true   " : "false  ") << " | "
              << (reconstruct ? "true       " : "false      ") << " |\n";
    assert(recolor == false && reconstruct == false);

    Node *nodes[] = {G, P, U, N};
    cleanup_nodes(nodes, 4);
  }

  // Case 5: P=BLACK, U=BLACK -> recolor=false, reconstruct=false (no violation)
  {
    Node *G = new Node(20, false);
    Node *P = new Node(10, false);
    Node *U = new Node(30, false);
    Node *N = new Node(5, true);
    G->left = P;
    G->right = U;
    P->parent = G;
    U->parent = G;
    P->left = N;
    N->parent = P;

    bool recolor = check_recoloring(N);
    bool reconstruct = check_reconstruct(N);
    std::cout << "| BLACK        | BLACK       | "
              << (recolor ? "true   " : "false  ") << " | "
              << (reconstruct ? "true       " : "false      ") << " |\n";
    assert(recolor == false && reconstruct == false);

    Node *nodes[] = {G, P, U, N};
    cleanup_nodes(nodes, 4);
  }

  std::cout << "\n✅ Decision matrix verification PASSED\n";
}

// ============================================================
// Main Test Function
// ============================================================

void test_map_rbt_helpers() {
  FILE_BANNER();

  // find_uncle tests
  test_find_uncle_left_path();
  test_find_uncle_right_path();
  test_find_uncle_no_grandparent();
  test_find_uncle_nullptr_uncle();

  // check_reconstruct tests
  test_check_reconstruct_true_case();
  test_check_reconstruct_false_uncle_red();
  test_check_reconstruct_false_parent_black();
  test_check_reconstruct_nullptr_uncle();

  // check_recoloring tests
  test_check_recoloring_true_case();
  test_check_recoloring_false_uncle_black();
  test_check_recoloring_false_nullptr_uncle();

  // Integration test
  test_recolor_vs_reconstruct_decision();

  print_section("ALL RBT HELPER TESTS COMPLETED");
  std::cout << "✅ All _find_uncle, _check_reconstruct, _check_recoloring "
               "tests passed!\n\n";
}
