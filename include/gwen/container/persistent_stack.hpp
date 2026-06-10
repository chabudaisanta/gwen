#pragma once

#include "gwen/types.hpp"
namespace gwen {

template <typename T> struct persistent_stack {
    struct node {
        T val;
        i32 sz;
        node* par;

        node(T x, i32 s, node* p) : val(x), sz(s), par(p) {}
    };

    node* ptr;

    i32 size() const { return ptr ? ptr->sz : 0; }
    bool empty() const { return !ptr; }
    void clear() { ptr = nullptr; }

    void push(T x) {
        node* new_node = new node(x, size() + 1, ptr);
        ptr = new_node;
    }
    void pop() { ptr = ptr->par; }
    T top() { return ptr->val; }
};

}  // namespace gwen