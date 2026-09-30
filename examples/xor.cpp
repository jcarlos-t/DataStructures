#include <bits/stdc++.h>
using namespace std;

const int L = 16;
const int N = 100000;
const int NODES = N * (L + 1) + 1;
const int E = 2;

int n;
int q;
int nodes;
int root[N + 1];
int frec[NODES];
int trie[E][NODES];

int add_node(int basis = -1) {
    for (int i = 0; i < E; ++i) trie[i][nodes] = ~basis ? trie[i][basis] : 0;
    frec[nodes] = ~basis ? frec[basis] : 0;
    return nodes++;
}

void insert(int x, int last, int pos) {
    for (int i = L - 1; i >= 0; --i) {
        int c = (x >> i) & 1;
        if (last == -1 or trie[c][last] == 0) {
            trie[c][pos] = add_node();
        }
        else {
            trie[c][pos] = add_node(trie[c][last]);
        }
        last = ~last and trie[c][last] ? trie[c][last] : -1;
        pos = trie[c][pos];
        ++frec[pos];
    }
}

int maximize(int x, int last, int pos) {
    int res = 0;
    for (int i = L - 1; i >= 0; --i) {
        int c = (x >> i) & 1;
        int d = c ^ 1;
        int cnt_r = frec[trie[d][pos]];
        int cnt_l = last == -1 ? 0 : frec[trie[d][last]];
        if (cnt_r == cnt_l) d ^= 1;
        if (c ^ d) res |= (1 << i);
        pos = trie[d][pos];
        last = last == -1 or trie[d][last] == 0 ? -1 : trie[d][last];
    }
    return res;
}

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) {
        cin >> n >> q;
        nodes = 0;
        root[0] = add_node();
        for (int i = 1; i <= n; ++i) {
            int x;
            cin >> x;
            root[i] = add_node(root[i - 1]);
            insert(x, root[i - 1], root[i]);
        }
        while (q--) {
            int x, l, r;
            cin >> x >> l >> r;
            cout << maximize(x, root[l - 1], root[r]) << '\n';
        }
    }
    return 0;
}
