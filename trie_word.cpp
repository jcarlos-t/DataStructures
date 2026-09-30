#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie {
    static constexpr int ALPHA = 26;
    static constexpr int EMPTY = -1;

    struct Node {
        int child[ALPHA];
        int count = 0;

        Node() {
            std::fill(child, child + ALPHA, EMPTY);
        }
    };

    vector<Node> nodes;

    PersistentTrie(int max_elements = 0, int max_len = 0) {
        if (max_elements > 0) {
            nodes.reserve(1 + (size_t)max_elements * (max_len + 1));
        }
    }

    int new_node(int from = EMPTY) {
        Node nd;
        if (from != EMPTY) {
            nd = nodes[from];
        }
        nodes.push_back(nd);
        return (int)nodes.size() - 1;
    }

    int empty_root() {
        return new_node();
    }

    int child(int v, int c) const {
        return (v == EMPTY) ? EMPTY : nodes[v].child[c];
    }

    int count_of(int v) const {
        return (v == EMPTY) ? 0 : nodes[v].count;
    }

    int insert(int old_root, const string &s) {
        int new_root = new_node(old_root);
        ++nodes[new_root].count;

        int u = old_root;
        int v = new_root;

        for (char ch : s) {
            int c = ch - 'a';
            int old_child = child(u, c);
            int new_child = new_node(old_child);

            nodes[v].child[c] = new_child;

            u = old_child;
            v = new_child;
            ++nodes[v].count;
        }

        return new_root;
    }

    int count_prefix(int root, const string &p) const {
        int u = root;
        for (char ch : p) {
            int c = ch - 'a';
            u = child(u, c);
            if (u == EMPTY) return 0;
        }
        return count_of(u);
    }

    int range_count_prefix(int u_root, int v_root, const string &p) const {
        int a = u_root;
        int b = v_root;
        for (char ch : p) {
            int c = ch - 'a';
            a = child(a, c);
            b = child(b, c);
            if (b == EMPTY) return 0;
        }
        return count_of(b) - count_of(a);
    }

    int count_equal(int u_root, int v_root, const string &s) const {
        int a = u_root;
        int b = v_root;
        for (char ch : s) {
            int c = ch - 'a';
            a = child(a, c);
            b = child(b, c);
        }
        return count_of(b) - count_of(a);
    }
};
