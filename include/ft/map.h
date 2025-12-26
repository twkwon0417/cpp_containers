#ifndef FT_MAP_H
#define FT_MAP_H

#include "pair.h"
#include "reverse_iterator.h"
#include <cstddef>
#include <functional>
#include <memory>

namespace ft {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<ft::pair<const Key, T> > >
class map {
public:
  // ---------------- Member types ----------------
  typedef Key key_type;
  typedef T mapped_type;
  typedef ft::pair<const Key, T> value_type;
  typedef Compare key_compare;
  typedef Allocator allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  // ---------------- value_compare class ----------------
  class value_compare {
    friend class map;

  protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

  public:
    typedef bool result_type;
    typedef value_type first_argument_type;
    typedef value_type second_argument_type;

    bool operator()(const value_type &x, const value_type &y) const {
      return comp(x.first, y.first);
    }
  };

private:
  // ---------------- Node definition ----------------
  struct Node {
    value_type value;
    Node *parent;
    Node *left;
    Node *right;
    bool red;

    Node(const value_type &v)
        : value(v), parent(NULL), left(NULL), right(NULL), red(true) {}
  };

public:
  // ---------------- Iterator class ----------------
  class iterator {
  public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef value_type value_type;
    typedef difference_type difference_type;
    typedef value_type *pointer;
    typedef value_type &reference;

  private:
    Node *_node;
    Node *_root;

    Node *_tree_min(Node *n) const {
      if (n == NULL)
        return NULL;
      while (n->left != NULL)
        n = n->left;
      return n;
    }

    Node *_tree_max(Node *n) const {
      if (n == NULL)
        return NULL;
      while (n->right != NULL)
        n = n->right;
      return n;
    }

  public:
    iterator() : _node(NULL), _root(NULL) {}
    iterator(Node *node, Node *root) : _node(node), _root(root) {}
    iterator(const iterator &other) : _node(other._node), _root(other._root) {}

    iterator &operator=(const iterator &other) {
      _node = other._node;
      _root = other._root;
      return *this;
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }

    // Pre-increment: move to next inorder
    iterator &operator++() {
      if (_node == NULL)
        return *this;
      if (_node->right != NULL) {
        _node = _tree_min(_node->right);
      } else {
        Node *p = _node->parent;
        while (p != NULL && _node == p->right) {
          _node = p;
          p = p->parent;
        }
        _node = p;
      }
      return *this;
    }

