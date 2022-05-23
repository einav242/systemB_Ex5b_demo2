#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <deque>
#include <iterator>
#include <iostream> // std::cout
#include <vector>
using namespace std;
#pragma once

namespace ariel
{
    class OrgChart
    {
        class Node
        {
        public:
            string info;
            Node *child;
            Node *brother;
            Node *parent;
            vector<Node *> children;
            int depth;
            Node(string const &data)
            {
                this->info = data;
                this->brother = nullptr;
                this->child = nullptr;
                this->parent = nullptr;
                this->depth = -1;
            }
        };

    public:
        Node *root;
        OrgChart()
        {
            this->root = nullptr;
        }
        OrgChart(const OrgChart &other)
        {
            this->root = other.root;
        }
        ~OrgChart()
        {
            delete_tree(this->root);
        }
        void delete_tree(Node *node);

        OrgChart &operator=(const OrgChart &other);

        OrgChart &add_root(string const &value);

        OrgChart &add_sub(string const &value1, string const &value2);

        class iterator_level_order
        {
        public:
            vector<Node *> iter;
            Node *current;
            iterator_level_order(Node *ptr)
            {
                this->iter.push_back(ptr);
                this->current = ptr;
            }

            iterator_level_order &operator++();

            iterator_level_order operator++(int a);

            bool operator==(const iterator_level_order &rhs) const;

            bool operator!=(const iterator_level_order &rhs) const;

            string &operator*() const;

            string *operator->() const;
        };

        class iterator_reverse
        {
        public:
            vector<Node *> iter;
            Node *pointer_to_current_node;
            unsigned int index;
            iterator_reverse(Node *ptr) //when we call the iterator we will also send the data transfer method
            {
                this->pointer_to_current_node = ptr;
                this->index = 0;
                this->iter.clear();
                create_vec_reverse_order(ptr);
            }

            void create_vec_reverse_order(Node *ptr);

            iterator_reverse &operator++();

            iterator_reverse operator++(int a);

            bool operator==(const iterator_reverse &rhs) const;

            bool operator!=(const iterator_reverse &rhs) const;

            string &operator*() const;

            string *operator->() const;
        };

        class iterator_preorder
        {
        public:
            vector<Node *> iter;
            Node *current;
            iterator_preorder(Node *ptr)
            {
                this->iter.clear();
                this->iter.push_back(ptr);
                this->current = ptr;
            }

            iterator_preorder &operator++();

            iterator_preorder operator++(int a);

            bool operator==(const iterator_preorder &rhs) const;

            bool operator!=(const iterator_preorder &rhs) const;

            string &operator*() const;

            string *operator->() const;
        };

        iterator_level_order begin() const;

        iterator_level_order end() const;

        iterator_level_order begin_level_order() const;

        iterator_level_order end_level_order() const;

        iterator_reverse begin_reverse_order() const;

        iterator_reverse reverse_order() const;

        iterator_preorder begin_preorder() const;

        iterator_preorder end_preorder() const;

        friend ostream &operator<<(ostream &out, const OrgChart &a);
    };
}