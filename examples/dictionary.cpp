#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie {
    static constexpr int ALPHA = 26;
    static constexpr int EMPTY = -1;

    struct Node {
        int child[ALPHA];
        int count = 0;

        Node() { fill(child, child + ALPHA, EMPTY); }
    };

    vector<Node> nodes;

    PersistentTrie(size_t max_nodes = 0) {
        if (max_nodes > 0) nodes.reserve(max_nodes);
    }

    int new_node(int from = EMPTY) {
        Node nd;
        if (from != EMPTY) nd = nodes[from];
        nodes.push_back(nd);
        return (int)nodes.size() - 1;
    }

    int empty_root() { return new_node(); }

    int insert(int old_root, const string& s) {
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

    int child(int v, int c) const { return v == EMPTY ? EMPTY : nodes[v].child[c]; }
    int count_of(int v) const { return v == EMPTY ? 0 : nodes[v].count; }

    int count_prefix(int root, const string& p) const {
        int u = root;
        for (char ch : p) {
            u = child(u, ch - 'a');
            if (u == EMPTY) return 0;
        }
        return count_of(u);
    }
};

struct Operation {
    int type = 0;
    int t = 0;
    string s;
};

int main() {
    cin.tie(0) -> sync_with_stdio(false);

    int Q;
    cin >> Q;

    vector<Operation> ops(Q + 1);
    size_t max_nodes = 2;
    for (int i = 1; i <= Q; ++i) {
        cin >> ops[i].type;
        if (ops[i].type == 2) {
            cin >> ops[i].t;
        }
        else {
            cin >> ops[i].s;
            if (ops[i].type == 1) max_nodes += ops[i].s.size() + 1;
        }
    }

    PersistentTrie trie(max_nodes);
    vector<int> root(Q + 1);
    root[0] = trie.empty_root();

    for (int i = 1; i <= Q; ++i) {
        if (ops[i].type == 1) {
            root[i] = trie.insert(root[i - 1], ops[i].s);
        }
        else if (ops[i].type == 2) {
            root[i] = root[ops[i].t];
        }
        else {
            cout << trie.count_prefix(root[i - 1], ops[i].s) << '\n';
            root[i] = root[i - 1];
        }
    }
    return 0;
}
