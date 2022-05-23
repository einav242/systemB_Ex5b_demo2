#include "OrgChart.hpp"
using namespace std;
namespace ariel
{

    OrgChart &OrgChart::add_root(string const &value)
    {
        if (root == nullptr) //if there is not root create one
        {
            this->root = new Node(value);
        }
        else
        {
            this->root->info = value; //if there is a root change the value
        }
        this->root->depth = 0;
        return *this;
    }

    OrgChart &OrgChart::add_sub(string const &value1, string const &value2)
    {
        bool check = false;
        vector<Node *> queue;
        if (root==nullptr) //if we want to add sub in a empty tree throw exption
        {
            throw std::invalid_argument("value does not exit");
        }
        queue.push_back(this->root);
        while (!queue.empty())
        {
            Node *tmp = queue[queue.size() - 1];
            if (tmp->info == value1)
            {
                Node *child = new Node(value2); //create a new node
                tmp->children.push_back(child);
                child->depth = tmp->depth + 1; //update the depth
                child->parent = tmp; //update the parent
                check = true; //we dont need to throw exception
                break;
            }
            queue.erase(queue.begin() + (int)queue.size() - 1);
            for (unsigned int i = 0; i < tmp->children.size(); i++)
            {
                unsigned int j = tmp->children.size() - 1 - i;
                queue.insert(queue.begin(), tmp->children[j]);
            }
        }
        if (!check) //If we did not find the requested Node we would throw an exception
        {
            throw std::invalid_argument("value does not exit");
        }

        return *this;
    }

    OrgChart &OrgChart::operator=(const OrgChart &other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete_tree(this->root);
        this->root = other.root;
        return *this;
    }

    void OrgChart::delete_tree(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        for (unsigned int i = 0; i < node->children.size(); i++)
        {
            delete_tree(node->children[i]);
        }
        delete node;
    }

    /********level oreder********/

    OrgChart::iterator_level_order &OrgChart::iterator_level_order::operator++()
    {
        /*like in a queue we enter the children of the curretn Node in the end of the vector 
         and update the current Node to be the the first Node in the vector*/
        if (this->current != nullptr)
        {
            for (unsigned int i = 0; i < this->current->children.size(); i++)
            {
                iter.push_back(this->current->children[i]);
            }
            iter.erase(iter.begin());
            if (!this->iter.empty())
            {
                this->current = this->iter[0];
            }
            else
            {
                this->current = nullptr;
            }
        }
        return *this;
    }

    OrgChart::iterator_level_order OrgChart::iterator_level_order::operator++(int a)
    {
          /*like in a queue we enter the children of the curretn Node in the end of the vector 
         and update the current Node to be the the first Node in the vector
         first return the value and then premote by one*/
        iterator_level_order tmp = *this;
        if (this->current != nullptr)
        {
            for (unsigned int i = 0; i < this->current->children.size(); i++)
            {
                iter.push_back(this->current->children[i]);
            }
            iter.erase(iter.begin());
            if (!this->iter.empty())
            {
                this->current = this->iter[0];
            }
            else
            {
                this->current = nullptr;
            }
        }
        return tmp;
    }

    bool OrgChart::iterator_level_order::operator==(const iterator_level_order &rhs) const
    {
        return this->current == rhs.current;
    }

    bool OrgChart::iterator_level_order::operator!=(const iterator_level_order &rhs) const
    {
        return this->current != rhs.current;
    }

    string &OrgChart::iterator_level_order::operator*() const
    {
        return this->current->info;
    }

    string *OrgChart::iterator_level_order::operator->() const
    {
        return &(this->current->info);
    }

    /********level oreder********/

    /********reverse oreder********/

    void OrgChart::iterator_reverse::create_vec_reverse_order(Node *ptr = nullptr)
    {
        /*
         To go through the tree in this method we will use a queue only this
         time every time we enter to the vector a node we will enter from 
        the beginning and so whoever enters last will be the first
        */
        if (ptr != nullptr)
        {
            this->iter.clear();
            vector<Node *> queue;
            queue.push_back(this->pointer_to_current_node);
            while (!queue.empty())
            {
                Node *tmp = queue[queue.size() - 1];
                this->iter.insert(this->iter.begin(), tmp);
                queue.erase(queue.begin() + (int)queue.size() - 1);
                for (unsigned int i = 0; i < tmp->children.size(); i++)
                {
                    unsigned int j = tmp->children.size() - 1 - i;//enter the children from left to the right
                    queue.insert(queue.begin(), tmp->children[j]);
                }
            }
            this->pointer_to_current_node = this->iter[0];
        }
    }

    OrgChart::iterator_reverse &OrgChart::iterator_reverse::operator++() //We will first advance the index and then send the iterator back
    {
        if (index == this->iter.size() - 1) //if we get to the end of the vector update the pointer to be null
        {
            this->pointer_to_current_node = nullptr;
            return *this;
        }
        this->index++;
        this->pointer_to_current_node = this->iter[this->index];
        return *this;
    }

