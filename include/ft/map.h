#ifndef FT_MAP_H
#define FT_MAP_H

#include <cstddef>
#include <functional>
#include "pair.h"

namespace ft
{

    template <class Key, class T, class Compare = std::less<Key> >
    class map
    {
      public:
        // ---------------- Member types ----------------
        typedef Key                    key_type;
        typedef T                      mapped_type;
        typedef ft::pair<const Key, T> value_type;
        typedef Compare                key_compare;
        typedef std::size_t            size_type;

      private:
        // ---------------- Node definition ----------------
        struct Node
        {
            value_type value;
            Node      *parent;
            Node      *left;
            Node      *right;
            bool       red;

            Node(const value_type &v) : value(v), parent(nullptr), left(nullptr), right(nullptr), red(true) {}

			bool operator<(const Node& other) {
				return value.first() < other.value.first();
			}
        };

        Node root = Node();  // Map의 생성자에서 처리해줄꺼야

        Node find(key_type key_to_be_inserted, Node node) {  // 일단 진짜 찾는건 나중에 생각해, 예외처리 나중에
            key_type key = node.value.fisrt;

            if(key > key_to_be_inserted) {
                if(node.left != nullptr) {
                    return find(node.left);
                }
            }
            else if(key < key_to_be_inserted) {
                if(node.right != nullptr) {
                    return find(node.right);
                }
            }
            // key == key_to_be_inserted 이거나 node가 들어갈 위치가 nullptr 즉 들어가야하는 위치인 경우
            return node;
        }

        void insert(value_type pair_to_be_inserted) {
            Node parent = find(pair_to_be_inserted.first);  // parent라는 변수명이 삽입되는 애 기준임
            Node node_to_be_inserted = Node(pair_to_be_inserted);

            if(parent.first > pair_to_be_inserted.first) {
                parent.left = node_to_be_inserted;
            }
            else if(parent.first < pair_to_be_inserted.first) {
                parent.right = node_to_be_inserted;
            }
            // TODO: 같은(equal) 경우는 예외 처리 해줘야해 일단 넘기겠습니다.

            // TODO: 규칙을 위배하는 친구들 찾고 해결하기


        }

        void erase() {

        }

        void search(value_type to_find) {   // TODO: yet implemented
            // Node temp_node = Node(temp);
            Node* node = &temp_node;
        }

        bool check_reconstruct(Node* new_node) {
            Node* uncle_node = new_node->parent->parent->right;

            if(new_node->parent->red && !uncle_node->red) {
                return true;
            }
            return false;
        }

        bool check_recoloring(Node* new_node) {
            Node* uncle_node = new_node->parent->parent->right;

            if(new_node->parent->red && uncle_node->red) {
                return true;
            }
            return false;
        }

        // node를 이동시키면, parent, left, right 다 update 시켜줘야해
        // reconstruct의 경우 부모 노드를 검정색으로 바꿔서 문제 없음
        // Uncle node가 가장 큼으로 오른쪽 아래 -> 오른쪽 아래로 가게 됨 따라서 자식 노드를 빨강으로 바꿔도 아무 문제가 생기지 않음
        // => 연쇄적으로 재구성(?)들이 발생하지 않아. + 재구성이 일어나는 상황은 RBT의 모든 리프 노드에서 Black Depth는 같다. 에 의해 생기지 않아.
        // rotate로 subtree가 이동해도 문제가 생기지 않아 -> 왜나면 uncle이 무조건 black인 경우에만 발생하니까
		void reconstruct(Node* new_node) {

		}

        //   (x)                       (y)
        //   / \     Left Rotate(x)    / \
        //  α  (y)   ------------->  (x)  γ
        //     / \                   / \
        //    β   γ  <------------- α   β
        //           Right Rotate(y)

        // nullptr로 만들어줘야 할 애들은 없어 보임
        void _rotate_left(Node* x) {
            Node* a = x->left;
            Node* y = x->right;
            Node* c = y->right;
            // 아래 코드는 nullptr일수도 있어
            Node* b = y->left;

            Node* parent = x->parent;
            y->parent = parent;
            if (parent != nullptr) {
                // x가 parent의 오른이면 오른, 왼이면 왼으로 update해줘야 해
                if (parent->left == x) {
                    parent->left = y;
                }
                else {
                    parent->right = y;
                }
            }
            else {
                this->root = y;
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

        void _rotate_right(Node* x) {
            Node* a = x->right;
            Node* y = x->left;
            Node* c = y->left;
            // 아래 코드는 nullptr일수도 있어
            Node* b = y->right; // left -> right

            Node* parent = x->parent;
            y->parent = parent;

            if (parent != nullptr) {
                // x가 parent의 오른이면 오른, 왼이면 왼으로 update
                if (parent->left == x) {
                    parent->left = y;
                }
                else {
                    parent->right = y;
                }
            }
            else {
                this->root = y;
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
    };

} // namespace ft

#endif
