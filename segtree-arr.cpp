#include <iostream>
#include <vector>
using namespace::std;

template<typename data_type>
struct PersistentSegmentTree {

    vector<data_type> data;
    vector<int> lc, rc;
    vector<int> version_roots;

    int rango_l, rango_r;

    // Operacion con que se combinan nodos
    data_type merge(data_type a, data_type b) {
        return a + b;
    }

    int newNode() {
        data.emplace_back(data_type());
        lc.emplace_back(0);
        rc.emplace_back(0);
        return (int)data.size() - 1;
    }

    PersistentSegmentTree(int l, int r, vector<data_type> &a) : rango_l(l), rango_r(r) {
        newNode();    // indice 0 reservado como nodo nulo
        version_roots.emplace_back(build(l, r, a));
    }

    int build(int l, int r, vector<data_type> &a) {
        int node = newNode();
        if (l == r) {
            data[node] = a[l - 1]; // -1 porque a esta indexado en 0
            return node;
        }
        int mi = (l + r) / 2;
        lc[node] = build(l, mi, a);
        rc[node] = build(mi + 1, r, a);
        data[node] = merge(data[lc[node]], data[rc[node]]);
        return node;
    }

    int update(int pos, data_type value, int node, int l, int r) {
        int curr = newNode();
        data[curr] = data[node];
        lc[curr] = lc[node];
        rc[curr] = rc[node];
        if (l == r) {
            data[curr] = value;
            return curr;
        }
        int mi = (l + r) / 2;
        if (pos <= mi)
            lc[curr] = update(pos, value, lc[node], l, mi);
        else
            rc[curr] = update(pos, value, rc[node], mi + 1, r);
        data[curr] = merge(data[lc[curr]], data[rc[curr]]);
        return curr;
    }

    int update(int version, int pos, data_type value) {
        version_roots.emplace_back(update(pos, value, version_roots[version], rango_l, rango_r));
        return (int)version_roots.size() - 1;
    }

    data_type query(int x, int y, int node, int l, int r) {
        if (y < l or r < x or x > y) return data_type(0);
        if (x <= l and r <= y) return data[node];
        int mi = (l + r) / 2;
        return merge(query(x, y, lc[node], l, mi), query(x, y, rc[node], mi + 1, r));
    }

    data_type query(int version, int x, int y) {
        return query(x, y, version_roots[version], rango_l, rango_r);
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    PersistentSegmentTree<int> S(1, n, a);
    int q;
    cin >> q;
    while (q--) {
        string op;
        cin >> op;
        if (op[0] == 'c') {
            int i, j, x;
            cin >> i >> j >> x;
            S.update(i - 1, j, x);
        }
        else {
            int i, j;
            cin >> i >> j;
            cout << S.query(i - 1, j, j) << endl;
        }
    }
    return 0;
}
