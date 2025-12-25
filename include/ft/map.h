#ifndef FT_MAP_H
#define FT_MAP_H

#include "pair.h"
#include <cstddef>
#include <functional>

namespace ft {

template <class Key, class T, class Compare = std::less<Key>> class map {
public:
  // ---------------- Member types ----------------
  typedef Key key_type;
  typedef T mapped_type;
  typedef ft::pair<const Key, T> value_type;
  typedef Compare key_compare;
  typedef std::size_t size_type;

private:
  // ---------------- Node definition ----------------
  struct Node {
    value_type value;
    Node *parent;
    Node *left;
    Node *right;
    bool red;

    Node(const value_type &v)
        : value(v), parent(nullptr), left(nullptr), right(nullptr), red(true) {}
  };

  Node *_root;
  size_type _size;
  key_compare _comp;

public:
  // ---------------- Constructor ----------------
  map() : _root(nullptr), _size(0), _comp(key_compare()) {}

  // ---------------- Capacity ----------------
  size_type size() const { return _size; }
  bool empty() const { return _size == 0; }

  // ---------------- Modifiers ----------------
  // BST 삽입 + RBT 균형 복구
  ft::pair<Node *, bool> insert(const value_type &pair_to_be_inserted) {
    Node *new_node = new Node(pair_to_be_inserted);

    // Case 1: 빈 트리인 경우
    if (_root == nullptr) {
      _root = new_node;
      _root->red = false; // 루트는 항상 black
      ++_size;
      return ft::make_pair(_root, true);
    }

    // BST 삽입 위치 찾기
    Node *parent = _find_insert_position(pair_to_be_inserted.first, _root);

    // 이미 같은 키가 존재하는 경우
    if (!_comp(parent->value.first, pair_to_be_inserted.first) &&
        !_comp(pair_to_be_inserted.first, parent->value.first)) {
      delete new_node; // 중복 키는 삽입하지 않음
      return ft::make_pair(parent, false);
    }

    // 새 노드를 부모에 연결
    new_node->parent = parent;
    if (_comp(pair_to_be_inserted.first, parent->value.first)) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
    ++_size;

    // RBT 속성 복구 (fix-up)
    _insert_fixup(new_node);

    return ft::make_pair(new_node, true);
  }

  void erase(const key_type &key) {
    Node *node = _find_node(key);
    if (node != nullptr) {
      _erase_node(node);
    }
  }

  // 키로 노드 찾기
  Node *find(const key_type &key) { return _find_node(key); }

private:
  // 키로 노드 찾기 (내부용)
  Node *_find_node(const key_type &key) {
    Node *current = _root;
    while (current != nullptr) {
      if (_comp(key, current->value.first)) {
        current = current->left;
      } else if (_comp(current->value.first, key)) {
        current = current->right;
      } else {
        return current; // 찾음
      }
    }
    return nullptr; // 못 찾음
  }

  // 서브트리의 최소값 노드 찾기
  Node *_minimum(Node *node) {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  // 노드 u를 노드 v로 대체 (transplant)
  void _transplant(Node *u, Node *v) {
    if (u->parent == nullptr) {
      _root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  // 노드 삭제
  void _erase_node(Node *z) {
    Node *y = z;
    Node *x = nullptr;
    Node *x_parent = nullptr; // x가 nullptr일 때 부모 추적용
    bool y_original_red = y->red;

    if (z->left == nullptr) {
      // Case 1: 왼쪽 자식 없음
      x = z->right;
      x_parent = z->parent;
      _transplant(z, z->right);
    } else if (z->right == nullptr) {
      // Case 2: 오른쪽 자식 없음
      x = z->left;
      x_parent = z->parent;
      _transplant(z, z->left);
    } else {
      // Case 3: 양쪽 자식 모두 있음 - 후임자(successor)로 대체
      y = _minimum(z->right);
      y_original_red = y->red;
      x = y->right;

      if (y->parent == z) {
        // y가 z의 직접 자식인 경우
        x_parent = y;
      } else {
        // y를 원래 위치에서 분리
        x_parent = y->parent;
        _transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }

      _transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->red = z->red;
    }

    delete z;
    --_size;

    // 삭제된 노드가 BLACK이면 RBT 속성 복구 필요
    if (!y_original_red) {
      _erase_fixup(x, x_parent);
    }
  }

  // RBT 삭제 후 속성 복구
  void _erase_fixup(Node *x, Node *x_parent) {
    while (x != _root && (x == nullptr || !x->red)) {
      if (x == (x_parent != nullptr ? x_parent->left : nullptr)) {
        // x가 왼쪽 자식인 경우
        Node *w = x_parent->right; // 형제

        if (w != nullptr && w->red) {
          // Case 1: 형제가 RED
          w->red = false;
          x_parent->red = true;
          _rotate_left(x_parent);
          w = x_parent->right;
        }

        if (w == nullptr || ((w->left == nullptr || !w->left->red) &&
                             (w->right == nullptr || !w->right->red))) {
          // Case 2: 형제의 양쪽 자식이 모두 BLACK
          if (w != nullptr)
            w->red = true;
          x = x_parent;
          x_parent = x->parent;
        } else {
          if (w->right == nullptr || !w->right->red) {
            // Case 3: 형제의 오른쪽 자식이 BLACK
            if (w->left != nullptr)
              w->left->red = false;
            w->red = true;
            _rotate_right(w);
            w = x_parent->right;
          }
          // Case 4: 형제의 오른쪽 자식이 RED
          w->red = x_parent->red;
          x_parent->red = false;
          if (w->right != nullptr)
            w->right->red = false;
          _rotate_left(x_parent);
          x = _root; // 종료
        }
      } else {
        // x가 오른쪽 자식인 경우 (대칭)
        Node *w = x_parent != nullptr ? x_parent->left : nullptr; // 형제

        if (w != nullptr && w->red) {
          // Case 1: 형제가 RED
          w->red = false;
          x_parent->red = true;
          _rotate_right(x_parent);
          w = x_parent->left;
        }

        if (w == nullptr || ((w->right == nullptr || !w->right->red) &&
                             (w->left == nullptr || !w->left->red))) {
          // Case 2: 형제의 양쪽 자식이 모두 BLACK
          if (w != nullptr)
            w->red = true;
          x = x_parent;
          x_parent = x->parent;
        } else {
          if (w->left == nullptr || !w->left->red) {
            // Case 3: 형제의 왼쪽 자식이 BLACK
            if (w->right != nullptr)
              w->right->red = false;
            w->red = true;
            _rotate_left(w);
            w = x_parent->left;
          }
          // Case 4: 형제의 왼쪽 자식이 RED
          w->red = x_parent->red;
          x_parent->red = false;
          if (w->left != nullptr)
            w->left->red = false;
          _rotate_right(x_parent);
          x = _root; // 종료
        }
      }
    }
    if (x != nullptr)
      x->red = false;
  }

private:
  // ---------------- Helper functions ----------------

  // 삽입 위치의 부모 노드를 찾는 함수
  Node *_find_insert_position(const key_type &key, Node *node) {
    if (_comp(key, node->value.first)) {
      // key < node의 key -> 왼쪽으로
      if (node->left != nullptr) {
        return _find_insert_position(key, node->left);
      }
      return node; // 삽입 위치의 부모
    } else if (_comp(node->value.first, key)) {
      // key > node의 key -> 오른쪽으로
      if (node->right != nullptr) {
        return _find_insert_position(key, node->right);
      }
      return node; // 삽입 위치의 부모
    }
    // key == node의 key (이미 존재)
    return node;
  }

  // RBT 삽입 후 속성 복구
  void _insert_fixup(Node *node) {
    while (node != _root && node->parent->red) {
      Node *parent = node->parent;
      Node *grandparent = parent->parent;

      if (grandparent == nullptr)
        break;

      if (parent == grandparent->left) {
        // Case A: 부모가 조부모의 왼쪽 자식
        Node *uncle = grandparent->right;

        if (uncle != nullptr && uncle->red) {
          // Case 1: Uncle이 RED -> Recoloring
          parent->red = false;
          uncle->red = false;
          grandparent->red = true;
          node = grandparent; // 위로 올라가며 계속 검사
        } else {
          // Case 2 & 3: Uncle이 BLACK (또는 nullptr)
          if (node == parent->right) {
            // Case 2: node가 오른쪽 자식 -> Left Rotate로 Case 3으로 변환
            node = parent;
            _rotate_left(node);
            parent = node->parent;
          }
          // Case 3: node가 왼쪽 자식 -> Right Rotate
          parent->red = false;
          grandparent->red = true;
          _rotate_right(grandparent);
        }
      } else {
        // Case B: 부모가 조부모의 오른쪽 자식 (대칭)
        Node *uncle = grandparent->left;

        if (uncle != nullptr && uncle->red) {
          // Case 1: Uncle이 RED -> Recoloring
          parent->red = false;
          uncle->red = false;
          grandparent->red = true;
          node = grandparent;
        } else {
          // Case 2 & 3: Uncle이 BLACK
          if (node == parent->left) {
            // Case 2: node가 왼쪽 자식 -> Right Rotate로 Case 3으로 변환
            node = parent;
            _rotate_right(node);
            parent = node->parent;
          }
          // Case 3: node가 오른쪽 자식 -> Left Rotate
          parent->red = false;
          grandparent->red = true;
          _rotate_left(grandparent);
        }
      }
    }
    // 루트는 항상 BLACK
    _root->red = false;
  }

  // Uncle 노드 찾기
  Node *_find_uncle(Node *new_node) {
    Node *parent_node = new_node->parent;
    if (parent_node == nullptr)
      return nullptr;

    Node *grand_node = parent_node->parent;
    if (grand_node == nullptr)
      return nullptr;

    if (grand_node->left == parent_node) {
      return grand_node->right;
    }
    return grand_node->left;
  }

  //   (x)                       (y)
  //   / \     Left Rotate(x)    / \
        //  α  (y)   ------------->  (x)  γ
  //     / \                   / \
        //    β   γ  <------------- α   β
  //           Right Rotate(y)

  void _rotate_left(Node *x) {
    Node *y = x->right;

    // β를 x의 오른쪽 자식으로
    x->right = y->left;
    if (y->left != nullptr) {
      y->left->parent = x;
    }

    // y의 부모를 x의 부모로
    y->parent = x->parent;
    if (x->parent == nullptr) {
      _root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    // x를 y의 왼쪽 자식으로
    y->left = x;
    x->parent = y;
  }

  void _rotate_right(Node *x) {
    Node *y = x->left;

    // β를 x의 왼쪽 자식으로
    x->left = y->right;
    if (y->right != nullptr) {
      y->right->parent = x;
    }

    // y의 부모를 x의 부모로
    y->parent = x->parent;
    if (x->parent == nullptr) {
      _root = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }

    // x를 y의 오른쪽 자식으로
    y->right = x;
    x->parent = y;
  }
};

} // namespace ft

#endif
