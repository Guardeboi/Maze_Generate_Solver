/* Your code here! */
#include "dsets.h"

DisjointSets::DisjointSets(int num) {
    addElements(num);
}

void DisjointSets::addElements(int num) {
    for (int i = 0; i < num; i++) {
        values_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (values_[elem] < 0) {
        return elem;
    } else {
        int root = find(values_[elem]);
        values_[elem] = root;
        return root;
    }
}

int DisjointSets::size(int elem) {
    return values_[find(elem)]*(-1);
}

int DisjointSets::getValue(int elem) const {
    return values_[elem];
}

void DisjointSets::setUnion(int a, int b) {
    int a_root = find(a);
    int b_root = find(b);
    if (a_root == b_root) { return; }
    int new_size = values_[a_root] + values_[b_root];
    if (values_[a_root] <= values_[b_root]) {
        values_[b_root] = a_root;
        values_[a_root] = new_size;
    } else {
        values_[a_root] = b_root;
        values_[b_root] = new_size;
    }
}

bool DisjointSets::Connected(int a, int b) {
    return find(a) == find(b);
}