    OrgChart::iterator_reverse OrgChart::iterator_reverse::operator++(int a) //We will first send the iterator back and then advance the index
    {
        if (index == this->iter.size() - 1)
        {
            this->pointer_to_current_node = nullptr;
            return *this;
        }
        iterator_reverse tmp = *this;
        index++;
        return tmp;
    }

    bool OrgChart::iterator_reverse::operator==(const iterator_reverse &rhs) const //We will check if the two pointers are points for the same address
    {
        return pointer_to_current_node == rhs.pointer_to_current_node;
    }

    bool OrgChart::iterator_reverse::operator!=(const iterator_reverse &rhs) const
    {
        return pointer_to_current_node != rhs.pointer_to_current_node;
    }

    string &OrgChart::iterator_reverse::operator*() const //return the value of the current node
    {
        return pointer_to_current_node->info;
    }

    string *OrgChart::iterator_reverse::operator->() const //return a pointer to the value of the current node
    {
        return &(this->pointer_to_current_node->info);
    }

    /********reverse oreder********/

    /********preoreder********/
    OrgChart::iterator_preorder &OrgChart::iterator_preorder::operator++()
    {
        /*like in the stack we enter the children of the current Node in the end of the vector and 
        update the current Node to be the last Node in the vector */
        if (this->current != nullptr)
        {
            iter.erase(iter.begin() + (int)this->iter.size() - 1);
            for (unsigned int i = 0; i < this->current->children.size(); i++)
            {
                unsigned int j = this->current->children.size() - 1 - i;
                iter.push_back(this->current->children[j]);
            }
            if (!this->iter.empty())
            {
                this->current = this->iter[this->iter.size() - 1];
            }
            else
            {
                this->current = nullptr;
            }
        }
        return *this;
    }

    OrgChart::iterator_preorder OrgChart::iterator_preorder::operator++(int a)
    {
        /*like in the stack we enter the children of the current Node in the end of the vector and 
        update the current Node to be the last Node in the vector */
        iterator_preorder tmp = *this;
        if (this->current != nullptr)
        {
            iter.erase(iter.begin() + (int)this->iter.size() - 1);
            for (unsigned int i = 0; i < this->current->children.size(); i++)
            {
                unsigned int j = this->current->children.size() - 1 - i;
                iter.push_back(this->current->children[j]);
            }
            iter.erase(iter.begin() + (int)this->iter.size() - 1);
            if (!this->iter.empty())
            {
                this->current = this->iter[this->iter.size() - 1];
            }
            else
            {
                this->current = nullptr;
            }
        }
        return tmp;
    }

    bool OrgChart::iterator_preorder::operator==(const iterator_preorder &rhs) const
    {
        return this->current == rhs.current;
    }

    bool OrgChart::iterator_preorder::operator!=(const iterator_preorder &rhs) const
    {
        return this->current != rhs.current;
    }

    string &OrgChart::iterator_preorder::operator*() const
    {
        return this->current->info;
    }

    string *OrgChart::iterator_preorder::operator->() const
    {
        return &(this->current->info);
    }

    /********preoreder********/

    OrgChart::iterator_level_order OrgChart::begin() const //so we can go throw the tree
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_level_order(this->root);
    }

    OrgChart::iterator_level_order OrgChart::end() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_level_order(nullptr);
    }

    OrgChart::iterator_level_order OrgChart::begin_level_order() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_level_order{root};
    }

    OrgChart::iterator_level_order OrgChart::end_level_order() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_level_order{nullptr};
    }

    OrgChart::iterator_reverse OrgChart::begin_reverse_order() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_reverse{root};
    }

    OrgChart::iterator_reverse OrgChart::reverse_order() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_reverse{nullptr};
    }

    OrgChart::iterator_preorder OrgChart::begin_preorder() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_preorder{root};
    }

    OrgChart::iterator_preorder OrgChart::end_preorder() const
    {
        if (root == nullptr)
        {
            throw std::invalid_argument("value does not exit");
        }
        return iterator_preorder{nullptr};
    }

    ostream &operator<<(ostream &out, const OrgChart &a)
    {
        vector<OrgChart::Node *> stack;
        stack.insert(stack.begin(), a.root);
        a.root->depth = 0;
        const int space = 6;
        while (!stack.empty())
        {
            OrgChart::Node *tmp = stack[0];
            out << "(" << tmp->depth << ")" << tmp->info << "-->";
            stack.erase(stack.begin());
            if (tmp->children.empty())
            {
                out << "nullptr\n\n";
                unsigned int s = 0;
                if (tmp->parent != nullptr)
                {
                    s = (unsigned int)stack[0]->depth * space + stack[0]->parent->info.size();
                }
                else
                {
                    s = (unsigned int)stack[0]->depth * space;
                }

                for (unsigned int i = 0; i < s; i++)
                {
                    out << " ";
                }
                if (!stack.empty())
                {
                    out << "-->";
                }
                continue;
            }
            for (unsigned int i = 0; i < tmp->children.size(); i++)
            {
                stack.insert(stack.begin(), tmp->children[i]);
            }
        }
        return out;
    }
}