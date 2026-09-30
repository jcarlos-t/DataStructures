#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie {
    static constexpr int EMPTY = -1;

    struct Node {
        int child[2] = {EMPTY, EMPTY};
        int count = 0;
    };

    int bits;
    vector<Node> nodes;

    PersistentTrie(int bits, int max_elements = 0) : bits(bits) {
        if (max_elements > 0)
            nodes.reserve(1 + (size_t)max_elements * (bits + 1));
    }

    int new_node(int from = EMPTY) {
        Node nd;
        if (from != EMPTY) nd = nodes[from];
        nodes.push_back(nd);
        return (int)nodes.size() - 1;
    }

    int empty_root() { return new_node(); }

    int insert(int old_root, int x) {
        int new_root = new_node(old_root);
        ++nodes[new_root].count;
        int u = old_root;
        int v = new_root;
        for (int i = bits - 1; i >= 0; --i) {
            int c = (x >> i) & 1;
            int old_child = child(u, c);
            int new_child = new_node(old_child);
            nodes[v].child[c] = new_child;
            u = old_child;
            v = new_child;
            ++nodes[v].count;
        }
        return new_root;
    }

    int child(int v, int c) const { return v == EMPTY ? EMPTY : nodes[v].child[c]; }
    int count_of(int v) const { return v == EMPTY ? 0 : nodes[v].count; }

    int range_count(int u, int v) const { return count_of(v) - count_of(u); }

    int count(int lo, int hi) const { return range_count(lo, hi); }

    int count_equal(int lo, int hi, int x) const {
        int u = lo, v = hi;
        for (int i = bits - 1; i >= 0; --i) {
            int c = (x >> i) & 1;
            u = child(u, c);
            v = child(v, c);
        }
        return range_count(u, v);
    }
};
