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
        };

        Node root = Node()  // Map의 생성자에서 처리해줄꺼야

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
            parent = find(pair_to_be_inserted.first);  // parent라는 변수명이 삽입되는 애 기준임
            Node node_to_be_inserted = Node(pair_to_be_inserted)

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

        void search(value_type to_find) {
            Node temp_node = Node(temp)
            Node* node = &temp_node;
        }

        bool check_reconstruct(Node new_node) {
            Node uncle_node = new_node.parent.parent.right;

            if(new_node.parent.red && !uncle_node.red) {
                return true;
            }
            return false;
        }

        bool check_recoloring() {
            Node uncle_node = new_node.parent.parent.right;

            if(new_node.parent.red && uncle_node.red) {
                return true;
            }
            return false;
        }
    };

} // namespace ft

#endif