    // Post-increment
    iterator operator++(int) {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    // Pre-decrement: move to previous inorder
    iterator &operator--() {
      if (_node == NULL) {
        // end() -> last element
        _node = _tree_max(_root);
        return *this;
      }
      if (_node->left != NULL) {
        _node = _tree_max(_node->left);
      } else {
        Node *p = _node->parent;
        while (p != NULL && _node == p->left) {
          _node = p;
          p = p->parent;
        }
        _node = p;
      }
      return *this;
    }

    // Post-decrement
    iterator operator--(int) {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const {
      return _node == other._node;
    }
    bool operator!=(const iterator &other) const {
      return _node != other._node;
    }

    Node *base() const { return _node; }
    Node *root() const { return _root; }
  };

  // ---------------- Const Iterator class ----------------
  class const_iterator {
  public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef value_type value_type;
    typedef difference_type difference_type;
    typedef const value_type *pointer;
    typedef const value_type &reference;

  private:
    const Node *_node;
    const Node *_root;

    const Node *_tree_min(const Node *n) const {
      if (n == NULL)
        return NULL;
      while (n->left != NULL)
        n = n->left;
      return n;
    }

    const Node *_tree_max(const Node *n) const {
      if (n == NULL)
        return NULL;
      while (n->right != NULL)
        n = n->right;
      return n;
    }

  public:
    const_iterator() : _node(NULL), _root(NULL) {}
    const_iterator(const Node *node, const Node *root)
        : _node(node), _root(root) {}
    const_iterator(const const_iterator &other)
        : _node(other._node), _root(other._root) {}
    const_iterator(const iterator &other)
        : _node(other.base()), _root(other.root()) {}

    const_iterator &operator=(const const_iterator &other) {
      _node = other._node;
      _root = other._root;
      return *this;
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }

    const_iterator &operator++() {
      if (_node == NULL)
        return *this;
      if (_node->right != NULL) {
        _node = _tree_min(_node->right);
      } else {
        const Node *p = _node->parent;
        while (p != NULL && _node == p->right) {
          _node = p;
          p = p->parent;
        }
        _node = p;
      }
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() {
      if (_node == NULL) {
        _node = _tree_max(_root);
        return *this;
      }
      if (_node->left != NULL) {
        _node = _tree_max(_node->left);
      } else {
        const Node *p = _node->parent;
        while (p != NULL && _node == p->left) {
          _node = p;
          p = p->parent;
        }
        _node = p;
      }
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const const_iterator &other) const {
      return _node == other._node;
    }
    bool operator!=(const const_iterator &other) const {
      return _node != other._node;
    }
  };

  // ---------------- Reverse iterators ----------------
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

private:
  Node *_root;
  size_type _size;
  key_compare _comp;
  allocator_type _alloc;

public:
  // ---------------- Constructors ----------------
  map()
      : _root(NULL), _size(0), _comp(key_compare()), _alloc(allocator_type()) {}

  explicit map(const Compare &comp, const Allocator &alloc = Allocator())
      : _root(NULL), _size(0), _comp(comp), _alloc(alloc) {}

  template <class InputIt>
  map(InputIt first, InputIt last, const Compare &comp = Compare(),
      const Allocator &alloc = Allocator())
      : _root(NULL), _size(0), _comp(comp), _alloc(alloc) {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  map(const map &other)
      : _root(NULL), _size(0), _comp(other._comp), _alloc(other._alloc) {
    for (const_iterator it = other.begin(); it != other.end(); ++it) {
      insert(*it);
    }
  }

  // ---------------- Destructor ----------------
  ~map() { clear(); }

  // ---------------- Assignment operator ----------------
  map &operator=(const map &other) {
    if (this != &other) {
      clear();
      _comp = other._comp;
      _alloc = other._alloc;
      for (const_iterator it = other.begin(); it != other.end(); ++it) {
        insert(*it);
      }
    }
    return *this;
  }

  // ---------------- Allocator ----------------
  allocator_type get_allocator() const { return _alloc; }

  // ---------------- Element access ----------------
  mapped_type &operator[](const key_type &key) {
    iterator it = find(key);
    if (it != end()) {
      return it->second;
    }
    ft::pair<iterator, bool> result = insert(ft::make_pair(key, mapped_type()));
    return result.first->second;
  }

  // ---------------- Capacity ----------------
  size_type size() const { return _size; }
  bool empty() const { return _size == 0; }
  size_type max_size() const {
    return static_cast<size_type>(-1) / sizeof(Node);
  }

  // ---------------- Iterators ----------------
  iterator begin() {
    if (_root == NULL)
      return end();
    Node *n = _root;
    while (n->left != NULL)
      n = n->left;
    return iterator(n, _root);
  }

  const_iterator begin() const {
    if (_root == NULL)
      return end();
    const Node *n = _root;
    while (n->left != NULL)
      n = n->left;
    return const_iterator(n, _root);
  }

  iterator end() { return iterator(NULL, _root); }
  const_iterator end() const { return const_iterator(NULL, _root); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // ---------------- Modifiers ----------------
  // BST 삽입 + RBT 균형 복구
  ft::pair<iterator, bool> insert(const value_type &pair_to_be_inserted) {
    Node *new_node = new Node(pair_to_be_inserted);

    // Case 1: 빈 트리인 경우
    if (_root == NULL) {
      _root = new_node;
      _root->red = false; // 루트는 항상 black
      ++_size;
      return ft::make_pair(iterator(_root, _root), true);
    }

    // BST 삽입 위치 찾기
    Node *parent = _find_insert_position(pair_to_be_inserted.first, _root);

    // 이미 같은 키가 존재하는 경우
    if (!_comp(parent->value.first, pair_to_be_inserted.first) &&
        !_comp(pair_to_be_inserted.first, parent->value.first)) {
      delete new_node; // 중복 키는 삽입하지 않음
      return ft::make_pair(iterator(parent, _root), false);
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

    return ft::make_pair(iterator(new_node, _root), true);
  }

  // 힌트 삽입 (힌트는 무시하고 일반 삽입 수행)
  iterator insert(iterator hint, const value_type &value) {
    (void)hint;
    return insert(value).first;
  }

  // 범위 삽입
  template <class InputIt> void insert(InputIt first, InputIt last) {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  void erase(iterator pos) {
    if (pos != end()) {
      Node *node = pos.base();
      if (node != NULL) {
        _erase_node(node);
      }
    }
  }

  void erase(iterator first, iterator last) {
    while (first != last) {
      iterator tmp = first;
      ++first;
      erase(tmp);
    }
  }

  size_type erase(const key_type &key) {
    Node *node = _find_node(key);
    if (node != NULL) {
      _erase_node(node);
      return 1;
    }
    return 0;
  }

  void clear() {
    _clear_tree(_root);
    _root = NULL;
  }

  void swap(map &other) {
    Node *tmp_root = _root;
    size_type tmp_size = _size;
    key_compare tmp_comp = _comp;
    allocator_type tmp_alloc = _alloc;

    _root = other._root;
    _size = other._size;
    _comp = other._comp;
    _alloc = other._alloc;

    other._root = tmp_root;
    other._size = tmp_size;
    other._comp = tmp_comp;
    other._alloc = tmp_alloc;
  }

  // ---------------- Lookup ----------------
  iterator find(const key_type &key) {
    Node *node = _find_node(key);
    return iterator(node, _root);
  }

  const_iterator find(const key_type &key) const {
    const Node *node = _find_node_const(key);
    return const_iterator(node, _root);
  }

  size_type count(const key_type &key) const {
    return _find_node_const(key) != NULL ? 1 : 0;
  }

  iterator lower_bound(const key_type &key) {
    Node *current = _root;
    Node *result = NULL;
    while (current != NULL) {
      if (!_comp(current->value.first, key)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result, _root);
  }

  const_iterator lower_bound(const key_type &key) const {
    const Node *current = _root;
    const Node *result = NULL;
    while (current != NULL) {
      if (!_comp(current->value.first, key)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, _root);
  }

  iterator upper_bound(const key_type &key) {
    Node *current = _root;
    Node *result = NULL;
    while (current != NULL) {
      if (_comp(key, current->value.first)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result, _root);
  }

  const_iterator upper_bound(const key_type &key) const {
    const Node *current = _root;
    const Node *result = NULL;
    while (current != NULL) {
      if (_comp(key, current->value.first)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, _root);
  }

  ft::pair<iterator, iterator> equal_range(const key_type &key) {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }

  ft::pair<const_iterator, const_iterator>
  equal_range(const key_type &key) const {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }

  // ---------------- Observers ----------------
  key_compare key_comp() const { return _comp; }
  value_compare value_comp() const { return value_compare(_comp); }

private:
  // 키로 노드 찾기 (내부용)
  Node *_find_node(const key_type &key) {
    Node *current = _root;
    while (current != NULL) {
      if (_comp(key, current->value.first)) {
        current = current->left;
      } else if (_comp(current->value.first, key)) {
        current = current->right;
      } else {
        return current; // 찾음
      }
    }
    return NULL; // 못 찾음
  }

  // 키로 노드 찾기 (const 버전)
  const Node *_find_node_const(const key_type &key) const {
    const Node *current = _root;
    while (current != NULL) {
      if (_comp(key, current->value.first)) {
        current = current->left;
      } else if (_comp(current->value.first, key)) {
        current = current->right;
      } else {
        return current; // 찾음
      }
    }
    return NULL; // 못 찾음
  }

  // 트리 전체 삭제 (후위 순회)
  void _clear_tree(Node *node) {
    if (node == NULL)
      return;
    _clear_tree(node->left);
    _clear_tree(node->right);
    delete node;
    --_size;
  }

  // 서브트리의 최소값 노드 찾기
  Node *_minimum(Node *node) {
    while (node->left != NULL) {
      node = node->left;
    }
    return node;
  }

  // 노드 u를 노드 v로 대체 (transplant)
  void _transplant(Node *u, Node *v) {
    if (u->parent == NULL) {
      _root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != NULL) {
      v->parent = u->parent;
    }
  }

  // 노드 삭제
  void _erase_node(Node *z) {
    Node *y = z;
    Node *x = NULL;
    Node *x_parent = NULL; // x가 NULL일 때 부모 추적용
    bool y_original_red = y->red;

    if (z->left == NULL) {
      // Case 1: 왼쪽 자식 없음
      x = z->right;
      x_parent = z->parent;
      _transplant(z, z->right);
    } else if (z->right == NULL) {
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
    while (x != _root && (x == NULL || !x->red)) {
      if (x == (x_parent != NULL ? x_parent->left : NULL)) {
        // x가 왼쪽 자식인 경우
        Node *w = x_parent->right; // 형제

        if (w != NULL && w->red) {
          // Case 1: 형제가 RED
          w->red = false;
          x_parent->red = true;
          _rotate_left(x_parent);
          w = x_parent->right;
        }

        if (w == NULL || ((w->left == NULL || !w->left->red) &&
                          (w->right == NULL || !w->right->red))) {
          // Case 2: 형제의 양쪽 자식이 모두 BLACK
          if (w != NULL)
            w->red = true;
          x = x_parent;
          x_parent = x->parent;
        } else {
          if (w->right == NULL || !w->right->red) {
            // Case 3: 형제의 오른쪽 자식이 BLACK
            if (w->left != NULL)
              w->left->red = false;
            w->red = true;
            _rotate_right(w);
            w = x_parent->right;
          }
          // Case 4: 형제의 오른쪽 자식이 RED
          w->red = x_parent->red;
          x_parent->red = false;
          if (w->right != NULL)
            w->right->red = false;
          _rotate_left(x_parent);
          x = _root; // 종료
        }
      } else {
        // x가 오른쪽 자식인 경우 (대칭)
        Node *w = x_parent != NULL ? x_parent->left : NULL; // 형제

        if (w != NULL && w->red) {
          // Case 1: 형제가 RED
          w->red = false;
          x_parent->red = true;
          _rotate_right(x_parent);
          w = x_parent->left;
        }

        if (w == NULL || ((w->right == NULL || !w->right->red) &&
                          (w->left == NULL || !w->left->red))) {
          // Case 2: 형제의 양쪽 자식이 모두 BLACK
          if (w != NULL)
            w->red = true;
          x = x_parent;
          x_parent = x->parent;
        } else {
          if (w->left == NULL || !w->left->red) {
            // Case 3: 형제의 왼쪽 자식이 BLACK
            if (w->right != NULL)
              w->right->red = false;
            w->red = true;
            _rotate_left(w);
            w = x_parent->left;
          }
          // Case 4: 형제의 왼쪽 자식이 RED
          w->red = x_parent->red;
          x_parent->red = false;
          if (w->left != NULL)
            w->left->red = false;
          _rotate_right(x_parent);
          x = _root; // 종료
        }
      }
    }
    if (x != NULL)
      x->red = false;
  }

private:
  // ---------------- Helper functions ----------------

  // 삽입 위치의 부모 노드를 찾는 함수
  Node *_find_insert_position(const key_type &key, Node *node) {
    if (_comp(key, node->value.first)) {
      // key < node의 key -> 왼쪽으로
      if (node->left != NULL) {
        return _find_insert_position(key, node->left);
      }
      return node; // 삽입 위치의 부모
    } else if (_comp(node->value.first, key)) {
      // key > node의 key -> 오른쪽으로
      if (node->right != NULL) {
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

      if (grandparent == NULL)
        break;

      if (parent == grandparent->left) {
        // Case A: 부모가 조부모의 왼쪽 자식
        Node *uncle = grandparent->right;

        if (uncle != NULL && uncle->red) {
          // Case 1: Uncle이 RED -> Recoloring
          parent->red = false;
          uncle->red = false;
          grandparent->red = true;
          node = grandparent; // 위로 올라가며 계속 검사
        } else {
          // Case 2 & 3: Uncle이 BLACK (또는 NULL)
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

        if (uncle != NULL && uncle->red) {
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
    if (parent_node == NULL)
      return NULL;

    Node *grand_node = parent_node->parent;
    if (grand_node == NULL)
      return NULL;

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
    if (y->left != NULL) {
      y->left->parent = x;
    }

    // y의 부모를 x의 부모로
    y->parent = x->parent;
    if (x->parent == NULL) {
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
    if (y->right != NULL) {
      y->right->parent = x;
    }

    // y의 부모를 x의 부모로
    y->parent = x->parent;
    if (x->parent == NULL) {
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

// ---------------- Non-member functions ----------------
template <class Key, class T, class Compare, class Alloc>
bool operator==(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs) {
  if (lhs.size() != rhs.size())
    return false;
  typename map<Key, T, Compare, Alloc>::const_iterator it1 = lhs.begin();
  typename map<Key, T, Compare, Alloc>::const_iterator it2 = rhs.begin();
  while (it1 != lhs.end()) {
    if (it1->first != it2->first || it1->second != it2->second)
      return false;
    ++it1;
    ++it2;
  }
  return true;
}

template <class Key, class T, class Compare, class Alloc>
bool operator!=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs) {
  return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator<(const map<Key, T, Compare, Alloc> &lhs,
               const map<Key, T, Compare, Alloc> &rhs) {
  typename map<Key, T, Compare, Alloc>::const_iterator it1 = lhs.begin();
  typename map<Key, T, Compare, Alloc>::const_iterator it2 = rhs.begin();
  while (it1 != lhs.end() && it2 != rhs.end()) {
    if (it1->first < it2->first)
      return true;
    if (it2->first < it1->first)
      return false;
    if (it1->second < it2->second)
      return true;
    if (it2->second < it1->second)
      return false;
    ++it1;
    ++it2;
  }
  return it1 == lhs.end() && it2 != rhs.end();
}

template <class Key, class T, class Compare, class Alloc>
bool operator<=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs) {
  return !(rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>(const map<Key, T, Compare, Alloc> &lhs,
               const map<Key, T, Compare, Alloc> &rhs) {
  return rhs < lhs;
}

template <class Key, class T, class Compare, class Alloc>
bool operator>=(const map<Key, T, Compare, Alloc> &lhs,
                const map<Key, T, Compare, Alloc> &rhs) {
  return !(lhs < rhs);
}

template <class Key, class T, class Compare, class Alloc>
void swap(map<Key, T, Compare, Alloc> &lhs, map<Key, T, Compare, Alloc> &rhs) {
  lhs.swap(rhs);
}

} // namespace ft

#endif
